/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "PluginLayer.h"

using namespace StockCharts;

StChartAreaExp PluginLayer::createStickExp(
    std::shared_ptr<const ChartAreaContext> context,
    const NumberCore& open,
    const NumberCore& high,
    const NumberCore& low, 
    const NumberCore& close
)
{
    const auto& ctx = *context;

    StChartAreaExp exp;
    exp.type = EnChartAreaExpType::Stick;
    exp.sticks.resize(ctx.viewCount);
    for (int i = ctx.beginIndex; i < ctx.endIndex; i += 1) {
        const Real xPos = ctx.coordinate.index2pos(i);
        const Number o = open[i];
        const Number c = close[i];
        const Real oPos = ctx.coordinate.price2pos(o);
        const Real hPos = ctx.coordinate.price2pos(high[i]);
        const Real lPos = ctx.coordinate.price2pos(low[i]);
        const Real cPos = ctx.coordinate.price2pos(c);

        exp.sticks[i].set(
            xPos,
            std::min(oPos, cPos),
            m_props->stickWidth,
            std::abs(oPos - cPos),
            hPos,
            lPos,
            (c - o)
        );
    }
    return exp;
}

StChartAreaExp PluginLayer::createLineExp(
    std::shared_ptr<const ChartAreaContext> context, 
    const NumberCore& price
)
{
    const auto& ctx = *context;

    StChartAreaExp exp;
    exp.type = EnChartAreaExpType::Line;
    exp.lines.resize(ctx.viewCount);
    for (int i = ctx.beginIndex; i < ctx.endIndex; i += 1) {
        const Real xPos = ctx.coordinate.index2pos(i);
        const Real yPos = ctx.coordinate.price2pos(price[i]);

        exp.lines[i].set(xPos, yPos);
    }
    return exp;
}
