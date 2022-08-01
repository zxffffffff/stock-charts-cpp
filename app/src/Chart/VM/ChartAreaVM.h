#pragma once
#include "../../Core/StockCore.h"
#include "../../Core/Graphics.h"
#include "ChartContext.h"

namespace StockCharts
{
    class ChartModel;
    class ChartAreaVM
    {
    public:
        ChartAreaVM(std::shared_ptr<ChartModel> model);

        // [0]
        void setMain(bool b);

        // [1]
        void calcContext(Rect rectView, bool force = false);

        std::vector<Point>   calcLine(const NumberCore& core);
        std::vector<Stick>   calcStickLine(std::shared_ptr<const StockCore> stockCore);

        Real getXPos(int index);
        Real getYPos(Number price);

        // [2]
        std::shared_ptr<ChartContext> getContext() { return m_context; }
        std::shared_ptr<const ChartContext> getContext() const { return m_context; }

    private:
        std::shared_ptr<ChartModel> m_model;
        std::shared_ptr<ChartContext> m_context;
    };
}

