#pragma once
#include "../../Core/StockCore.h"
#include "../../TechnicalIndicators/Core/IndexCore.h"

namespace StockCharts
{
    struct StIndicator
    {
        IndexFormula formula;
        IndexCore indexCore;
    };
    using SpIndicator = std::shared_ptr<StIndicator>;
    using CpIndicator = std::shared_ptr<const StIndicator>;

    class KChartModel
    {
    public:
        KChartModel();

        void setStockCore(SpStockCore stockCore);
        void addIndicator(SpIndicator indicator);

        CpStockCore getStockCore() const;
        CpIndicator getIndicator(int i) const;

    private:
        SpStockCore stockCore;
        std::vector<SpIndicator> indicators;
    };
    using SpKChartModel = std::shared_ptr<KChartModel>;
}

