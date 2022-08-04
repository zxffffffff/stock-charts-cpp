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
    class PluginBG : public ChartPlugin
    {
    public:
        PluginBG(std::shared_ptr<const StockCore> stockCore);

        // [2]
        virtual void onContextChanged(std::shared_ptr<const ChartContext> context) override;
        virtual void onPaint(Painter& painter) override;

    private:
        Rect rectView;
        Rect rectXAxis;
        Rect rectYAxis;
        Rect rectChart;
        Rect rectInnerChart;
    };
}
