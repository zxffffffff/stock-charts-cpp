#pragma once
#include "../PluginLayer.h"

namespace StockCharts
{
    enum class EnKLineType
    {
        CandleStick,
        CloseLine,
    };

    struct StKLineConfig
    {
        EnKLineType klineType = EnKLineType::CandleStick;
    };

    class PluginKLine : public PluginLayer
    {
    public:
        PluginKLine(std::shared_ptr<const StockCore> stockCore, std::shared_ptr<const ChartProps> props);

        // [0]
        virtual void onStockCoreChanged() override;
        virtual void onPropsChanged() override {}

        // [1]
        virtual std::pair<Number, Number> getMinMax(int beginIndex, int endIndex) override;

        // [2]
        virtual void onCalcArea(std::shared_ptr<const ChartAreaContext> context) override;
        virtual void onPaintArea(Painter& painter) override;

    public:
        void setKLineType(EnKLineType type);
        EnKLineType getKLineType() const
        {
            return m_config.klineType;
        }

    private:
        StKLineConfig m_config;
    };
}

