/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "kchartview.h"
#include "ui_kchartview.h"
#include <QResizeEvent>
#include <QDebug>

using namespace StockCharts;

KChartView::KChartView(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::KChartView)
{
    ui->setupUi(this);
    QWidget* p = ui->body;
    while (p) {
        p->setMouseTracking(true);
        p = p->parentWidget();
    }
    ui->body->installEventFilter(this);
}

KChartView::~KChartView()
{
    delete ui;
}

void KChartView::init(std::shared_ptr<StockCore> stockCore, bool main)
{
    m_stockCore = stockCore;

    m_model = std::make_shared<ChartModel>(m_stockCore);
    m_model->addPlugin<PluginBG>();
    if (main)
        m_model->addPlugin<PluginKLine>();
    m_model->addPlugin<PluginIndicator>();
    m_model->addPlugin<PluginCrossLine>();

    m_vm = std::make_shared<ChartVM>(m_model);
    ui->body->update();
}

std::shared_ptr<const StIndicator> KChartView::addIndicator(const IndexFormula& formula)
{
    auto pluginIndicator = m_model->getPlugin<PluginIndicator>();
    if (!pluginIndicator)
        return nullptr;
    auto indicator = pluginIndicator->addIndicator(formula);

    m_vm->calcContext();
    ui->body->update();
    return indicator;
}

void KChartView::clearIndicators()
{
    auto pluginIndicator = m_model->getPlugin<PluginIndicator>();
    if (!pluginIndicator)
        return;
    pluginIndicator->delIndicators();

    m_vm->calcContext();
    ui->body->update();
}

void KChartView::slotDrawingType(int i)
{
    m_vm->setDrawingType((EnKLineType)i);
    ui->body->update();
}

void KChartView::slotCorrdinate(int i)
{
    m_vm->setCorrdinate((EnCoordinateType)i);
    ui->body->update();
}

void KChartView::slotYLWidth(int i)
{
    m_vm->setYLWidth(i);
    ui->body->update();
}

void KChartView::slotYRWidth(int i)
{
    m_vm->setYRWidth(i);
    ui->body->update();
}

void KChartView::slotXHeight(int i)
{
    m_vm->setXHeight(i);
    ui->body->update();
}

void KChartView::slotPaddingLeft(int i)
{
    m_vm->setPaddingLeft(i);
    ui->body->update();
}

void KChartView::slotPaddingTop(int i)
{
    m_vm->setPaddingTop(i);
    ui->body->update();
}

void KChartView::slotPaddingRight(int i)
{
    m_vm->setPaddingRight(i);
    ui->body->update();
}

void KChartView::slotPaddingBottom(int i)
{
    m_vm->setPaddingBottom(i);
    ui->body->update();
}

void KChartView::slotNodeStickWidth(int nodeWidth, int stickWidth)
{
    m_vm->setNodeStickWidth(nodeWidth, stickWidth);
    ui->body->update();
}

bool KChartView::eventFilter(QObject* obj, QEvent* event)
{
    if (obj != ui->body)
        return false;

    switch (event->type())
    {
    case QEvent::Resize:
    {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
        m_vm->OnResize(Rect(0, 0, resizeEvent->size().width(), resizeEvent->size().height()));
    } break;
    case QEvent::Paint:
    {
        QPaintEvent* paintEvent = static_cast<QPaintEvent*>(event);
        PainterQt painter(ui->body);
        m_vm->OnPaint(painter);
    } break;
    case QEvent::MouseMove:
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        m_vm->OnMouseMove(Point(mouseEvent->pos().x(), mouseEvent->pos().y()));
        ui->body->update();
    } break;
    case QEvent::KeyPress:
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key())
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
        }
        ui->body->update();
    } break;
    case QEvent::Wheel:
    {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        int yStep = wheelEvent->angleDelta().y() / 120;
        int xStep = wheelEvent->angleDelta().x() / 120;
        if (yStep != 0)
            m_vm->OnWheelY(yStep);
        if (xStep != 0)
            m_vm->OnScrollX(xStep);
        ui->body->update();
    } break;
    case QEvent::Leave:
    {
        m_vm->OnMouseLeave();
        ui->body->update();
    } break;
    }
    return false;
}

