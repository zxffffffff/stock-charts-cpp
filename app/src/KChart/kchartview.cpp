/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include "kchartview.h"
#include <QResizeEvent>
#include <QDebug>

using namespace StockCharts;

KChartView::KChartView(QWidget* parent)
    : QWidget(parent)
{
}

KChartView::~KChartView()
{
}

void KChartView::init(std::shared_ptr<ChartVM> vm)
{
    m_vm = vm;
    bind(m_vm.get());
    update();
}

void KChartView::on(DataBinding* sender, const std::string& id)
{
    if (m_vm.get() == sender)
        fire(id);
}

void KChartView::slotCalcContext()
{
    m_vm->calcContext();
    update();
}

void KChartView::slotDrawingType(int i)
{
    auto ctx = m_vm->getContext();
    if (ctx->props.klineType == (EnKLineType)i)
        return;
    ChartProps props = ctx->props;
    props.klineType = (EnKLineType)i;
    m_vm->setProps(props);
    update();
}

void KChartView::slotCorrdinate(int i)
{
    auto ctx = m_vm->getContext();
    if (ctx->props.coordinateType == (EnCoordinateType)i)
        return;
    ChartProps props = ctx->props;
    props.coordinateType = (EnCoordinateType)i;
    m_vm->setProps(props);
    update();
}

void KChartView::slotYLWidth(int i)
{
    auto ctx = m_vm->getContext();
    if (ctx->props.ylAxisWidth == i)
        return;
    ChartProps props = ctx->props;
    props.ylAxisWidth = i;
    m_vm->setProps(props);
    update();
}

void KChartView::slotYRWidth(int i)
{
    auto ctx = m_vm->getContext();
    if (ctx->props.yrAxisWidth == i)
        return;
    ChartProps props = ctx->props;
    props.yrAxisWidth = i;
    m_vm->setProps(props);
    update();
}

void KChartView::slotXHeight(int i)
{
    auto ctx = m_vm->getContext();
    if (ctx->props.xAxisHeight == i)
        return;
    ChartProps props = ctx->props;
    props.xAxisHeight = i;
    m_vm->setProps(props);
    update();
}

void KChartView::slotPaddingLeft(int i)
{
    auto ctx = m_vm->getContext();
    if (ctx->props.paddingLeft == i)
        return;
    ChartProps props = ctx->props;
    props.paddingLeft = i;
    m_vm->setProps(props);
    update();
}

void KChartView::slotPaddingTop(int i)
{
    auto ctx = m_vm->getContext();
    if (ctx->props.paddingTop == i)
        return;
    ChartProps props = ctx->props;
    props.paddingTop = i;
    m_vm->setProps(props);
    update();
}

void KChartView::slotPaddingRight(int i)
{
    auto ctx = m_vm->getContext();
    if (ctx->props.paddingRight == i)
        return;
    ChartProps props = ctx->props;
    props.paddingRight = i;
    m_vm->setProps(props);
    update();
}

void KChartView::slotPaddingBottom(int i)
{
    auto ctx = m_vm->getContext();
    if (ctx->props.paddingBottom == i)
        return;
    ChartProps props = ctx->props;
    props.paddingBottom = i;
    m_vm->setProps(props);
    update();
}

void KChartView::slotNodeWidth(int nodeWidth)
{
    auto ctx = m_vm->getContext();
    if (ctx->props.nodeWidth == nodeWidth)
        return;
    ChartProps props = ctx->props;
    props.nodeWidth = nodeWidth;
    m_vm->setProps(props);
    update();
}

void KChartView::slotStickWidth(int stickWidth)
{
    auto ctx = m_vm->getContext();
    if (ctx->props.stickWidth == stickWidth)
        return;
    ChartProps props = ctx->props;
    props.stickWidth = stickWidth;
    m_vm->setProps(props);
    update();
}

void KChartView::slotSyncViewCount(int viewCount, int beginIndex, int endIndex, Real nodeWidth, Real stickWidth)
{
    m_vm->SyncViewCount(viewCount, beginIndex, endIndex, nodeWidth, stickWidth);
    update();
}

void KChartView::slotSyncMouseMove(int hoverIndex, Number hoverPrice)
{
    m_vm->SyncMouseMove(hoverIndex, hoverPrice);
    update();
}

void KChartView::paintEvent(QPaintEvent* event)
{
    PainterQt painter(this);
    m_vm->OnPaint(painter);
}

void KChartView::resizeEvent(QResizeEvent* event)
{
    m_vm->OnResize(Rect(0, 0, event->size().width(), event->size().height()));
}

void KChartView::mouseMoveEvent(QMouseEvent* event)
{
    m_vm->OnMouseMove(Point(event->pos().x(), event->pos().y()));
    update();
}

void KChartView::keyPressEvent(QKeyEvent* event)
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
    update();
}

void KChartView::wheelEvent(QWheelEvent* event)
{
    int yStep = event->angleDelta().y() / 120;
    int xStep = event->angleDelta().x() / 120;
    if (yStep != 0)
        m_vm->OnWheelY(yStep);
    if (xStep != 0)
        m_vm->OnScrollX(xStep);
    update();
}

void KChartView::leaveEvent(QEvent* event)
{
    m_vm->OnMouseLeave();
    update();
}
