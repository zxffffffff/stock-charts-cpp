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

        // mouse event
        bool syncHover = false;
        Point pointHover;
        int hoverIndex = -1;
        Number hoverPrice = NumberNull;
    };
}

