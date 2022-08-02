/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include "../../Chart/Plugin/PluginLayer.h"
#include "../../Indicator/Core/IndexCore.h"

namespace StockCharts
{
    class PluginIndicator : public PluginLayer
    {
    public:
        PluginIndicator(std::shared_ptr<const StockCore> stockCore, std::shared_ptr<const ChartProps> props);

        // [0]
        virtual void onStockCoreChanged() override;
        virtual void onPropsChanged() override {}

        // [1]
        virtual std::pair<Number, Number> getMinMax(int beginIndex, int endIndex) override;

        // [2]
        virtual void onCalcArea(std::shared_ptr<const ChartAreaContext> context) override;
        virtual void onPaintArea(Painter& painter) override;

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

