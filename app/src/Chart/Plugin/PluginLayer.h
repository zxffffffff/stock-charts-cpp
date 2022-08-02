#pragma once
#include "../../Core/StockCore.h"
#include "../../Core/Graphics.h"
#include "../../Core/Painter.h"
#include "../Model/ChartProps.h"
#include "../Model/Area/ChartAreaContext.h"

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

    class ChartAreaContext;
    class PluginLayer
    {
    public:
        PluginLayer(std::shared_ptr<const StockCore> stockCore, std::shared_ptr<const ChartProps> props)
            : m_stockCore(stockCore)
            , m_props(props)
        {
        }
        virtual ~PluginLayer()
        {
        }

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
            return { NumberCore::EmptyNumber, NumberCore::EmptyNumber };
        }

        // [2]
        virtual void onCalcArea(std::shared_ptr<const ChartAreaContext> context) {}
        virtual void onPaintArea(Painter& painter) {}

        StChartAreaExp createStickExp(
            std::shared_ptr<const ChartAreaContext> context,
            const NumberCore& open, 
            const NumberCore& high, 
            const NumberCore& low, 
            const NumberCore& close
        );

        StChartAreaExp createLineExp(
            std::shared_ptr<const ChartAreaContext> context,
            const NumberCore& price
        );

    protected:
        std::shared_ptr<const StockCore> m_stockCore;
        std::shared_ptr<const ChartProps> m_props;

        std::vector<StChartAreaIndex> m_areaIndexs;
    };
}

