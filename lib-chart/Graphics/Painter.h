/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "Graphics.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace StockCharts
{
    using Byte = unsigned char;

    struct Color
    {
        Byte r;
        Byte g;
        Byte b;
        Byte a;

        constexpr Color(Byte _r = 0, Byte _g = 255, Byte _b = 255, Byte _a = 255)
            : r(_r), g(_g), b(_b), a(_a)
        {
        }

        Color(const char *hex)
        {
            std::size_t len = strlen(hex);
            if ((len & 1) && (hex[0] == '#'))
            {
                hex += 1;
                len -= 1;
            }

            unsigned int _r, _g, _b, _a;
            if (len == 6)
            {
                sscanf(hex, "%02x%02x%02x", &_r, &_g, &_b);
                _a = 255;
            }
            else if (len == 8)
            {
                sscanf(hex, "%02x%02x%02x%02x", &_r, &_g, &_b, &_a);
            }
            else
            {
                // windows invalid color
                _r = 0;
                _g = _b = _a = 255;
            }
            r = _r;
            g = _g;
            b = _b;
            a = _a;
        }

        bool operator==(const Color &rhs) const
        {
            return (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a);
        }
        bool operator!=(const Color &rhs) const
        {
            return !operator==(rhs);
        }
    };

    constexpr inline Color ColorWhite = Color(255, 255, 255);
    constexpr inline Color ColorBlack = Color(0, 0, 0);
    constexpr inline Color ColorGray = Color(160, 160, 164);
    constexpr inline Color ColorDarkGray = Color(128, 128, 128);
    constexpr inline Color ColorLightGray = Color(192, 192, 192);
    constexpr inline Color ColorTransparent = Color(0, 0, 0, 0);

    enum class LineType
    {
        SolidLine,   // _____
        DashLine,    // _ _ _
        DotLine,     // . . .
        DashDotLine, // _ . _
    };

    struct Pen
    {
        Color color;
        Real lineWidth;
        LineType lineType;

        Pen(const Color &_color = ColorBlack, Real _lineWidth = 1, LineType _lineType = LineType::SolidLine)
            : color(_color), lineWidth(_lineWidth), lineType(_lineType)
        {
        }

        bool operator==(const Pen &rhs) const
        {
            return (color == rhs.color && lineWidth == rhs.lineWidth && lineType == rhs.lineType);
        }
        bool operator!=(const Pen &rhs) const
        {
            return !operator==(rhs);
        }
    };

    enum class PaintDirection
    {
        TopLeft,
        TopCenter,
        TopRight,
        CenterLeft,
        Center,
        CenterRight,
        BottomLeft,
        BottomCenter,
        BottomRight,
    };

    struct Font
    {
        Color color;
        int fontSize;
        PaintDirection dir;

        Font(const Color &_color = ColorBlack, int _fontSize = 12, PaintDirection _dir = PaintDirection::Center)
            : color(_color), fontSize(_fontSize), dir(_dir)
        {
        }

        bool operator==(const Font &rhs) const
        {
            return (color == rhs.color && fontSize == rhs.fontSize && dir == rhs.dir);
        }
        bool operator!=(const Font &rhs) const
        {
            return !operator==(rhs);
        }
    };

    class Painter
    {
    public:
        Painter() {}
        virtual ~Painter() {}

        virtual void save() = 0;
        virtual void restore() = 0;

        virtual void drawString(const Rect &rect, const std::string &text, const Font &font) = 0;
        virtual void drawRect(const Rect &rect, const Pen &pen) = 0;
        virtual void fillRect(const Rect &rect, const Pen &pen) = 0;
        virtual void drawLine(const Line &line, const Pen &pen) = 0;
        virtual void drawPath(const std::vector<Point> &points, const Pen &pen) = 0;

        virtual void drawStick(const std::vector<Stick> &sticks, const Color &rise, const Color &fall) = 0;
        virtual void drawStickHollow(const std::vector<Stick> &sticks, const Color &rise, const Color &fall) = 0;
        virtual void drawBAR(const std::vector<Stick> &sticks, const Color &rise, const Color &fall) = 0;
    };
}
