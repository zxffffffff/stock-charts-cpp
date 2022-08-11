/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include "Graphics.h"

namespace StockCharts
{
    using Byte = unsigned char;

    struct Color
    {
        Byte r;
        Byte g;
        Byte b;
        Byte a;

        Color(const char* hex)
        {
            unsigned int _r, _g, _b, _a;
            const size_t len = strlen(hex);
            if (len == 6) {
                sscanf(hex, "%02x%02x%02x", &_r, &_g, &_b);
                _a = 255;
            }
            else if (len == 8) {
                sscanf(hex, "%02x%02x%02x%02x", &_r, &_g, &_b, &_a);
            }
            else {
                // windows invalid color
                _r = 0;
                _g = _b = _a = 255;
            }
            r = _r;
            g = _g;
            b = _b;
            a = _a;
        }

        Color(Byte _r = 0, Byte _g = 255, Byte _b = 255, Byte _a = 255)
            : r(_r)
            , g(_g)
            , b(_b)
            , a(_a)
        {
        }

        bool operator==(const Color& rhs) const
        {
            return (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a);
        }
        bool operator!=(const Color& rhs) const
        {
            return !operator==(rhs);
        }
    };

    enum class LineType
    {
        SolidLine,      // _____
        DashLine,       // _ _ _
        DotLine,        // . . .
        DashDotLine,    // _ . _
    };

    struct Pen
    {
        Color       color;
        Real        lineWidth;
        LineType    lineType;

        Pen(const Color& _color, Real _lineWidth = 1, LineType _lineType = LineType::SolidLine)
            : color(_color)
            , lineWidth(_lineWidth)
            , lineType(_lineType)
        {
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
        Color           color;
        int             fontSize;
        PaintDirection  dir;

        Font(const Color& _color, int _fontSize = 12, PaintDirection _dir = PaintDirection::Center)
            : color(_color)
            , fontSize(_fontSize)
            , dir(_dir)
        {
        }
    };

    class Painter
    {
    public:
        Painter() {}
        virtual ~Painter() {}

        virtual void save() = 0;
        virtual void restore() = 0;

        virtual void drawString(const Rect& rect, const std::string& text, const Font& font) = 0;
        virtual void drawRect(const Rect& rect, const Pen& pen) = 0;
        virtual void fillRect(const Rect& rect, const Pen& pen) = 0;
        virtual void drawLine(const Line& line, const Pen& pen) = 0;
        virtual void drawPath(const std::vector<Point>& points, const Pen& pen) = 0;

        virtual void drawStick(const std::vector<Stick>& sticks, const Color& rise, const Color& fall) = 0;
        virtual void drawStickHollow(const std::vector<Stick>& sticks, const Color& rise, const Color& fall) = 0;
        virtual void drawBAR(const std::vector<Stick>& sticks, const Color& rise, const Color& fall) = 0;
    };
}

