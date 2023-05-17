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

    struct Widget
    {
        std::string text;
        Rect rect;
        WidgetBG bg;
        Font font;

        Widget()
        {
        }

        Widget(const std::string &_text, const Rect &_rect, const WidgetBG &_bg, const Font &_font)
            : text(_text), rect(_rect), bg(_bg), font(_font)
        {
        }

        void paint(Painter &painter)
        {
            // todo
            painter.fillRect(rect, bg.normal.colorBG);
            painter.drawRect(rect, bg.normal.colorBorder);
            painter.drawString(rect, text, font);
        }
    };
}
