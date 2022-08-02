#pragma once
#include "ChartCoordinate.h"

namespace StockCharts
{
    class ChartAreaContext
    {
    public:
        int beginIndex = 0;
        int endIndex = 0;
        int viewCount = endIndex - beginIndex;

        // [0]
        Rect rectView;
        Rect rectXAxis;
        Rect rectYAxis;
        Rect rectChart;
        Rect rectInnerChart;

        // [1]
        Number minPrice = NumberCore::EmptyNumber;
        Number maxPrice = NumberCore::EmptyNumber;

        // [2]
        ChartCoordinate coordinate;
    };
}

