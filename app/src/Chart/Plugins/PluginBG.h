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
        virtual void onPaint(std::shared_ptr<const ChartContext> context, Painter& painter) override;

    private:
        // x
        std::vector<Real> xAxis;
        // y
        std::vector<Real> lyAxis;
        std::vector<Real> ryAxis;
    };
}
