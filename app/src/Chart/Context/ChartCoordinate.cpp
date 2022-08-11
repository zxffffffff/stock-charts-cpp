/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include "ChartCoordinate.h"

using namespace StockCharts;

bool ChartCoordinate::validX() const
{
    const auto& ctx = *context;
    return (
        ctx.rectInnerChart.valid() && ctx.viewCount > 0 && ctx.props.nodeWidth > 0);
}

bool ChartCoordinate::validY() const
{
    const auto& ctx = *context;
    return (ctx.rectInnerChart.valid() && ctx.minPrice != NumberNull && ctx.maxPrice != NumberNull);
}

Real ChartCoordinate::price2pos(Number price) const
{
    if (!validY())
        return 0;

    const auto& ctx = *context;
    switch (ctx.props.coordinateType)
    {
    case EnCoordinateType::Linear:
    case EnCoordinateType::Proportional:
    case EnCoordinateType::Percentage:
        return ctx.rectInnerChart.bottom() - (price - ctx.minPrice) * (ctx.rectInnerChart.height() / (ctx.maxPrice - ctx.minPrice));
    case EnCoordinateType::LogLinear:
    case EnCoordinateType::LogProportional:
    case EnCoordinateType::LogPercentage:
        return ctx.rectInnerChart.bottom() - (std::log(price) - std::log(ctx.minPrice)) * (ctx.rectInnerChart.height() / (std::log(ctx.maxPrice) - std::log(ctx.minPrice)));
    default:
        return 0;
    }
}

Number ChartCoordinate::pos2price(Real pos) const
{
    if (!validY())
        return NumberNull;

    const auto& ctx = *context;

    Number cache;
    switch (ctx.props.coordinateType)
    {
    case EnCoordinateType::Linear:
    case EnCoordinateType::Proportional:
    case EnCoordinateType::Percentage:
        cache = (ctx.rectInnerChart.bottom() - pos) / (ctx.rectInnerChart.height() / (ctx.maxPrice - ctx.minPrice));
        return ctx.minPrice + cache;
    case EnCoordinateType::LogLinear:
    case EnCoordinateType::LogProportional:
    case EnCoordinateType::LogPercentage:
        cache = (ctx.rectInnerChart.bottom() - pos) / (ctx.rectInnerChart.height() / (std::log(ctx.maxPrice) - std::log(ctx.minPrice)));
        cache = std::exp(cache + std::log(ctx.minPrice));
        cache -= std::exp(std::log(ctx.minPrice));
        return ctx.minPrice + cache;
    default:
        return NumberNull;
    }
}

Real ChartCoordinate::index2pos(int index) const
{
    if (!validX())
        return 0;

    const auto& ctx = *context;
    int viewIndex = index - ctx.beginIndex;
    return ctx.rectInnerChart.left() + (viewIndex * ctx.props.nodeWidth) + (ctx.props.nodeWidth / 2.0 - 0.5);
}

int ChartCoordinate::pos2index(Real pos) const
{
    if (!validX())
        return -1;

    const auto& ctx = *context;
    int viewIndex = std::round((pos - ctx.rectInnerChart.left() - (ctx.props.nodeWidth / 2.0 - 0.5)) / ctx.props.nodeWidth);
    return ctx.beginIndex + viewIndex;
}
