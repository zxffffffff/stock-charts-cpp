/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "ChartContext.h"

namespace StockCharts
{
    struct ChartCoordinate
    {
        const ChartContext& ctx;
        const ChartProps& props;

        ChartCoordinate(std::shared_ptr<const ChartContext> context, std::shared_ptr<const ChartProps> props)
            : ctx(*context)
            , props(*props)
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

            switch (props.coordinateType)
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

        Number pos2price(const Real pos) const
        {
            if (pos == RealNull || !validY())
                return NumberNull;

            Number cache;
            switch (props.coordinateType)
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

