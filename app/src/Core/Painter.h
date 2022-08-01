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

        Color(int _r = 0, int _g = 0, int _b = 0, int _a = 255) : r(_r), g(_g), b(_b), a(_a) {}
    };

    class Painter
    {
    public:
        Painter() {}
        virtual ~Painter() {}

        virtual void fillRect(const Rect& rect, const Color& color) = 0;
        virtual void drawLine(const Line& line, const Color& color) = 0;
        virtual void drawStick(const std::vector<Stick>& sticks, const Color& rise, const Color& fall) = 0;
    };
}

#include "QPainter"

namespace StockCharts
{
    class PainterQt : public Painter
    {
    public:
        PainterQt(QPaintDevice* device) : painter(device) {}

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

        virtual void drawStick(const std::vector<Stick>& sticks, const Color& rise, const Color& fall) override
        {
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

