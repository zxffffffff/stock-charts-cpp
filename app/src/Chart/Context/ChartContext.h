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
    enum class MouseHoverType
    {
        Normal,
        SyncOther,      // 其他Chart联动
    };

    struct ChartContext
    {
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
        Real nodeWidth = 7;
        Real stickWidth = 5;

        // [2] y
        Number minPrice = NumberNull;
        Number maxPrice = NumberNull;

        // mouse & keyboard
        MouseHoverType hoverType = MouseHoverType::Normal;

        Point pointHover;
        int hoverIndex = -1;
        Number hoverPrice = NumberNull;

        bool crossLineVisible = true;
    };
}

