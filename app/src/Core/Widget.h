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
#include <string>

namespace StockCharts
{
    struct WidgetBGStyle
    {
        Color colorBG;
        Color colorBorder;

        std::string image;
    };

    struct WidgetBG
    {
        WidgetBGStyle normal;
        WidgetBGStyle hover;
        WidgetBGStyle press;
        WidgetBGStyle disable;
    };

    enum class WidgetTextStyle
    {
        Left,
        Center,
        Right,
    };

    struct Widget
    {
        std::string text;
        Rect rect;
        WidgetBG bg;
        Font font;
        WidgetTextStyle style = WidgetTextStyle::Center;

        Widget(const std::string& _text, const Rect& _rect, const WidgetBG& _bg, const Font& _font)
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

        void paint(Painter& painter)
        {
            // todo
        }
    };
}

