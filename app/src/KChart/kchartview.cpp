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

void KChartView::slotCalcContext()
{
    m_vm->calcContext();
    update();
}

void KChartView::slotDrawingType(int i)
{
    m_vm->setDrawingType((EnKLineType)i);
    update();
}

void KChartView::slotCorrdinate(int i)
{
    m_vm->setCorrdinate((EnCoordinateType)i);
    update();
}

void KChartView::slotYLWidth(int i)
{
    m_vm->setYLWidth(i);
    update();
}

void KChartView::slotYRWidth(int i)
{
    m_vm->setYRWidth(i);
    update();
}

void KChartView::slotXHeight(int i)
{
    m_vm->setXHeight(i);
    update();
}

void KChartView::slotPaddingLeft(int i)
{
    m_vm->setPaddingLeft(i);
    update();
}

void KChartView::slotPaddingTop(int i)
{
    m_vm->setPaddingTop(i);
    update();
}

void KChartView::slotPaddingRight(int i)
{
    m_vm->setPaddingRight(i);
    update();
}

void KChartView::slotPaddingBottom(int i)
{
    m_vm->setPaddingBottom(i);
    update();
}

void KChartView::slotNodeWidth(int nodeWidth)
{
    m_vm->setNodeWidth(nodeWidth);
    update();
}

void KChartView::slotStickWidth(int stickWidth)
{
    m_vm->setStickWidth(stickWidth);
    update();
}

void KChartView::slotSyncViewCount(int viewCount, int beginIndex, int endIndex)
{
    m_vm->SyncViewCount(viewCount, beginIndex, endIndex);
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
