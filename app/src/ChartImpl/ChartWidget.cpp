/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include "ChartWidget.h"
#include <QResizeEvent>
#include <QDebug>

using namespace StockCharts;

ChartWidget::ChartWidget(QWidget* parent)
    : QWidget(parent)
{
    QWidget* p = this;
    while (p) {
        p->setMouseTracking(true); // for mousemove
        p = p->parentWidget();
    }
    setFocusPolicy(Qt::StrongFocus); // for keypress
}

ChartWidget::~ChartWidget()
{
}

void ChartWidget::init(std::shared_ptr<ChartView> view)
{
    m_view = view;
    bind(m_view.get());
}

void ChartWidget::syncSubChart(ChartWidget* otherChart)
{
    m_view->setSyncOther(otherChart->m_view.get());
}

void ChartWidget::on(DataBinding* sender, const std::string& id)
{
    if (m_view.get() == sender) {
        fire(id);
        update();
    }
}

void ChartWidget::slotDrawingType(int i)
{
    auto props = *m_view->getProps();
    if (props.lineType == (EnStockLineType)i)
        return;
    props.lineType = (EnStockLineType)i;
    m_view->setProps(props);
}

void ChartWidget::slotCorrdinate(int i)
{
    auto props = *m_view->getProps();
    if (props.yCoordType == (EnYCoordinateType)i)
        return;
    props.yCoordType = (EnYCoordinateType)i;
    m_view->setProps(props);
}

void ChartWidget::slotYLWidth(int i)
{
    auto props = *m_view->getProps();
    if (props.ylAxisWidth == i)
        return;
    props.ylAxisWidth = i;
    m_view->setProps(props);
}

void ChartWidget::slotYRWidth(int i)
{
    auto props = *m_view->getProps();
    if (props.yrAxisWidth == i)
        return;
    props.yrAxisWidth = i;
    m_view->setProps(props);
}

void ChartWidget::slotXHeight(int i)
{
    auto props = *m_view->getProps();
    if (props.xAxisHeight == i)
        return;
    props.xAxisHeight = i;
    m_view->setProps(props);
}

void ChartWidget::slotPaddingLeft(int i)
{
    auto props = *m_view->getProps();
    if (props.paddingLeft == i)
        return;
    props.paddingLeft = i;
    m_view->setProps(props);
}

void ChartWidget::slotPaddingTop(int i)
{
    auto props = *m_view->getProps();
    if (props.paddingTop == i)
        return;
    props.paddingTop = i;
    m_view->setProps(props);
}

void ChartWidget::slotPaddingRight(int i)
{
    auto props = *m_view->getProps();
    if (props.paddingRight == i)
        return;
    props.paddingRight = i;
    m_view->setProps(props);
}

void ChartWidget::slotPaddingBottom(int i)
{
    auto props = *m_view->getProps();
    if (props.paddingBottom == i)
        return;
    props.paddingBottom = i;
    m_view->setProps(props);
}

void ChartWidget::paintEvent(QPaintEvent* event)
{
    PainterQt painter(this);
    m_view->onPaint(painter);
}

void ChartWidget::resizeEvent(QResizeEvent* event)
{
    m_view->onResize(Rect(0, 0, event->size().width(), event->size().height()));
}

void ChartWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_view->onMouseMove(Point(event->pos().x(), event->pos().y()));
}

void ChartWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        m_view->onScrollX(-1);
        break;
    case Qt::Key_Right:
        m_view->onScrollX(1);
        break;
    case Qt::Key_Up:
        m_view->onWheelY(1);
        break;
    case Qt::Key_Down:
        m_view->onWheelY(-1);
        break;
    default:
        return;
    }
}

void ChartWidget::wheelEvent(QWheelEvent* event)
{
    int yStep = event->angleDelta().y() / 120;
    int xStep = event->angleDelta().x() / 120;
    if (yStep != 0) {
        auto& ctx = *getContext();
        if (ctx.crossLineVisible)
            m_view->onWheelY(yStep);
        else
            m_view->onScrollX(-yStep);
    }
    if (xStep != 0) {
        m_view->onScrollX(xStep);
    }
}

void ChartWidget::leaveEvent(QEvent* event)
{
    m_view->onMouseLeave();
}

void ChartWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    m_view->onDBClick(Point(event->pos().x(), event->pos().y()));
}
