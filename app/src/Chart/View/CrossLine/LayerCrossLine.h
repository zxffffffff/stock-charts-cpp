/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../ChartLayer.h"
#include <array>

namespace StockCharts
{
    struct StCrossLine
    {
        Line xLine;
        Rect xBG;
        std::string xText;

        Line yLine;
        Rect ylBG;
        Rect yrBG;
        std::string ylText;
        std::string yrText;
    };

    class LayerCrossLine : public ChartLayer
    {
    public:
        LayerCrossLine(std::shared_ptr<const ChartModel> model, std::shared_ptr<const ChartProps> props, std::shared_ptr<const ChartContext> context)
            : ChartLayer(model, props, context)
        {
        }
        virtual ~LayerCrossLine() = default;

        virtual void onContextChanged() override
        {
            onMouseMove();
        }

        virtual void onMouseMove() override
        {
            const auto& stockCore = *m_model->getStockCore();
            const auto& props = *m_props;
            const auto& ctx = *m_context;
            ChartCoordinate coordinate(m_props, m_context);

            auto calcCross = [&](const StMouseHover& mouseHover, StCrossLine& crossLine)
            {
                Real x = coordinate.index2pos(mouseHover.index);
                const Real bgWidth = props.crossTextBGSize.width;
                const Real bgHalfWidth = bgWidth / 2;
                const Real bgHeight = props.crossTextBGSize.height;
                const Real bgHalfHeight = bgHeight / 2;

                crossLine.xLine.clear();
                crossLine.xBG.clear();
                crossLine.xText.clear();

                crossLine.yLine.clear();
                crossLine.ylBG.clear();
                crossLine.yrBG.clear();
                crossLine.ylText.clear();
                crossLine.yrText.clear();

                bool xCross = false;
                bool yCross = false;
                if (mouseHover.point.x >= ctx.rectInnerChart.left() &&
                    mouseHover.point.x < ctx.rectInnerChart.right())
                    xCross = true;
                if (mouseHover.point.y >= ctx.rectInnerChart.top() &&
                    mouseHover.point.y < ctx.rectInnerChart.bottom())
                    yCross = true;

                if (xCross) {
                    crossLine.xLine.set(
                        x,
                        ctx.rectChart.top() + 1,
                        x,
                        ctx.rectChart.bottom() - 1
                    );

                    crossLine.xBG.set(
                        x - bgHalfWidth,
                        ctx.rectXAxis.top() + 1,
                        bgWidth,
                        ctx.rectXAxis.height() - 2
                    ).moveInside(ctx.rectXAxis);
                    switch (props.xAxisHoverType)
                    {
                    case EnXAxisType::yyyyMMdd:
                    default:
                        crossLine.xText = NumberUtils::toTimestamp(stockCore.timestamp[mouseHover.index], "%Y-%m-%d");
                        break;
                    }
                }
                if (yCross) {
                    crossLine.yLine.set(
                        ctx.rectChart.left() + 1,
                        mouseHover.point.y,
                        ctx.rectChart.right() - 1,
                        mouseHover.point.y
                    );

                    crossLine.ylBG.set(
                        ctx.rectYLAxis.left() + 1,
                        mouseHover.point.y - bgHalfHeight,
                        ctx.rectYLAxis.width() - 2,
                        bgHeight
                    ).moveInside(ctx.rectYLAxis);
                    crossLine.ylText = NumberUtils::toString(mouseHover.price, props.precision);

                    crossLine.yrBG.set(
                        ctx.rectYRAxis.left() + 1,
                        mouseHover.point.y - bgHalfHeight,
                        ctx.rectYRAxis.width() - 2,
                        bgHeight
                    ).moveInside(ctx.rectYRAxis);
                    crossLine.yrText = NumberUtils::toString(mouseHover.price, props.precision);
                }
            };

            calcCross(ctx.hoverNormal, crossNormal);
            calcCross(ctx.hoverSync, crossSync);
        }

        virtual void onPaint(Painter& painter) override
        {
            const auto& props = *m_props;
            const auto& ctx = *m_context;

            auto paintCross = [&](const StCrossLine& crossLine)
            {
                // x
                if (ctx.crossLineVisible)
                    painter.drawLine(crossLine.xLine, props.crossLineStyle);

                painter.fillRect(crossLine.xBG, props.crossTextBGStyle);
                painter.drawString(
                    crossLine.xBG,
                    crossLine.xText,
                    props.xAxisHoverTextFont
                );

                // y
                if (ctx.crossLineVisible)
                    painter.drawLine(crossLine.yLine, props.crossLineStyle);

                painter.fillRect(crossLine.ylBG, props.crossTextBGStyle);
                painter.drawString(
                    crossLine.ylBG,
                    crossLine.ylText,
                    props.ylAxisHoverTextFont
                );

                painter.fillRect(crossLine.yrBG, props.crossTextBGStyle);
                painter.drawString(
                    crossLine.yrBG,
                    crossLine.yrText,
                    props.yrAxisHoverTextFont
                );
            };

            paintCross(crossSync);
            paintCross(crossNormal);
        }

    private:
        StCrossLine crossNormal;
        StCrossLine crossSync;
    };
}

