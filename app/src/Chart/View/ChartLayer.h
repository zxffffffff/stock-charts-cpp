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
    };

    struct StChartAreaIndex
    {
        std::vector<StChartAreaExp> exps;
    };

    constexpr inline char ID_ChartLayerChanged[] = "ID_ChartLayerChanged";

    class ChartLayer : public DataBinding
    {
    public:
        ChartLayer(std::shared_ptr<const ChartModel> model, std::shared_ptr<const ChartProps> props, std::shared_ptr<const ChartContext> context)
            : m_model(model)
            , m_props(props)
            , m_context(context)
        {
        }
        virtual ~ChartLayer() = default;

    public:
        // [1]
        virtual std::pair<Number, Number> getMinMax(int beginIndex, int endIndex)
        {
            return { NumberNull, NumberNull };
        }

        // [2]
        virtual void onContextChanged() {}

        virtual void onMouseMove() {}

        virtual void onMouseLeave() {}

        virtual void onPaint(Painter& painter) {}

        StChartAreaExp createStickExp(const NumberCore& open, const NumberCore& high, const NumberCore& low, const NumberCore& close)
        {
            const auto& ctx = *m_context;
            const auto& props = *m_props;
            ChartCoordinate coordinate(m_props, m_context);

            StChartAreaExp exp;
            exp.type = EnChartAreaExpType::Stick;
            exp.sticks.resize(ctx.viewCount);
            for (int index = ctx.beginIndex; index < ctx.endIndex; index++) {
                int i = index - ctx.beginIndex;

                const Real xPos = coordinate.index2pos(index);
                const Number o = open[index];
                const Number c = close[index];
                const Real oPos = coordinate.price2pos(o);
                const Real hPos = coordinate.price2pos(high[index]);
                const Real lPos = coordinate.price2pos(low[index]);
                const Real cPos = coordinate.price2pos(c);

                exp.sticks[i].set(
                    xPos - ctx.stickWidth / 2,
                    std::min(oPos, cPos),
                    ctx.stickWidth,
                    std::abs(oPos - cPos),
                    hPos,
                    lPos,
                    (c - o)
                );
            }
            return exp;
        }

        StChartAreaExp createLineExp(const NumberCore& price)
        {
            const auto& ctx = *m_context;
            ChartCoordinate coordinate(m_props, m_context);

            StChartAreaExp exp;
            exp.type = EnChartAreaExpType::Line;
            exp.lines.resize(ctx.viewCount);
            for (int index = ctx.beginIndex; index < ctx.endIndex; index++) {
                int i = index - ctx.beginIndex;

                const Real xPos = coordinate.index2pos(index);
                const Real yPos = coordinate.price2pos(price[index]);

                exp.lines[i].set(xPos, yPos);
            }
            return exp;
        }

    protected:
        // [0]
        std::shared_ptr<const ChartModel> m_model;
        std::shared_ptr<const ChartProps> m_props;
        std::shared_ptr<const ChartContext> m_context;

        // [1] layers-indexs-exps
        std::vector<StChartAreaIndex> m_areaIndexs;
    };
}

