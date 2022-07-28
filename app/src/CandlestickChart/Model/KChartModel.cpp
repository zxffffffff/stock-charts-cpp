#include "KChartModel.h"

using namespace StockCharts;

KChartModel::KChartModel()
{

}

void KChartModel::setStockCore(SpStockCore _stockCore)
{
    stockCore = _stockCore;
}

void KChartModel::addIndicator(SpIndicator _indicator)
{
    indicators.push_back(_indicator);
}

CpStockCore KChartModel::getStockCore() const
{
    return stockCore;
}

CpIndicator StockCharts::KChartModel::getIndicator(int i) const
{
    if (i < 0 || i >= indicators.size())
        return nullptr;
    return indicators[i];
}
