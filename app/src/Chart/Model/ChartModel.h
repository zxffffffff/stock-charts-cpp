#pragma once
#include "../../Core/StockCore.h"
#include "../../Indicator/Core/IndexCore.h"

namespace StockCharts
{
    class ChartModel
    {
    public:
        ChartModel(std::shared_ptr<StockCore> stockCore = nullptr);

        void setStockCore(std::shared_ptr<StockCore> stockCore);
        std::shared_ptr<const StockCore> getStockCore() const;

        void addIndicator(IndexFormula formular);
        void delIndicator(std::shared_ptr<const StIndicator> indicator);
        std::vector<std::shared_ptr<const StIndicator>> getIndicators() const;
        std::shared_ptr<const StIndicator> getIndicator(int i) const;
        void calcIndicators();
        void calcIndicator(int i);

    private:
        std::shared_ptr<StockCore> m_stockCore;
        std::vector<std::shared_ptr<StIndicator>> m_indicators;
    };
}

