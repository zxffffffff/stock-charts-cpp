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
}

std::shared_ptr<const StIndicator> KChartView::addIndicator(const IndexFormula& formula)
{
    auto pluginIndicator = m_model->getPlugin<PluginIndicator>();
    if (!pluginIndicator)
        return nullptr;
    auto indicator = pluginIndicator->addIndicator(formula);

    m_vm->calcContext();
    return indicator;
}

void KChartView::clearIndicators()
{
    auto pluginIndicator = m_model->getPlugin<PluginIndicator>();
    if (!pluginIndicator)
        return;
    pluginIndicator->delIndicators();

    m_vm->calcContext();
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
    case QEvent::Leave:
    {
        m_vm->OnMouseLeave();
        ui->body->update();
    } break;
    }
    return __super::eventFilter(obj, event);
}

