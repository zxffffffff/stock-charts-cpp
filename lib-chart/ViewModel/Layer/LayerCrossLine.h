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
        virtual void init(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context) override
        {
        }

        virtual void onContextChanged(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context) override
        {
            onMouseMove(model, props, context);
        }

        virtual void onMouseMove(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context) override
        {
            auto stockCore = *model->getStockCore();
            ChartCoordinate coordinate(props, context);

            auto calcCross = [&](const StMouseHover &mouseHover, StCrossLine &crossLine)
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
                if (mouseHover.point.x >= context.rectInnerChart.left() &&
                    mouseHover.point.x < context.rectInnerChart.right())
                    xCross = true;
                if (mouseHover.point.y >= context.rectInnerChart.top() &&
                    mouseHover.point.y < context.rectInnerChart.bottom())
                    yCross = true;

                if (xCross)
                {
                    crossLine.xLine.set(
                        x,
                        context.rectChart.top() + 1,
                        x,
                        context.rectChart.bottom() - 1);

                    crossLine.xBG.set(
                                     x - bgHalfWidth,
                                     context.rectXAxis.top() + 1,
                                     bgWidth,
                                     context.rectXAxis.height() - 2)
                        .moveInside(context.rectXAxis);
                    switch (props.xAxisHoverType)
                    {
                    case EnXAxisType::yyyyMMdd:
                    default:
                        crossLine.xText = NumberUtils::toTimestamp(stockCore.timestamp[mouseHover.index], "%Y-%m-%d");
                        break;
                    }
                }
                if (yCross)
                {
                    crossLine.yLine.set(
                        context.rectChart.left() + 1,
                        mouseHover.point.y,
                        context.rectChart.right() - 1,
                        mouseHover.point.y);

                    crossLine.ylBG.set(
                                      context.rectYLAxis.left() + 1,
                                      mouseHover.point.y - bgHalfHeight,
                                      context.rectYLAxis.width() - 2,
                                      bgHeight)
                        .moveInside(context.rectYLAxis);
                    crossLine.ylText = NumberUtils::toString(mouseHover.price, props.precision);

                    crossLine.yrBG.set(
                                      context.rectYRAxis.left() + 1,
                                      mouseHover.point.y - bgHalfHeight,
                                      context.rectYRAxis.width() - 2,
                                      bgHeight)
                        .moveInside(context.rectYRAxis);
                    crossLine.yrText = NumberUtils::toString(mouseHover.price, props.precision);
                }
            };

            calcCross(context.hoverNormal, crossNormal);
            calcCross(context.hoverSync, crossSync);
        }

        virtual void onPaint(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context,
            Painter &painter) override
        {
            auto paintCross = [&](const StCrossLine &crossLine)
            {
                // x
                if (context.crossLineVisible)
                    painter.drawLine(crossLine.xLine, props.crossLineStyle);

                painter.fillRect(crossLine.xBG, props.crossTextBGStyle);
                painter.drawString(
                    crossLine.xBG,
                    crossLine.xText,
                    props.xAxisHoverTextFont);

                // y
                if (context.crossLineVisible)
                    painter.drawLine(crossLine.yLine, props.crossLineStyle);

                painter.fillRect(crossLine.ylBG, props.crossTextBGStyle);
                painter.drawString(
                    crossLine.ylBG,
                    crossLine.ylText,
                    props.ylAxisHoverTextFont);

                painter.fillRect(crossLine.yrBG, props.crossTextBGStyle);
                painter.drawString(
                    crossLine.yrBG,
                    crossLine.yrText,
                    props.yrAxisHoverTextFont);
            };

            paintCross(crossSync);
            paintCross(crossNormal);
        }

    private:
        StCrossLine crossNormal;
        StCrossLine crossSync;
    };
}
