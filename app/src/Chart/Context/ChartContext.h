/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include "../../Core/NumberCore.h"
#include "../../Core/Graphics.h"

namespace StockCharts
{
    enum class EnKChartDrawingType
    {
        StickLine,          // 蜡烛图
        CloseLine,          // 收盘价线
    };

    enum class EnCoordinateYType
    {
        Linear,             // 等差
        Proportional,       // 等比
        Percentage,         // 百分比
        LogLinear,          // 等差（对数）
        LogProportional,    // 等比（对数）
        LogPercentage,      // 百分比（对数）
    };

    struct ChartProps
    {
        EnKChartDrawingType klineType = EnKChartDrawingType::StickLine;

        EnCoordinateYType type = EnCoordinateYType::Linear;

        // border
        Real xAxisHeight = 10; // timebar
        Real yAxisWidth = 50;

        // padding
        Real topPadding = 10;
        Real btmPadding = 10;
        Real leftPadding = 10;
        Real rightPadding = 10;

        // stick
        Real initNodeWidth = 7;
        Real initStickWidth = 5;
    };

    struct ChartContext
    {
        ChartProps props;

        // [0] content
        Rect rectView;
        Rect rectXAxis;
        Rect rectYAxis;
        Rect rectChart;
        Rect rectInnerChart;

        // [1] x
        Real nodeWidth = props.initNodeWidth;
        Real stickWidth = props.initStickWidth;
        int viewCount = 0;
        int endIndex = 0; // lastIndex = (endIndex - 1)
        int beginIndex = endIndex - viewCount;

        // [2] y
        Number minPrice = NumberNull;
        Number maxPrice = NumberNull;

        // mouse event
        Point pointHover;
        int hoverIndex = -1;
        Number hoverPrice = NumberNull;
    };
}

