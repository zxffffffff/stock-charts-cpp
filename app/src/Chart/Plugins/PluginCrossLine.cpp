/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include "PluginCrossLine.h"
#include "../Context/ChartCoordinate.h"

using namespace StockCharts;

PluginCrossLine::PluginCrossLine(std::shared_ptr<const StockCore> stockCore)
    : ChartPlugin(stockCore)
{
}

void PluginCrossLine::onContextChanged(std::shared_ptr<const ChartContext> context)
{
    onMouseMove(context);
}

void PluginCrossLine::onMouseMove(std::shared_ptr<const ChartContext> context)
{
    const auto& ctx = *context;
    ChartCoordinate coordinate(context);

    Real x = coordinate.index2pos(ctx.hoverIndex);
    if (context->rectInnerChart.contains(context->pointHover)) {
        crossLine[0].set(
            ctx.rectChart.left(),
            ctx.pointHover.y,
            ctx.rectChart.right(),
            ctx.pointHover.y
        );
        crossLine[1].set(
            x,
            ctx.rectChart.top(),
            x,
            ctx.rectChart.bottom()
        );
    }
    else {
        crossLine = {};
    }
}

void PluginCrossLine::onPaint(std::shared_ptr<const ChartContext> context, Painter& painter)
{
    painter.drawLine(crossLine[0], Color("888888"));
    painter.drawLine(crossLine[1], Color("888888"));
}
