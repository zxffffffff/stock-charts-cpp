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

namespace StockCharts
{
    class PluginIndicator : public ChartPlugin
    {
    public:
        PluginIndicator(std::shared_ptr<const StockCore> stockCore);

        // [0]
        virtual void onStockCoreChanged() override;
        virtual void onPropsChanged() override {}

        // [1]
        virtual std::pair<Number, Number> getMinMax(int beginIndex, int endIndex) override;

        // [2]
        virtual void onContextChanged(std::shared_ptr<const ChartContext> context) override;
        virtual void onPaint(std::shared_ptr<const ChartContext> context, Painter& painter) override;

    public:
        std::shared_ptr<const StIndicator> addIndicator(IndexFormula formular);

        void delIndicator(std::shared_ptr<const StIndicator> indicator);
        void delIndicators();

        std::vector<std::shared_ptr<const StIndicator>> getIndicators() const;
        std::shared_ptr<const StIndicator> getIndicator(int i) const;

        void calcIndicators();
        void calcIndicator(int i);

    private:
        std::vector<std::shared_ptr<StIndicator>> m_indicators;
    };
}

