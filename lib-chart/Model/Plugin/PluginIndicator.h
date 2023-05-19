/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../ChartPlugin.h"
#include "Indicator/IndicatorParser.h"

namespace StockCharts
{
    class PluginIndicator : public ChartPlugin
    {
    private:
        std::weak_ptr<const StockCore> m_stockCore;
        std::vector<std::shared_ptr<StIndicator>> m_indicators;

    public:
        virtual void init(std::shared_ptr<const StockCore> stockCore) override
        {
            m_stockCore = stockCore;
        }

        virtual void onStockCoreChanged(std::shared_ptr<const StockCore> stockCore) override
        {
            m_stockCore = stockCore;
            calcIndicators();
        }

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
            auto &indicator = m_indicators[i];

            IndicatorParser parser;
            parser.setFormula(indicator->formula);
            parser.setStockCore(m_stockCore.lock());
            bool ok = parser.run();
            indicator->indexCore = parser.getResult();

            fire(ID_ChartPluginChanged);
        }
    };
}
