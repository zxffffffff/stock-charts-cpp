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
        std::vector<Real> xAxisPos;
        std::vector<Rect> xAxisRect;
        std::vector<std::string> xAxisDate;
        // y
        std::vector<Real> yAxisPos;
        std::vector<Rect> ylAxisRect;
        std::vector<Rect> yrAxisRect;
        std::vector<std::string> ylAxisPrice;
        std::vector<std::string> yrAxisPrice;
    };
}
