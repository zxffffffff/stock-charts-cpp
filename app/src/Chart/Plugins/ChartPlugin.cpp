/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "ChartPlugin.h"
#include "../Context/ChartCoordinate.h"

using namespace StockCharts;

ChartPlugin::ChartPlugin(std::shared_ptr<const StockCore> stockCore)
    : m_stockCore(stockCore)
{
}

StChartAreaExp ChartPlugin::createStickExp(
    std::shared_ptr<const ChartContext> context,
    const NumberCore& open,
    const NumberCore& high,
    const NumberCore& low, 
    const NumberCore& close
) {
    const auto& ctx = *context;
    ChartCoordinate coordinate(context);

    StChartAreaExp exp;
    exp.type = EnChartAreaExpType::Stick;
    exp.sticks.resize(ctx.viewCount);
    for (int i = 0; i < ctx.viewCount; i++) {
        int index = ctx.beginIndex + i;

        const Real xPos = coordinate.index2pos(index);
        const Number o = open[index];
        const Number c = close[index];
        const Real oPos = coordinate.price2pos(o);
        const Real hPos = coordinate.price2pos(high[index]);
        const Real lPos = coordinate.price2pos(low[index]);
        const Real cPos = coordinate.price2pos(c);

        exp.sticks[i].set(
            xPos - ctx.props.stickWidth / 2,
            std::min(oPos, cPos),
            ctx.props.stickWidth,
            std::abs(oPos - cPos),
            hPos,
            lPos,
            (c - o)
        );
    }
    return exp;
}

StChartAreaExp ChartPlugin::createLineExp(std::shared_ptr<const ChartContext> context, const NumberCore& price
)
{
    const auto& ctx = *context;
    ChartCoordinate coordinate(context);

    StChartAreaExp exp;
    exp.type = EnChartAreaExpType::Line;
    exp.lines.resize(ctx.viewCount);
    for (int i = 0; i < ctx.viewCount; i++) {
        int index = ctx.beginIndex + i;

        const Real xPos = coordinate.index2pos(index);
        const Real yPos = coordinate.price2pos(price[index]);

        exp.lines[i].set(xPos, yPos);
    }
    return exp;
}
