/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include "kchart.h"

using namespace StockCharts;

KChart::KChart(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

KChart::~KChart()
{
}

void KChart::init(std::shared_ptr<StockCharts::StockCore> stockCore, bool main)
{
    m_stockCore = stockCore;

    m_model = std::make_shared<ChartModel>(m_stockCore);
    m_model->addPlugin<PluginBG>();
    if (main)
        m_model->addPlugin<PluginKLine>();
    m_model->addPlugin<PluginIndicator>();
    m_model->addPlugin<PluginCrossLine>();

    ui.chart->init(std::make_shared<ChartVM>(m_model));
    bind(ui.chart);
}

void KChart::addSyncChart(KChart* otherChart)
{
    bind(otherChart);
}

void KChart::on(DataBinding* sender, const std::string& id)
{
    if (sender == ui.chart) {
        fire(id);
        return;
    }

    auto otherChart = static_cast<KChart*>(sender);
    if (!otherChart)
        return;
    auto ctx = *otherChart->getContext();

    if (id == ID_OnMouseMove || id == ID_OnMouseLeave)
        getChartView()->slotSyncMouseMove(ctx.hoverIndex, ctx.hoverPrice);
    else if (id == ID_OnScrollX || id == ID_OnWheelY)
        getChartView()->slotSyncViewCount(ctx.viewCount, ctx.beginIndex, ctx.endIndex, ctx.props.nodeWidth, ctx.props.stickWidth);
}

std::shared_ptr<const StIndicator> KChart::addIndicator(const IndexFormula& formula)
{
    auto pluginIndicator = m_model->getPlugin<PluginIndicator>();
    if (!pluginIndicator)
        return nullptr;
    auto indicator = pluginIndicator->addIndicator(formula);

    ui.chart->slotCalcContext();
    return indicator;
}

void KChart::clearIndicators()
{
    auto pluginIndicator = m_model->getPlugin<PluginIndicator>();
    if (!pluginIndicator)
        return;
    pluginIndicator->delIndicators();

    ui.chart->slotCalcContext();
}
