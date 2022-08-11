/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "Painter.h"

#ifdef QT_VERSION
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

        virtual void drawRect(const Rect& rect, const Color& color) override
        {
            painter.setPen(QColor(color.r, color.g, color.b, color.a));
            painter.drawRect(
                std::round(rect.left()),
                std::round(rect.top()),
                std::round(rect.width() - 1), // ºÊ»›
                std::round(rect.height())
            );
        }

        virtual void fillRect(const Rect& rect, const Color& color) override
        {
            painter.fillRect(
                std::round(rect.left()),
                std::round(rect.top()),
                std::round(rect.width()),
                std::round(rect.height()),
                QColor(color.r, color.g, color.b, color.a)
            );
        }

        virtual void drawLine(const Line& line, const Color& color) override
        {
            painter.setPen(QColor(color.r, color.g, color.b, color.a));
            painter.drawLine(
                std::round(line.first.x),
                std::round(line.first.y),
                std::round(line.second.x),
                std::round(line.second.y)
            );
        }

        virtual void drawPath(const std::vector<Point>& points, const Color& color) override
        {
            if (points.empty())
                return;
            painter.setPen(QColor(color.r, color.g, color.b, color.a));
            QPainterPath path;
            path.moveTo(
                std::round(points[0].x),
                std::round(points[0].y)
            );
            for (int i = 1; i < points.size(); i++) {
                path.lineTo(
                    std::round(points[i].x),
                    std::round(points[i].y)
                );
            }
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

        virtual void drawStickHollow(const std::vector<Stick>& sticks, const Color& rise, const Color& fall) override
        {
            if (sticks.empty())
                return;
            for (auto& stick : sticks) {
                const Color& color = stick.flag >= 0 ? rise : fall;
                if (stick.flag >= 0) {
                    drawRect(stick.rect, color);
                    drawLine(Line(stick.centerX(), stick.top() - 1, stick.centerX(), stick.line.first.y), color);
                    drawLine(Line(stick.centerX(), stick.bottom() + 1, stick.centerX(), stick.line.second.y), color);
                }
                else {
                    fillRect(stick.rect, color);
                    drawLine(stick.line, color);
                }
            }
        }

        virtual void drawBAR(const std::vector<Stick>& sticks, const Color& rise, const Color& fall) override
        {
            if (sticks.empty())
                return;
            for (auto& stick : sticks) {
                const Color& color = stick.flag >= 0 ? rise : fall;
                int openY;
                int closeY;
                if (stick.flag >= 0) {
                    closeY = stick.top();
                    openY = stick.bottom();
                }
                else {
                    openY = stick.top();
                    closeY = stick.bottom();
                }
                drawLine(stick.line, color);
                drawLine(Line(stick.left(), openY, stick.centerX(), openY), color);
                drawLine(Line(stick.right(), closeY, stick.centerX(), closeY), color);
            }
        }

    private:
        QPainter painter;
    };
}
#endif // QT_VERSION

