#pragma once
#include "../../Core/StockCore.h"
#include "../../Core/Graphics.h"
#include "ChartCoordinate.h"

namespace StockCharts
{
    struct StChartDrawing
    {
        std::vector<Point> lines;
        std::vector<Stick> sticks;
    };

    enum class EnKChartDrawingType
    {
        StickLine,
        CloseLine,
    };

    struct KLine
    {
        StChartDrawing buffer;
    };
    struct Exp
    {
        StChartDrawing buffer;
    };
    struct Index
    {
        std::vector<Exp> exps;
    };

    class ChartContext
    {
    public:
        bool main = true;

        // [0]
        struct {
            Real xAxisHeight = 10; // timebar
            Real yAxisWidth = 50;
            Real topPadding = 10;
            Real btmPadding = 10;
            Real leftPadding = 10;
            Real rightPadding = 10;
            Real nodeWidth = 9; // odd
            Real stickWidth = 7; // odd
            EnKChartDrawingType klineType;
        } config;

        Rect rectView;

        // [1]
        struct {
            int cnt = 0;
            Number min = 0.0;
            Number max = 0.0;
        } model;

        // [2]
        struct {
            Rect rectXAxis;
            Rect rectYAxis;
            Rect rectChart;
            Rect rectInnerChart;
        } calc;

        ChartCoordinate yCoordinate;

        // [3]
        KLine kline;
        std::vector<Index> indexs;
    };
}

