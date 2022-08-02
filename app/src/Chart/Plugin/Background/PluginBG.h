/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include "../PluginLayer.h"

namespace StockCharts
{
    class PluginBG : public PluginLayer
    {
    public:
        PluginBG(std::shared_ptr<const StockCore> stockCore, std::shared_ptr<const ChartProps> props);

        // [2]
        virtual void onCalcArea(std::shared_ptr<const ChartAreaContext> context) override;
        virtual void onPaintArea(Painter& painter) override;

    private:
        Rect rectView;
        Rect rectXAxis;
        Rect rectYAxis;
        Rect rectChart;
        Rect rectInnerChart;
    };
}
