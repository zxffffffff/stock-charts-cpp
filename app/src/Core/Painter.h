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

#include "QPainter"
#include "QPainterPath"

namespace StockCharts
{
    class PainterQt : public Painter
    {
    public:
        PainterQt(QPaintDevice* device) 
            : painter(device)
        {
        }

        virtual void save() override
        {
            painter.save();
        }
        virtual void restore() override
        {
            painter.restore();
        }

        virtual void fillRect(const Rect& rect, const Color& color) override
        {
            painter.fillRect(
                rect.left(),
                rect.top(),
                rect.width(),
                rect.height(),
                QColor(color.r, color.g, color.b, color.a)
            );
        }

        virtual void drawLine(const Line& line, const Color& color) override
        {
            painter.setPen(QColor(color.r, color.g, color.b, color.a));
            painter.drawLine(
                line.first.x,
                line.first.y,
                line.second.x,
                line.second.y
            );
        }

        virtual void drawLines(const std::vector<Point>& points, const Color& color) override
        {
            if (points.empty())
                return;
            painter.setPen(QColor(color.r, color.g, color.b, color.a));
            QPainterPath path;
            path.moveTo(points[0].x, points[0].y);
            for (int i = 1; i < points.size(); i++)
                path.lineTo(points[i].x, points[i].y);
            painter.drawPath(path);
        }

        virtual void drawStick(const std::vector<Stick>& sticks, const Color& rise, const Color& fall) override
        {
            if (sticks.empty())
                return;
            for (auto& stick : sticks) {
                const Color& color = stick.flag >= 0 ? rise : fall;
                fillRect(stick.rect, color);
                drawLine(stick.line, color);
            }
        }

    private:
        QPainter painter;
    };
}

