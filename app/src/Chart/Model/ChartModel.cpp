#include "ChartModel.h"
#include "iostream"

using namespace StockCharts;

ChartModel::ChartModel(std::shared_ptr<StockCore> stockCore, std::shared_ptr<ChartProps> props)
{
    m_stockCore = (stockCore ? stockCore : std::make_shared<StockCore>());
    m_props = (props ? props : std::make_shared<ChartProps>());
}

ChartModel::~ChartModel()
{
}

void ChartModel::setStockCore(const StockCore& stockCore)
{
    *m_stockCore = stockCore;

    for (auto& plugin : m_plugins)
        plugin->onStockCoreChanged();
}

void ChartModel::setProps(const ChartProps& props)
{
    *m_props = props;

    for (auto& plugin : m_plugins)
        plugin->onPropsChanged();
}
