/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../../Core/StockCore.h"
#include "../../Core/DataBinding.h"
#include "../../Core/Utils.h"
#include "../Context/ChartContext.h"
#include "../Context/ChartCoordinate.h"

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

    class ChartPlugin : public DataBinding
    {
    public:
        ChartPlugin(std::shared_ptr<const StockCore> stockCore)
            : m_stockCore(stockCore)
        {
        }
        virtual ~ChartPlugin() = default;

        std::shared_ptr<const StockCore> getStockCore() const
        {
            return m_stockCore;
        }

    public:
        // [0]
        virtual void onStockCoreChanged() {}
        virtual void onPropsChanged() {}

        // [1]
        virtual std::pair<Number, Number> getMinMax(int beginIndex, int endIndex)
        {
            return { NumberNull, NumberNull };
        }

        // [2]
        virtual void onContextChanged(std::shared_ptr<const ChartContext> context) {}

        virtual void onMouseMove(std::shared_ptr<const ChartContext> context) {}

        virtual void onMouseLeave(std::shared_ptr<const ChartContext> context) {}

        virtual void onPaint(std::shared_ptr<const ChartContext> context, Painter& painter) {}

        StChartAreaExp createStickExp(
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

        StChartAreaExp createLineExp(
            std::shared_ptr<const ChartContext> context,
            const NumberCore& price
        ) {
            const auto& ctx = *context;
            ChartCoordinate coordinate(context);

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
        // [0] data
        std::shared_ptr<const StockCore> m_stockCore;

        // [1] layers-indexs-exps
        std::vector<StChartAreaIndex> m_areaIndexs;
    };
}

