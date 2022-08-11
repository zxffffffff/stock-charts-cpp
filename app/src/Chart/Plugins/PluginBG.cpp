/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "PluginBG.h"
#include "../Context/ChartCoordinate.h"

using namespace StockCharts;

PluginBG::PluginBG(std::shared_ptr<const StockCore> stockCore)
    : ChartPlugin(stockCore)
{
}

void PluginBG::onContextChanged(std::shared_ptr<const ChartContext> context)
{
    auto& ctx = *context;
    ChartCoordinate coordinate(context);

    // x
    xAxis.resize(ctx.viewCount);
    for (int i = 0; i < ctx.viewCount; i++) {
        int index = ctx.beginIndex + i;
        xAxis[i] = coordinate.index2pos(index);
    }

    //y
    auto minmax = m_stockCore->getMinMax(ctx.beginIndex, ctx.endIndex);
    
}

void PluginBG::onPaint(std::shared_ptr<const ChartContext> context, Painter& painter)
{
    auto& ctx = *context;

    painter.fillRect(ctx.rectView, Color(255, 255, 255, 255 * 0.2));
    painter.fillRect(ctx.rectChart, Color(100, 100, 150, 255 * 0.2));
    painter.fillRect(ctx.rectInnerChart, Color(100, 100, 200, 255 * 0.2));

    // x
    painter.fillRect(ctx.rectXAxis, Color(150, 100, 100, 255 * 0.2));

    // y
    painter.fillRect(ctx.rectYLAxis, Color(100, 150, 100, 255 * 0.2));
    painter.fillRect(ctx.rectYRAxis, Color(100, 150, 100, 255 * 0.2));
}
