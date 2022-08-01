#include "ChartModel.h"
#include "../../Indicator/IndicatorParser.h"
#include "iostream"

using namespace StockCharts;

ChartModel::ChartModel(std::shared_ptr<StockCore> stockCore)
{
    setStockCore(stockCore);
}

void ChartModel::setStockCore(std::shared_ptr<StockCore> stockCore)
{
    m_stockCore = stockCore;
}

void StockCharts::ChartModel::addIndicator(IndexFormula formular)
{
    std::shared_ptr<StIndicator> indicator = std::make_shared<StIndicator>();
    indicator->formula = formular;
    m_indicators.push_back(indicator);
    calcIndicator(m_indicators.size() - 1);
}

void ChartModel::delIndicator(std::shared_ptr<const StIndicator> indicator)
{
    auto ite = std::find(m_indicators.begin(), m_indicators.end(), indicator);
    if (ite == m_indicators.end())
        return;
    m_indicators.erase(ite);
}

std::vector<std::shared_ptr<const StIndicator>> ChartModel::getIndicators() const
{
    return std::vector<std::shared_ptr<const StIndicator>>(m_indicators.begin(), m_indicators.end());
}

std::shared_ptr<const StockCore> ChartModel::getStockCore() const
{
    return m_stockCore;
}

std::shared_ptr<const StIndicator> ChartModel::getIndicator(int i) const
{
    if (i < 0 || i >= m_indicators.size())
        return nullptr;
    return m_indicators[i];
}

void StockCharts::ChartModel::calcIndicators()
{
    for (int i = 0; i < m_indicators.size(); i += 1)
        calcIndicator(i);
}

void StockCharts::ChartModel::calcIndicator(int i)
{
    if (i < 0 || i >= m_indicators.size())
        return;
    auto indicator = m_indicators[i];

    IndicatorParser parser;
    parser.setFormula(indicator->formula);
    parser.setStockCore(m_stockCore);
    bool ok = parser.run();
    indicator->indexCore = parser.getResult();

    // debug
    if (indicator->indexCore.err) {
        std::string tips = "error!\n\n";
        if (!indicator->indexCore.errExpression.empty())
            tips += "errExpression: " + indicator->indexCore.errExpression + "\n\n";
        if (!indicator->indexCore.errWord.empty())
            tips += "errWord: " + indicator->indexCore.errWord + "\n\n";
        std::cout << tips;
    }
}
