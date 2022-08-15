/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../../Core/NumberCore.h"
#include "../../Core/Painter.h"

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
        Real xAxisHeight = 20;
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

        // axis
        Pen axisLineStyle = Color(0, 0, 0);
        Pen axisGridStyle = Color(150, 150, 150);
        Pen crossLineStyle = Pen(Color(100, 100, 100), 1, LineType::DashLine);
        Pen crossTextBGStyle = Color(200, 200, 200);
        Size crossTextBGSize = Size(80, 20);

        // x-axis
        EnXAxisType xAxisType = EnXAxisType::yyyyMM;
        EnXAxisType xAxisHoverType = EnXAxisType::yyyyMMdd;
        Real xAxisTextWidth = 80;
        Font xAxisTextFont = Font(Color(0, 0, 0), 12, PaintDirection::Center);
        Font xAxisHoverTextFont = Font(Color(50, 0, 0), 12, PaintDirection::Center);

        // y-axis
        Real yAxisGridStepHeight = 26;
        Real yAxisGridStart = 26;
        Font ylAxisTextFont = Font(Color(0, 0, 0), 12, PaintDirection::CenterRight);
        Font yrAxisTextFont = Font(Color(0, 0, 0), 12, PaintDirection::CenterLeft);
        Font ylAxisHoverTextFont = Font(Color(50, 0, 0), 12, PaintDirection::CenterRight);
        Font yrAxisHoverTextFont = Font(Color(50, 0, 0), 12, PaintDirection::CenterLeft);

        // stock
        int precision = 2;

        ChartProps() = default;
        ChartProps(const ChartProps&) = default;
        ChartProps(ChartProps&&) = default;
        ChartProps& operator=(const ChartProps&) = default;
        ChartProps& operator=(ChartProps&&) = default;
        ~ChartProps() = default;

        bool operator==(const ChartProps& rhs) const
        {
            return (
                klineType == rhs.klineType &&
                coordinateType == rhs.coordinateType &&
                xAxisHeight == rhs.xAxisHeight &&
                ylAxisWidth == rhs.ylAxisWidth &&
                yrAxisWidth == rhs.yrAxisWidth &&
                topPadding == rhs.topPadding &&
                btmPadding == rhs.btmPadding &&
                leftPadding == rhs.leftPadding &&
                rightPadding == rhs.rightPadding &&
                nodeWidth == rhs.nodeWidth &&
                stickWidth == rhs.stickWidth &&
                axisLineStyle == rhs.axisLineStyle &&
                axisGridStyle == rhs.axisGridStyle &&
                crossLineStyle == rhs.crossLineStyle &&
                crossTextBGStyle == rhs.crossTextBGStyle &&
                crossTextBGSize == rhs.crossTextBGSize &&
                xAxisType == rhs.xAxisType &&
                xAxisHoverType == rhs.xAxisHoverType &&
                xAxisTextWidth == rhs.xAxisTextWidth &&
                xAxisTextFont == rhs.xAxisTextFont &&
                xAxisHoverTextFont == rhs.xAxisHoverTextFont &&
                yAxisGridStepHeight == rhs.yAxisGridStepHeight &&
                yAxisGridStart == rhs.yAxisGridStart &&
                ylAxisTextFont == rhs.ylAxisTextFont &&
                yrAxisTextFont == rhs.yrAxisTextFont &&
                ylAxisHoverTextFont == rhs.ylAxisHoverTextFont &&
                yrAxisHoverTextFont == rhs.yrAxisHoverTextFont &&
                precision == rhs.precision);
        }
        bool operator!=(const ChartProps& rhs) const
        {
            return !operator==(rhs);
        }
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
        int beginIndex = 0; // = (endIndex - viewCount)

        // [2] y
        Number minPrice = NumberNull;
        Number maxPrice = NumberNull;

        // mouse event
        bool syncHover = false;
        Point pointHover;
        int hoverIndex = -1;
        Number hoverPrice = NumberNull;

        ChartContext() = default;
        ChartContext(const ChartContext&) = default;
        ChartContext(ChartContext&&) = default;
        ChartContext& operator=(const ChartContext&) = default;
        ChartContext& operator=(ChartContext&&) = default;
        ~ChartContext() = default;

        bool operator==(const ChartContext& rhs) const
        {
            return (
                props == rhs.props &&
                rectView == rhs.rectView &&
                rectXAxis == rhs.rectXAxis &&
                rectYLAxis == rhs.rectYLAxis &&
                rectYRAxis == rhs.rectYRAxis &&
                rectChart == rhs.rectChart &&
                rectInnerChart == rhs.rectInnerChart &&
                viewCount == rhs.viewCount &&
                endIndex == rhs.endIndex &&
                beginIndex == rhs.beginIndex &&
                minPrice == rhs.minPrice &&
                maxPrice == rhs.maxPrice &&
                syncHover == rhs.syncHover &&
                pointHover == rhs.pointHover &&
                hoverIndex == rhs.hoverIndex &&
                hoverPrice == rhs.hoverPrice);
        }
        bool operator!=(const ChartContext& rhs) const
        {
            return !operator==(rhs);
        }
    };
}

