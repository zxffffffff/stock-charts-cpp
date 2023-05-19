/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../../../../Core/NumberCore.h"

namespace StockCharts
{
    struct ExpInfo
    {
        std::string rename;
        bool renameAssign = false;
    };

    enum class EnExpLineType
    {
        None,
        LINE,
        DOTLINE,
        STICK,
        COLORSTICK,
    };

    enum class EnExpLineThick : int
    {
        None = 0,
        LINETHICK1,
        LINETHICK2,
        LINETHICK3,
        LINETHICK4,
        LINETHICK5,
        LINETHICK6,
        LINETHICK7,
        LINETHICK8,
        LINETHICK9,
    };

    struct ExpColorType
    {
        EnExpLineType type = EnExpLineType::None;
        EnExpLineThick thick = EnExpLineThick::None;
        std::string color;
    };

    enum class EnDrawingType
    {
        None, // Line
        Number,
        Text,
        Candlestick,
    };

    struct ExpDrawingType
    {
        EnDrawingType type = EnDrawingType::None;
        Number stickWidth = 0.0;
        Number stickEmpty = 0.0;
    };

    struct ExpCore
    {
        NumberCore core;
        ExpInfo info;
        ExpColorType colorType;
        ExpDrawingType drawingType;
    };
}
