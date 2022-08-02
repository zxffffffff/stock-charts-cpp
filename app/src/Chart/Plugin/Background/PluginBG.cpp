/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "PluginBG.h"

using namespace StockCharts;

PluginBG::PluginBG(std::shared_ptr<const StockCore> stockCore, std::shared_ptr<const ChartProps> props)
    : PluginLayer(stockCore, props)
{
}

void PluginBG::onCalcArea(std::shared_ptr<const ChartAreaContext> context)
{
    auto& ctx = *context;

    rectView = ctx.rectView;
    rectXAxis = ctx.rectXAxis;
    rectYAxis = ctx.rectYAxis;
    rectChart = ctx.rectChart;
    rectInnerChart = ctx.rectInnerChart;
}

void PluginBG::onPaintArea(Painter& painter)
{
    painter.fillRect(rectView, Color(255, 255, 255, 255 * 0.2));
    painter.fillRect(rectXAxis, Color(150, 100, 100, 255 * 0.2));
    painter.fillRect(rectYAxis, Color(100, 150, 100, 255 * 0.2));
    painter.fillRect(rectChart, Color(100, 100, 150, 255 * 0.2));
    painter.fillRect(rectInnerChart, Color(100, 100, 200, 255 * 0.2));
}
