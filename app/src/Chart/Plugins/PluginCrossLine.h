/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "ChartPlugin.h"
#include <array>

namespace StockCharts
{
    class PluginCrossLine : public ChartPlugin
    {
    public:
        PluginCrossLine(std::shared_ptr<const StockCore> stockCore);

        // [2]
        virtual void onContextChanged(std::shared_ptr<const ChartContext> context) override;
        virtual void onMouseMove(std::shared_ptr<const ChartContext> context) override;
        virtual void onPaint(std::shared_ptr<const ChartContext> context, Painter& painter) override;

    private:
        std::array<Line, 2> crossLine;
    };
}

