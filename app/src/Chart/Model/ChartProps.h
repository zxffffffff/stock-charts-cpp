#pragma once
#include "../../Core/StockCore.h"
#include "../../Core/Graphics.h"

namespace StockCharts
{
    enum class EnKChartDrawingType
    {
        StickLine,
        CloseLine,
    };

    struct ChartProps
    {
        Real xAxisHeight = 10; // timebar
        Real yAxisWidth = 50;
        Real topPadding = 10;
        Real btmPadding = 10;
        Real leftPadding = 10;
        Real rightPadding = 10;
        Real nodeWidth = 9; // odd
        Real stickWidth = 7; // odd
        EnKChartDrawingType klineType;
    };
}
