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
        ctx.rectInnerChart.valid() && ctx.viewCount > 0 && ctx.nodeWidth > 0);
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
    switch (ctx.props.type)
    {
    case EnCoordinateYType::Linear:
    case EnCoordinateYType::Proportional:
    case EnCoordinateYType::Percentage:
        return ctx.rectInnerChart.bottom() - (price - ctx.minPrice) * (ctx.rectInnerChart.height() / (ctx.maxPrice - ctx.minPrice));
    case EnCoordinateYType::LogLinear:
    case EnCoordinateYType::LogProportional:
    case EnCoordinateYType::LogPercentage:
        return ctx.rectInnerChart.bottom() - (std::log(price) - ctx.minPrice) * (ctx.rectInnerChart.height() / (ctx.maxPrice - ctx.minPrice));
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
    switch (ctx.props.type)
    {
    case EnCoordinateYType::Linear:
    case EnCoordinateYType::Proportional:
    case EnCoordinateYType::Percentage:
        return ctx.minPrice + (ctx.rectInnerChart.bottom() - pos) / (ctx.rectInnerChart.height() / (ctx.maxPrice - ctx.minPrice));
    case EnCoordinateYType::LogLinear:
    case EnCoordinateYType::LogProportional:
    case EnCoordinateYType::LogPercentage:
        cache = (ctx.rectInnerChart.bottom() - pos) / (ctx.rectInnerChart.height() / (ctx.maxPrice - ctx.minPrice));
        cache = std::exp(cache + ctx.minPrice);
        cache -= std::exp(ctx.minPrice);
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
    return ctx.rectInnerChart.left() + (viewIndex * ctx.nodeWidth) + (ctx.nodeWidth / 2.0 - 0.5);
}

int ChartCoordinate::pos2index(Real pos) const
{
    if (!validX())
        return -1;

    const auto& ctx = *context;
    int viewIndex = std::round((pos - ctx.rectInnerChart.left() - (ctx.nodeWidth / 2.0 - 0.5)) / ctx.nodeWidth);
    return ctx.beginIndex + viewIndex;
}
