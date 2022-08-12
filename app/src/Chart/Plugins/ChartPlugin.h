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
#include "../Context/ChartContext.h"

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
        ChartPlugin(std::shared_ptr<const StockCore> stockCore);
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
        );

        StChartAreaExp createLineExp(
            std::shared_ptr<const ChartContext> context,
            const NumberCore& price
        );

    protected:
        // [0] data
        std::shared_ptr<const StockCore> m_stockCore;

        // [1] layers-indexs-exps
        std::vector<StChartAreaIndex> m_areaIndexs;
    };
}

