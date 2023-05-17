/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../Core/NumberCore.h"
#include "../Graphics/Painter.h"

namespace StockCharts
{
    enum class EnStockLineType
    {
        CandlestickHollow, // 蜡烛图（空心）
        Candlestick,       // 蜡烛图
        BAR,               // 美国线
        Line,              // 收盘价线，分时线
    };

    enum class EnXCoordinateType
    {
        Fill,  // 填充
        Cover, // x滚动y缩放
    };

    enum class EnYCoordinateType
    {
        Linear,          // 等差
        Proportional,    // 等比
        Percentage,      // 百分比
        LogLinear,       // 等差（对数）
        LogProportional, // 等比（对数）
        LogPercentage,   // 百分比（对数）
    };

    enum class EnXAxisType
    {
        HHmm,
        yyyyMMdd_HHmm,
        yyyyMMdd,
        yyyyMM,
        yyyy,
    };

    struct ChartProps
    {
        EnStockLineType lineType = EnStockLineType::Candlestick;
        EnXCoordinateType xCoordType = EnXCoordinateType::Cover;
        EnYCoordinateType yCoordType = EnYCoordinateType::Linear;

        // margin
        Real MarginTop = 2;
        Real MarginBottom = 2;
        Real MarginLeft = 2;
        Real MarginRight = 2;

        // padding
        Real paddingTop = 10;
        Real paddingBottom = 10;
        Real paddingLeft = 2;
        Real paddingRight = 2;

        // content
        Color colorViewBG = Color(255, 255, 255, 255 * 0.2);
        Color colorXAxisBG = Color(150, 100, 100, 255 * 0.2);
        Color colorYLAxisBG = Color(100, 150, 100, 255 * 0.2);
        Color colorYRAxisBG = Color(100, 150, 100, 255 * 0.2);
        Color colorChartBG = Color(100, 100, 150, 255 * 0.2);
        Color colorInnerChartBG = Color(100, 100, 200, 255 * 0.2);
        Pen axisLineStyle = Color(0, 0, 0);
        Pen axisGridStyle = Color(150, 150, 150);
        Pen crossLineStyle = Pen(Color(100, 100, 100), 1, LineType::DashLine);
        Pen crossTextBGStyle = Color(200, 200, 200);
        Size crossTextBGSize = Size(80, 20);

        // x-axis
        EnXAxisType xAxisType = EnXAxisType::yyyyMM;
        EnXAxisType xAxisHoverType = EnXAxisType::yyyyMMdd;
        Real xAxisHeight = 20;
        Real xAxisTextWidth = 80;
        Font xAxisTextFont = Font(Color(0, 0, 0), 12, PaintDirection::Center);
        Font xAxisHoverTextFont = Font(Color(50, 0, 0), 12, PaintDirection::Center);

        // y-axis
        Real ylAxisWidth = 50;
        Real yrAxisWidth = 50;
        Real yAxisGridStepHeight = 26;
        Real yAxisGridStart = 26;
        Font ylAxisTextFont = Font(Color(0, 0, 0), 12, PaintDirection::CenterRight);
        Font yrAxisTextFont = Font(Color(0, 0, 0), 12, PaintDirection::CenterLeft);
        Font ylAxisHoverTextFont = Font(Color(50, 0, 0), 12, PaintDirection::CenterRight);
        Font yrAxisHoverTextFont = Font(Color(50, 0, 0), 12, PaintDirection::CenterLeft);

        // mouse & keyboard
        int scrollXStep = 1;
        int wheelYStep = 2;

        // stock
        int precision = 2;
        Color riseColor = Color(200, 0, 0);
        Color fallColor = Color(0, 200, 0);
    };
}
