/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "ChartProps.h"
#include "ChartContext.h"

namespace StockCharts
{
    struct ChartCoordinate
    {
        const ChartProps &props;
        const ChartContext &ctx;

        ChartCoordinate(const ChartProps &props, const ChartContext &context)
            : props(props), ctx(context)
        {
        }

        bool validX() const
        {
            return (
                ctx.rectInnerChart.valid() && ctx.viewCount > 0 && ctx.nodeWidth > 0);
        }

        bool validY() const
        {
            return (ctx.rectInnerChart.valid() && ctx.minPrice != NumberNull && ctx.maxPrice != NumberNull);
        }

        Real price2pos(const Number price) const
        {
            if (price == NumberNull || !validY())
                return RealNull;

            switch (props.yCoordType)
            {
            case EnYCoordinateType::Linear:
            case EnYCoordinateType::Proportional:
            case EnYCoordinateType::Percentage:
                return ctx.rectInnerChart.bottom() - (price - ctx.minPrice) * (ctx.rectInnerChart.height() / (ctx.maxPrice - ctx.minPrice));
            case EnYCoordinateType::LogLinear:
            case EnYCoordinateType::LogProportional:
            case EnYCoordinateType::LogPercentage:
                return ctx.rectInnerChart.bottom() - (std::log(price) - std::log(ctx.minPrice)) * (ctx.rectInnerChart.height() / (std::log(ctx.maxPrice) - std::log(ctx.minPrice)));
            default:
                return 0;
            }
        }

        Number pos2price(const Real pos) const
        {
            if (pos == RealNull || !validY())
                return NumberNull;

            Number cache;
            switch (props.yCoordType)
            {
            case EnYCoordinateType::Linear:
            case EnYCoordinateType::Proportional:
            case EnYCoordinateType::Percentage:
                cache = (ctx.rectInnerChart.bottom() - pos) / (ctx.rectInnerChart.height() / (ctx.maxPrice - ctx.minPrice));
                return ctx.minPrice + cache;
            case EnYCoordinateType::LogLinear:
            case EnYCoordinateType::LogProportional:
            case EnYCoordinateType::LogPercentage:
                cache = (ctx.rectInnerChart.bottom() - pos) / (ctx.rectInnerChart.height() / (std::log(ctx.maxPrice) - std::log(ctx.minPrice)));
                cache = std::exp(cache + std::log(ctx.minPrice));
                cache -= std::exp(std::log(ctx.minPrice));
                return ctx.minPrice + cache;
            default:
                return NumberNull;
            }
        }

        Real index2pos(const int index) const
        {
            if (index < 0 || !validX())
                return RealNull;

            int viewIndex = index - ctx.beginIndex;
            return ctx.rectInnerChart.left() + (viewIndex * ctx.nodeWidth) + (ctx.nodeWidth / 2.0 - 0.5);
        }

        int pos2index(const Real pos) const
        {
            if (pos == RealNull || !validX())
                return -1;

            int viewIndex = std::round((pos - ctx.rectInnerChart.left() - (ctx.nodeWidth / 2.0 - 0.5)) / ctx.nodeWidth);
            return ctx.beginIndex + viewIndex;
        }
    };
}
