#pragma once
#include "../../Core/StockCore.h"
#include "../../Core/Painter.h"

namespace StockCharts
{
    class ChartAreaVM;
    class ChartAreaView
    {
    public:
        ChartAreaView(std::shared_ptr<ChartAreaVM> vm = nullptr);

        void setVM(std::shared_ptr<ChartAreaVM> vm);

        void OnResize(Rect rect);
        void OnPaint(Painter& painter);

    private:
        std::shared_ptr<ChartAreaVM> m_vm;
    };
}
