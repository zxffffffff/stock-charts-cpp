#pragma once
#include "Graphics.h"

namespace StockCharts
{
    struct Color
    {
        int r;
        int g;
        int b;
        int a;

        Color(int _r = 0, int _g = 0, int _b = 0, int _a = 255)
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

    class Painter
    {
    public:
        Painter() {}
        virtual ~Painter() {}

        virtual void save() = 0;
        virtual void restore() = 0;

        virtual void fillRect(const Rect& rect, const Color& color) = 0;
        virtual void drawLine(const Line& line, const Color& color) = 0;
        virtual void drawLines(const std::vector<Point>& points, const Color& color) = 0;
        virtual void drawStick(const std::vector<Stick>& sticks, const Color& rise, const Color& fall) = 0;
    };
}

