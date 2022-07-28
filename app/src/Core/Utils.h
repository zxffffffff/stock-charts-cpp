#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace StockCharts
{
    namespace Utils
    {
        std::wstring	toWStr(const std::string& str);
        std::string		toStr(const std::wstring& wstr);

        std::string		to8bitStr(const std::string& str);

        std::vector<std::string> splitStr(const std::string str, const char separator);

        bool			checkEmpty(const std::string& str);
    }

    using Real = float;

    struct Point
    {
        Real x;
        Real y;

        Point(Real _x = 0, Real _y = 0)
            : x(_x)
            , y(_y)
        {
        }
    };

    struct Size
    {
        Real width;
        Real height;

        Size(Real _width = 0, Real _height = 0)
            : width(_width)
            , height(_height)
        {
        }
    };

    struct Rect
    {
        Point point;
        Size size;

        Rect(Real _x = 0, Real _y = 0, Real _width = 0, Real _height = 0)
            : point(_x, _y)
            , size(_width, _height)
        {
        }

        Rect(const Point& _point, const Size& _size)
            : point(_point)
            , size(_size)
        {
        }

        Rect& set(Real _x, Real _y, Real _width, Real _height)
        {
            point.x = _x;
            point.y = _y;
            size.width = _width;
            size.height = _height;
            return *this;
        }

        Real left() const { return point.x; }
        Real right() const { return point.x + size.width; }
        Real top() const { return point.y; }
        Real bottom() const { return point.y + size.height; }
        Real width() const { return size.width; }
        Real height() const { return size.height; }
        Real centerX() const { return left() + width() / 2; }
        Real centerY() const { return top() + height() / 2; }
        Point center() const { return { centerX(), centerY() }; }
    };

    struct Line
    {
        Point first;
        Point second;

        Line(Real _x1 = 0, Real _y1 = 0, Real _x2 = 0, Real _y2 = 0)
            : first(_x1, _y1)
            , second(_x2, _y2)
        {
        }

        Line(const Point& _point1, const Point& _point2)
            : first(_point1)
            , second(_point2)
        {
        }

        Line& set(Real _x1, Real _y1, Real _x2, Real _y2)
        {
            first.x = _x1;
            first.y = _y1;
            second.x = _x2;
            second.y = _y2;
            return *this;
        }
    };

    struct Stick
    {
        Rect rect;
        Line line;

        Stick(Real _x = 0, Real _y = 0, Real _width = 0, Real _height = 0, Real _high = 0, Real _low = 0)
            : rect(_x, _y, _width, _height)
            , line(rect.centerX(), _high, rect.centerX(), _low)
        {
        }

        Stick(const Rect& _rect, const Line& _line)
            : rect(_rect)
            , line(_line)
        {
        }

        Stick& set(Real _x, Real _y, Real _width, Real _height, Real _high, Real _low)
        {
            rect.set(_x, _y, _width, _height);
            line.set(rect.centerX(), _high, rect.centerX(), _low);
            return *this;
        }

        Real left() const { return rect.left(); }
        Real right() const { return rect.right(); }
        Real top() const { return rect.top(); }
        Real bottom() const { return rect.bottom(); }
        Real width() const { return rect.width(); }
        Real height() const { return rect.height(); }
        Real high() const { return line.first.y; }
        Real low() const { return line.second.y; }
        Real height2() const { return low() - height(); }
        Real centerX() const { return left() + width() / 2; }
        Real centerY() const { return high() + height2() / 2; }
        Point center() const { return { centerX(),  centerY() }; }
    };
}
