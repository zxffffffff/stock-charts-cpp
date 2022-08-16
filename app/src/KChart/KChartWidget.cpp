/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include "KChartWidget.h"
#include <QResizeEvent>
#include <QDebug>

using namespace StockCharts;

KChartWidget::KChartWidget(QWidget* parent)
    : QWidget(parent)
{
    QWidget* p = this;
    while (p) {
        p->setMouseTracking(true); // for mousemove
        p = p->parentWidget();
    }
    setFocusPolicy(Qt::StrongFocus); // for keypress
}

KChartWidget::~KChartWidget()
{
}

void KChartWidget::init(std::shared_ptr<ChartVM> vm)
{
    m_vm = vm;
    bind(m_vm.get());
}

void KChartWidget::syncSubChart(KChartWidget* otherChart)
{
    bind(otherChart);
}

void KChartWidget::on(DataBinding* sender, const std::string& id)
{
    if (m_vm.get() == sender) {
        fire(id);
        update();
    }
    else {
        auto otherChart = static_cast<KChartWidget*>(sender);
        const auto& ctx = *otherChart->getContext();

        if (id == ID_OnMouseMove || id == ID_OnMouseLeave)
            slotSyncMouseMove(ctx.hoverIndex, ctx.hoverPrice);
        else if (id == ID_OnScrollX || id == ID_OnWheelY)
            slotSyncViewCount(ctx.viewCount, ctx.beginIndex, ctx.endIndex, ctx.nodeWidth, ctx.stickWidth);
    }
}

void KChartWidget::slotCalcContext()
{
    m_vm->calcContext();
}

void KChartWidget::slotSyncViewCount(int viewCount, int beginIndex, int endIndex, Real nodeWidth, Real stickWidth)
{
    m_vm->SyncViewCount(viewCount, beginIndex, endIndex, nodeWidth, stickWidth);
}

void KChartWidget::slotSyncMouseMove(int hoverIndex, Number hoverPrice)
{
    m_vm->SyncMouseMove(hoverIndex, hoverPrice);
}

void KChartWidget::paintEvent(QPaintEvent* event)
{
    PainterQt painter(this);
    m_vm->OnPaint(painter);
}

void KChartWidget::resizeEvent(QResizeEvent* event)
{
    m_vm->OnResize(Rect(0, 0, event->size().width(), event->size().height()));
}

void KChartWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_vm->OnMouseMove(Point(event->pos().x(), event->pos().y()));
}

void KChartWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        m_vm->OnScrollX(-1);
        break;
    case Qt::Key_Right:
        m_vm->OnScrollX(1);
        break;
    case Qt::Key_Up:
        m_vm->OnWheelY(1);
        break;
    case Qt::Key_Down:
        m_vm->OnWheelY(-1);
        break;
    default:
        return;
    }
}

void KChartWidget::wheelEvent(QWheelEvent* event)
{
    int yStep = event->angleDelta().y() / 120;
    int xStep = event->angleDelta().x() / 120;
    if (yStep != 0)
        m_vm->OnWheelY(yStep);
    if (xStep != 0)
        m_vm->OnScrollX(xStep);
}

void KChartWidget::leaveEvent(QEvent* event)
{
    m_vm->OnMouseLeave();
}
