/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "ChartPlugin.h"
#include "../../Indicator/Core/IndexCore.h"
#include "../../Indicator/IndicatorParser.h"

namespace StockCharts
{
    class PluginIndicator : public ChartPlugin
    {
    public:
        PluginIndicator(std::shared_ptr<const StockCore> stockCore, std::shared_ptr<const ChartProps> props)
            : ChartPlugin(stockCore, props)
        {
        }
        virtual ~PluginIndicator() = default;

        // [0]
        virtual void onStockCoreChanged() override
        {
            calcIndicators();
        }

        // [1]
        virtual std::pair<Number, Number> getMinMax(int beginIndex, int endIndex) override
        {
            auto minmax = std::pair<Number, Number>{ NumberNull, NumberNull };
            for (auto& indicator : m_indicators) {
                for (auto& exp : indicator->indexCore.exps) {
                    auto minmax2 = exp.core.getMinMax(beginIndex, endIndex);
                    minmax.first = NumberCore::min(minmax.first, minmax2.first);
                    minmax.second = NumberCore::max(minmax.second, minmax2.second);
                }
            }
            return minmax;
        }

        // [2]
        virtual void onContextChanged(std::shared_ptr<const ChartContext> context) override
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
                    switch (indicatorExp.drawingType.type)
                    {
                    case EnDrawingType::None:
                    case EnDrawingType::Number:
                    case EnDrawingType::Text:
                        exp = createLineExp(
                            context,
                            indicatorExp.core
                        );
                        break;
                    case EnDrawingType::CandleStick:
                        // zxf todo
                        break;
                    }
                }
            }
        }

        virtual void onPaint(std::shared_ptr<const ChartContext> context, Painter& painter) override
        {
            for (auto& index : m_areaIndexs) {
                for (auto& exp : index.exps) {
                    switch (exp.type)
                    {
                    case EnChartAreaExpType::Stick:
                        painter.drawStick(exp.sticks, Color(200, 0, 0), Color(0, 200, 0));
                        break;
                    case EnChartAreaExpType::Line:
                        painter.drawPath(exp.lines, Color(100, 100, 200));
                        break;
                    }
                }
            }
        }

    public:
        std::shared_ptr<const StIndicator> addIndicator(IndexFormula formular)
        {
            std::shared_ptr<StIndicator> indicator = std::make_shared<StIndicator>();
            indicator->formula = formular;
            m_indicators.push_back(indicator);
            calcIndicator(m_indicators.size() - 1);

            fire(ID_ChartPluginChanged);
            return indicator;
        }

        void delIndicator(std::shared_ptr<const StIndicator> indicator)
        {
            auto ite = std::find(m_indicators.begin(), m_indicators.end(), indicator);
            if (ite == m_indicators.end())
                return;
            m_indicators.erase(ite);

            fire(ID_ChartPluginChanged);
        }
        void delIndicators()
        {
            m_indicators.clear();

            fire(ID_ChartPluginChanged);
        }

        std::vector<std::shared_ptr<const StIndicator>> getIndicators() const
        {
            return std::vector<std::shared_ptr<const StIndicator>>(m_indicators.begin(), m_indicators.end());
        }
        std::shared_ptr<const StIndicator> getIndicator(int i) const
        {
            if (i < 0 || i >= m_indicators.size())
                return std::make_shared<StIndicator>();
            return m_indicators[i];
        }

        void calcIndicators()
        {
            for (int i = 0; i < m_indicators.size(); i += 1)
                calcIndicator(i);
        }
        void calcIndicator(int i)
        {
            if (i < 0 || i >= m_indicators.size())
                return;
            auto& indicator = m_indicators[i];

            IndicatorParser parser;
            parser.setFormula(indicator->formula);
            parser.setStockCore(getStockCore());
            bool ok = parser.run();
            indicator->indexCore = parser.getResult();

            fire(ID_ChartPluginChanged);
        }

    private:
        std::vector<std::shared_ptr<StIndicator>> m_indicators;
    };
}

