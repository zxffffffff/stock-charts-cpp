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
    enum class EnKLineType
    {
        CandleStickHollow,  // 蜡烛图（空心）
        CandleStick,        // 蜡烛图
        BAR,                // 美国线
        CloseLine,          // 收盘价线
    };

    enum class EnCoordinateType
    {
        Linear,             // 等差
        Proportional,       // 等比
        Percentage,         // 百分比
        LogLinear,          // 等差（对数）
        LogProportional,    // 等比（对数）
        LogPercentage,      // 百分比（对数）
    };

    enum class EnXAxisType
    {
        yyyyMMdd_HHmm,
        yyyyMMdd,
        yyyyMM,
        yyyy,
    };

    struct ChartProps
    {
        EnKLineType klineType = EnKLineType::CandleStick;

        EnCoordinateType coordinateType = EnCoordinateType::Linear;

        // border
        Real xAxisHeight = 30;
        Real ylAxisWidth = 50;
        Real yrAxisWidth = 50;

        // padding
        Real topPadding = 10;
        Real btmPadding = 10;
        Real leftPadding = 10;
        Real rightPadding = 10;

        // stick
        Real nodeWidth = 7;
        Real stickWidth = 5;

        // x-axis
        EnXAxisType xAxisType;
        EnXAxisType xAxisHoverType;

        // y-axis
    };

    struct ChartContext
    {
        ChartProps props;

        // [0] content
        Rect rectView;
        Rect rectXAxis;
        Rect rectYLAxis;
        Rect rectYRAxis;
        Rect rectChart;
        Rect rectInnerChart;

        // [1] x
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

