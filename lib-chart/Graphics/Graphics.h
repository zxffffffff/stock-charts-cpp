/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include <cmath>
#include <limits>
#include <array>

namespace StockCharts
{
    using Real = float;
    constexpr inline Real RealNull = std::numeric_limits<Real>::lowest();

    struct Point
    {
        Real x;
        Real y;

        Point(Real _x = RealNull, Real _y = RealNull)
            : x(_x), y(_y)
        {
        }

        Point &set(Real _x, Real _y)
        {
            x = _x;
            y = _y;
            return *this;
        }
        void clear()
        {
            x = RealNull;
            y = RealNull;
        }

        bool valid() const
        {
            if (std::isnan(x) || std::isinf(x))
                return false;
            if (std::isnan(y) || std::isinf(y))
                return false;
            return (x != RealNull && y != RealNull);
        }

        bool operator==(const Point &rhs) const
        {
            return (x == rhs.x && y == rhs.y);
        }
        bool operator!=(const Point &rhs) const
        {
            return !operator==(rhs);
        }
    };

    struct Size
    {
        Real width;
        Real height;

        Size(Real _width = 0, Real _height = 0)
            : width(_width), height(_height)
        {
        }

        Size &set(Real _width, Real _height)
        {
            width = _width;
            height = _height;
            return *this;
        }
        void clear()
        {
            width = 0;
            height = 0;
        }

        bool valid() const
        {
            return (width >= 0 && height >= 0);
        }

        bool operator==(const Size &rhs) const
        {
            return (width == rhs.width && height == rhs.height);
        }
        bool operator!=(const Size &rhs) const
        {
            return !operator==(rhs);
        }
    };

    struct Rect
    {
        Point point;
        Size size;

        Rect(Real _x = RealNull, Real _y = RealNull, Real _width = 0, Real _height = 0)
            : point(_x, _y), size(_width, _height)
        {
        }

        Rect(const Point &_point, const Size &_size)
            : point(_point), size(_size)
        {
        }

        Rect &set(Real _x, Real _y, Real _width, Real _height)
        {
            point.x = _x;
            point.y = _y;
            size.width = _width;
            size.height = _height;
            return *this;
        }
        void clear()
        {
            point.clear();
            size.clear();
        }

        Rect &moveInside(const Rect &parent, std::array<Real, 4> padding = {1, 1, 0, 0})
        {
            if (left() < parent.left() + padding[0])
                point.x = parent.left() + padding[0];
            if (top() < parent.top() + padding[1])
                point.y = parent.top() + padding[1];
            if (right() > parent.right() - padding[2])
                point.x = parent.right() - padding[2] - width();
            if (bottom() > parent.bottom() - padding[3])
                point.y = parent.bottom() - padding[3] - height();
            return *this;
        }

        Rect &clipInside(const Rect &parent, std::array<Real, 4> padding = {0, 0, 0, 0})
        {
            Real offset_l = (parent.left() + padding[0]) - left();
            if (offset_l > 0)
            {
                point.x -= offset_l;
                size.width -= offset_l;
            }
            Real offset_t = (parent.top() + padding[1]) - top();
            if (offset_t > 0)
            {
                point.y -= offset_t;
                size.height -= offset_t;
            }
            Real offset_r = right() - (parent.right() - padding[2]);
            if (offset_r > 0)
                size.width -= offset_r;
            Real offset_b = bottom() - (parent.bottom() - padding[3]);
            if (offset_b > 0)
                size.height -= offset_b;

            if (!valid())
                clear();
            return *this;
        }

        bool valid() const
        {
            return (point.valid() && size.valid());
        }

        bool contains(const Point &point) const
        {
            if (!valid() || !point.valid())
                return false;
            return (point.x >= left() && point.x < right() && point.y >= top() && point.y < bottom());
        }

        Real left() const { return point.x; }
        Real right() const { return point.x + size.width; }
        Real top() const { return point.y; }
        Real bottom() const { return point.y + size.height; }
        Real width() const { return size.width; }
        Real height() const { return size.height; }
        Real centerX() const { return left() + width() / 2; }
        Real centerY() const { return top() + height() / 2; }
        Point center() const { return {centerX(), centerY()}; }
        Point topLeft() const { return {left(), top()}; }
        Point topRight() const { return {right(), top()}; }
        Point bottomLeft() const { return {left(), bottom()}; }
        Point bottomRight() const { return {right(), bottom()}; }

        bool operator==(const Rect &rhs) const
        {
            return (point == rhs.point && size == rhs.size);
        }
        bool operator!=(const Rect &rhs) const
        {
            return !operator==(rhs);
        }
    };

    struct Line
    {
        Point first;
        Point second;

        Line(Real _x1 = RealNull, Real _y1 = RealNull, Real _x2 = RealNull, Real _y2 = RealNull)
            : first(_x1, _y1), second(_x2, _y2)
        {
        }

        Line(const Point &_point1, const Point &_point2)
            : first(_point1), second(_point2)
        {
        }

        Line &set(Real _x1, Real _y1, Real _x2, Real _y2)
        {
            first.x = _x1;
            first.y = _y1;
            second.x = _x2;
            second.y = _y2;
            return *this;
        }
        void clear()
        {
            first.clear();
            second.clear();
        }

        bool valid() const
        {
            return (first.valid() && second.valid());
        }

        bool operator==(const Line &rhs) const
        {
            return (first == rhs.first && second == rhs.second) || (first == rhs.second && second == rhs.first);
        }
        bool operator!=(const Line &rhs) const
        {
            return !operator==(rhs);
        }
    };

    struct Stick
    {
        Rect rect;
        Line line;
        int flag;

        Stick(
            Real _x = RealNull,
            Real _y = RealNull,
            Real _width = 0,
            Real _height = 0,
            Real _high = RealNull,
            Real _low = RealNull,
            int _flag = 0)
            : rect(_x, _y, _width, _height), line(rect.centerX(), _high, rect.centerX(), _low), flag(_flag)
        {
        }

        Stick(const Rect &_rect, const Line &_line, int _flag = 0)
            : rect(_rect), line(_line), flag(_flag)
        {
        }

        Stick &set(Real _x, Real _y, Real _width, Real _height, Real _high, Real _low, int _flag)
        {
            rect.set(_x, _y, _width, _height);
            line.set(rect.centerX(), _high, rect.centerX(), _low);
            flag = _flag;
            return *this;
        }
        void clear()
        {
            rect.clear();
            line.clear();
            flag = 0;
        }

        bool valid() const
        {
            return (rect.valid() && line.valid());
        }

        Real left() const { return rect.left(); }
        Real right() const { return rect.right(); }
        Real top() const { return rect.top(); }
        Real bottom() const { return rect.bottom(); }
        Real width() const { return rect.width(); }
        Real height() const { return rect.height(); }
        Real high() const { return line.first.y; }
        Real low() const { return line.second.y; }
        Real height2() const { return low() - high(); }
        Real centerX() const { return left() + width() / 2; }
        Real centerY() const { return high() + height2() / 2; }
        Point center() const { return {centerX(), centerY()}; }

        bool operator==(const Stick &rhs) const
        {
            return (rect == rhs.rect && line == rhs.line);
        }
        bool operator!=(const Stick &rhs) const
        {
            return !operator==(rhs);
        }
    };
}
