/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "Graphics.h"
#include "Painter.h"
#include "DataBinding.h"

namespace StockCharts
{
    struct Button
    {
        std::string text;
        Rect rect;
        Color bg;
        Font font;

        Button(const std::string& _text, const Rect& _rect, const Color& _bg, const Font& _font)
            : text(_text)
            , rect(_rect)
            , bg(_bg)
            , font(_font)
        {
        }

        void setText(const std::string& _text)
        {
            text = _text;
        }

        void move(const Rect& _rect)
        {
            rect = _rect;
        }
    };
}

