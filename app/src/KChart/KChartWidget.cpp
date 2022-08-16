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
    m_vm->setSyncOther(otherChart->m_vm.get());
}

void KChartWidget::on(DataBinding* sender, const std::string& id)
{
    if (m_vm.get() == sender) {
        fire(id);
        update();
    }
}

void KChartWidget::paintEvent(QPaintEvent* event)
{
    PainterQt painter(this);
    m_vm->onPaint(painter);
}

void KChartWidget::resizeEvent(QResizeEvent* event)
{
    m_vm->onResize(Rect(0, 0, event->size().width(), event->size().height()));
}

void KChartWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_vm->onMouseMove(Point(event->pos().x(), event->pos().y()));
}

void KChartWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        m_vm->onScrollX(-1);
        break;
    case Qt::Key_Right:
        m_vm->onScrollX(1);
        break;
    case Qt::Key_Up:
        m_vm->onWheelY(1);
        break;
    case Qt::Key_Down:
        m_vm->onWheelY(-1);
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
        m_vm->onWheelY(yStep);
    if (xStep != 0)
        m_vm->onScrollX(xStep);
}

void KChartWidget::leaveEvent(QEvent* event)
{
    m_vm->onMouseLeave();
}
