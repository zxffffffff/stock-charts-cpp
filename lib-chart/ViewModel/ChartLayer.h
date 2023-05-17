/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../Model/ChartModel.h"
#include "ChartProps.h"
#include "ChartContext.h"
#include "ChartCoordinate.h"

namespace StockCharts
{
    enum class EnChartAreaExpType
    {
        None,
        Stick,
        Line,
    };

    struct StChartAreaExp
    {
        EnChartAreaExpType type = EnChartAreaExpType::None;
        std::vector<Point> lines;
        std::vector<Stick> sticks;
        std::array<Color, 2> colors; // extends
    };

    struct StChartAreaIndex
    {
        std::vector<StChartAreaExp> exps;
    };

    constexpr inline char ID_ChartLayerChanged[] = "ID_ChartLayerChanged";

    class ChartLayer : public DataBinding
    {
    public:
        virtual void init(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context) = 0;

        // [1]
        virtual std::pair<Number, Number> getMinMax(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context)
        {
            return {NumberNull, NumberNull};
        }

        // [2]
        virtual void onContextChanged(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context) {}

        virtual void onMouseMove(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context) {}

        virtual void onMouseLeave(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context) {}

        virtual void onPaint(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context,
            Painter &painter) {}

        // [3]
        StChartAreaExp createStickExp(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context,
            const NumberCore &open,
            const NumberCore &high,
            const NumberCore &low,
            const NumberCore &close)
        {
            ChartCoordinate coordinate(props, context);

            StChartAreaExp exp;
            exp.type = EnChartAreaExpType::Stick;
            exp.sticks.resize(context.viewCount);
            for (int index = context.beginIndex; index < context.endIndex; index++)
            {
                int i = index - context.beginIndex;

                const Real xPos = coordinate.index2pos(index);
                const Number o = open[index];
                const Number c = close[index];
                const Real oPos = coordinate.price2pos(o);
                const Real hPos = coordinate.price2pos(high[index]);
                const Real lPos = coordinate.price2pos(low[index]);
                const Real cPos = coordinate.price2pos(c);

                exp.sticks[i].set(
                    xPos - context.stickWidth / 2,
                    std::min(oPos, cPos),
                    context.stickWidth,
                    std::abs(oPos - cPos),
                    hPos,
                    lPos,
                    (c - o));
            }
            return exp;
        }

        StChartAreaExp createLineExp(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context,
            const NumberCore &price)
        {
            ChartCoordinate coordinate(props, context);

            StChartAreaExp exp;
            exp.type = EnChartAreaExpType::Line;
            exp.lines.resize(context.viewCount);
            for (int index = context.beginIndex; index < context.endIndex; index++)
            {
                int i = index - context.beginIndex;

                const Real xPos = coordinate.index2pos(index);
                const Real yPos = coordinate.price2pos(price[index]);

                exp.lines[i].set(xPos, yPos);
            }
            return exp;
        }

    protected:
        // layers-indexs-exps
        std::vector<StChartAreaIndex> m_areaIndexs;
    };
}
