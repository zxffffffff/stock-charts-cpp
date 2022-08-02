/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "PluginIndicator.h"
#include "../../Chart/Model/Area/ChartAreaContext.h"
#include "../../Indicator/Parser/IndicatorParser.h"

using namespace StockCharts;

PluginIndicator::PluginIndicator(std::shared_ptr<const StockCore> stockCore, std::shared_ptr<const ChartProps> props)
    : PluginLayer(stockCore, props)
{
}

std::pair<Number, Number> PluginIndicator::getMinMax(int beginIndex, int endIndex)
{
    auto minmax = std::pair<Number, Number>{ NumberCore::EmptyNumber, NumberCore::EmptyNumber };
    for (auto& indicator : m_indicators) {
        for (auto& exp : indicator->indexCore.exps) {
            auto minmax2 = exp.core.getMinMax(beginIndex, endIndex);
            minmax.first = NumberCore::min(minmax.first, minmax2.first);
            minmax.second = NumberCore::max(minmax.second, minmax2.second);
        }
    }
    return minmax;
}

void PluginIndicator::onCalcArea(std::shared_ptr<const ChartAreaContext> context)
{
    auto& ctx = *context;

    const int indexCnt = m_indicators.size();
    m_areaIndexs.resize(indexCnt);
    for (int i = 0; i < indexCnt; i += 1) {
        auto& areaExps = m_areaIndexs[i].exps;
        const auto& indicator = m_indicators[i];
        const int expCnt = indicator->indexCore.exps.size();
        areaExps.resize(expCnt);
        for (int j = 0; j < expCnt; j += 1) {
            auto& exp = areaExps[j];
            auto& indicatorExp = indicator->indexCore.exps[j];
            switch (indicatorExp.drawing.type)
            {
            case EnDrawingType::None:
            case EnDrawingType::Number:
            case EnDrawingType::Text:
                exp = createLineExp(
                    context,
                    indicatorExp.core
                );
                break;
            case EnDrawingType::StickLine:
                // todo
                break;
            }
        }
    }
}

void PluginIndicator::onPaintArea(Painter& painter)
{
    for (auto& index : m_areaIndexs) {
        for (auto& exp : index.exps) {
            switch (exp.type)
            {
            case EnChartAreaExpType::Stick:
                painter.drawStick(exp.sticks, Color(200, 0, 0), Color(0, 200, 0));
                break;
            case EnChartAreaExpType::Line:
                painter.drawLines(exp.lines, Color(100, 100, 200));
                break;
            }
        }
    }
}

void PluginIndicator::onStockCoreChanged()
{
    calcIndicators();
}

std::shared_ptr<const StIndicator> PluginIndicator::addIndicator(IndexFormula formular)
{
    std::shared_ptr<StIndicator> indicator = std::make_shared<StIndicator>();
    indicator->formula = formular;
    m_indicators.push_back(indicator);
    calcIndicator(m_indicators.size() - 1);
    return indicator;
}

void PluginIndicator::delIndicator(std::shared_ptr<const StIndicator> indicator)
{
    auto ite = std::find(m_indicators.begin(), m_indicators.end(), indicator);
    if (ite == m_indicators.end())
        return;
    m_indicators.erase(ite);
}

void PluginIndicator::delIndicators()
{
    m_indicators.clear();
}

std::vector<std::shared_ptr<const StIndicator>> PluginIndicator::getIndicators() const
{
    return std::vector<std::shared_ptr<const StIndicator>>(m_indicators.begin(), m_indicators.end());
}

std::shared_ptr<const StIndicator> PluginIndicator::getIndicator(int i) const
{
    if (i < 0 || i >= m_indicators.size())
        return std::make_shared<StIndicator>();
    return m_indicators[i];
}

void PluginIndicator::calcIndicators()
{
    for (int i = 0; i < m_indicators.size(); i += 1)
        calcIndicator(i);
}

void PluginIndicator::calcIndicator(int i)
{
    if (i < 0 || i >= m_indicators.size())
        return;
    auto& indicator = m_indicators[i];

    IndicatorParser parser;
    parser.setFormula(indicator->formula);
    parser.setStockCore(getStockCore());
    bool ok = parser.run();
    indicator->indexCore = parser.getResult();
}
