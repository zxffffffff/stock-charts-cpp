/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "Painter.h"
#include "QPainter"
#include "QPainterPath"

namespace StockCharts
{
    class PainterQt : public Painter
    {
    public:
        PainterQt(QPaintDevice *device)
            : painter(device)
        {
        }

        static QColor parseColor(const Color &color)
        {
            return QColor(color.r, color.g, color.b, color.a);
        }

        static Qt::PenStyle parsePenStyle(LineType lineStyle)
        {
            switch (lineStyle)
            {
            case StockCharts::LineType::SolidLine:
                return Qt::SolidLine;
            case StockCharts::LineType::DashLine:
                return Qt::DashLine;
            case StockCharts::LineType::DotLine:
                return Qt::DotLine;
            case StockCharts::LineType::DashDotLine:
                return Qt::DashDotLine;
            default:
                return Qt::SolidLine;
            }
        }

        static QPen parsePen(const Pen &pen)
        {
            return QPen(parseColor(pen.color), pen.lineWidth, parsePenStyle(pen.lineType));
        }

        static int parsePaintDirection(PaintDirection dir)
        {
            switch (dir)
            {
            case StockCharts::PaintDirection::TopLeft:
                return (Qt::AlignTop | Qt::AlignLeft);
            case StockCharts::PaintDirection::TopCenter:
                return (Qt::AlignTop | Qt::AlignHCenter);
            case StockCharts::PaintDirection::TopRight:
                return (Qt::AlignTop | Qt::AlignRight);
            case StockCharts::PaintDirection::CenterLeft:
                return (Qt::AlignVCenter | Qt::AlignLeft);
            case StockCharts::PaintDirection::Center:
                return (Qt::AlignVCenter | Qt::AlignHCenter);
            case StockCharts::PaintDirection::CenterRight:
                return (Qt::AlignVCenter | Qt::AlignRight);
            case StockCharts::PaintDirection::BottomLeft:
                return (Qt::AlignBottom | Qt::AlignLeft);
            case StockCharts::PaintDirection::BottomCenter:
                return (Qt::AlignBottom | Qt::AlignHCenter);
            case StockCharts::PaintDirection::BottomRight:
                return (Qt::AlignBottom | Qt::AlignRight);
            default:
                return 0;
            }
        }

        virtual void save() override
        {
            painter.save();
        }
        virtual void restore() override
        {
            painter.restore();
        }

        virtual void drawString(const Rect &rect, const std::string &text, const Font &font) override
        {
            if (!rect.valid())
                return;
            if (painter.font().pixelSize() != font.fontSize)
            {
                QFont qfont = painter.font();
                qfont.setPixelSize(font.fontSize);
                painter.setFont(qfont);
            }
            painter.setPen(parseColor(font.color));
            painter.drawText(
                std::round(rect.left()),
                std::round(rect.top()),
                std::round(rect.width()),
                std::round(rect.height()),
                parsePaintDirection(font.dir),
                QString::fromLocal8Bit(text));
        }

        virtual void drawRect(const Rect &rect, const Pen &pen) override
        {
            if (!rect.valid())
                return;
            painter.setPen(parsePen(pen));
            painter.drawRect(
                std::round(rect.left()),
                std::round(rect.top()),
                std::round(rect.width() - 1),
                std::round(rect.height()));
        }

        virtual void fillRect(const Rect &rect, const Pen &pen) override
        {
            if (!rect.valid())
                return;
            int x = std::round(rect.left());
            int y = std::round(rect.top());
            int w = std::round(rect.width());
            int h = std::round(rect.height());
            if (w < 1)
                w = 1;
            if (h < 1)
                h = 1;
            painter.fillRect(x, y, w, h, QColor(parseColor(pen.color)));
        }

        virtual void drawLine(const Line &line, const Pen &pen) override
        {
            if (!line.valid())
                return;
            painter.setPen(parsePen(pen));
            painter.drawLine(
                std::round(line.first.x),
                std::round(line.first.y),
                std::round(line.second.x),
                std::round(line.second.y));
        }

        virtual void drawPath(const std::vector<Point> &points, const Pen &pen) override
        {
            painter.setPen(parsePen(pen));
            QPainterPath path;
            path.moveTo(
                std::round(points[0].x),
                std::round(points[0].y));
            for (int i = 1; i < points.size(); i++)
            {
                if (!points[i].valid())
                    break;
                path.lineTo(
                    std::round(points[i].x),
                    std::round(points[i].y));
            }
            painter.drawPath(path); // todo:performance
        }

    private:
        QPainter painter;
    };
}
