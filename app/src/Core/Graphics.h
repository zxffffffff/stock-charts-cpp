#pragma once

namespace StockCharts
{
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

        Point& set(Real _x, Real _y)
        {
            x = _x;
            y = _y;
            return *this;
        }

        bool operator==(const Point& rhs) const
        {
            return (x == rhs.x && y == rhs.y);
        }
        bool operator!=(const Point& rhs) const
        {
            return !operator==(rhs);
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

        Size& set(Real _width, Real _height)
        {
            width = _width;
            height = _height;
            return *this;
        }

        bool operator==(const Size& rhs) const
        {
            return (width == rhs.width && height == rhs.height);
        }
        bool operator!=(const Size& rhs) const
        {
            return !operator==(rhs);
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

        bool operator==(const Rect& rhs) const
        {
            return (point == rhs.point && size == rhs.size);
        }
        bool operator!=(const Rect& rhs) const
        {
            return !operator==(rhs);
        }
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

        bool operator==(const Line& rhs) const
        {
            return (first == rhs.first && second == rhs.second);
        }
        bool operator!=(const Line& rhs) const
        {
            return !operator==(rhs);
        }
    };

    struct Stick
    {
        Rect rect;
        Line line;
        int flag;

        Stick(Real _x = 0, Real _y = 0, Real _width = 0, Real _height = 0, Real _high = 0, Real _low = 0, int _flag = 0)
            : rect(_x, _y, _width, _height)
            , line(rect.centerX(), _high, rect.centerX(), _low)
            , flag(_flag)
        {
        }

        Stick(const Rect& _rect, const Line& _line, int _flag = 0)
            : rect(_rect)
            , line(_line)
            , flag(_flag)
        {
        }

        Stick& set(Real _x, Real _y, Real _width, Real _height, Real _high, Real _low, int _flag)
        {
            rect.set(_x, _y, _width, _height);
            line.set(rect.centerX(), _high, rect.centerX(), _low);
            flag = _flag;
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

        bool operator==(const Stick& rhs) const
        {
            return (rect == rhs.rect && line == rhs.line);
        }
        bool operator!=(const Stick& rhs) const
        {
            return !operator==(rhs);
        }
    };
}
