/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include "ChartPlugin.h"

namespace StockCharts
{
    class PluginKLine : public ChartPlugin
    {
    public:
        PluginKLine(std::shared_ptr<const StockCore> stockCore);

        // [0]
        virtual void onStockCoreChanged() override;
        virtual void onPropsChanged() override {}

        // [1]
        virtual std::pair<Number, Number> getMinMax(int beginIndex, int endIndex) override;

        // [2]
        virtual void onContextChanged(std::shared_ptr<const ChartContext> context) override;
        virtual void onPaint(Painter& painter) override;
    };
}

