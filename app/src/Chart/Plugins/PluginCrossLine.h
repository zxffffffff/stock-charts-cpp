/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "ChartPlugin.h"
#include <array>

namespace StockCharts
{
    class PluginCrossLine : public ChartPlugin
    {
    public:
        PluginCrossLine(std::shared_ptr<const StockCore> stockCore, std::shared_ptr<const ChartProps> props)
            : ChartPlugin(stockCore, props)
        {
        }
        virtual ~PluginCrossLine() = default;

        // [2]
        virtual void onContextChanged(std::shared_ptr<const ChartContext> context) override
        {
            onMouseMove(context);
        }

        virtual void onMouseMove(std::shared_ptr<const ChartContext> context) override
        {
            const auto& ctx = *context;
            const auto& props = *m_props;
            ChartCoordinate coordinate(context, m_props);

            Real x = coordinate.index2pos(ctx.hoverIndex);
            const Real bgWidth = props.crossTextBGSize.width;
            const Real bgHalfWidth = bgWidth / 2;
            const Real bgHeight = props.crossTextBGSize.height;
            const Real bgHalfHeight = bgHeight / 2;

            crossLineX.clear();
            crossLineY.clear();
            crossXBG.clear();
            crossYLBG.clear();
            crossYRBG.clear();
            crossXText.clear();
            crossYLText.clear();
            crossYRText.clear();
            bool xCross = false;
            bool yCross = false;

            if (!ctx.syncHover) {
                if (ctx.rectInnerChart.contains(ctx.pointHover))
                    xCross = true, yCross = true;
            }
            else {
                if (ctx.pointHover.x >= ctx.rectInnerChart.left() && ctx.pointHover.x < ctx.rectInnerChart.right())
                    xCross = true;
                if (ctx.pointHover.y >= ctx.rectInnerChart.top() && ctx.pointHover.y < ctx.rectInnerChart.bottom())
                    yCross = true;
            }

            if (xCross) {
                crossLineX.set(
                    x,
                    ctx.rectChart.top() + 1,
                    x,
                    ctx.rectChart.bottom() - 1
                );

                crossXBG.set(
                    x - bgHalfWidth,
                    ctx.rectXAxis.top() + 1,
                    bgWidth,
                    ctx.rectXAxis.height() - 2
                ).moveInside(ctx.rectXAxis);
                switch (props.xAxisHoverType)
                {
                case EnXAxisType::yyyyMMdd:
                default:
                    crossXText = NumberUtils::toTimestamp(m_stockCore->timestamp[ctx.hoverIndex], "%Y-%m-%d");
                    break;
                }
            }
            if (yCross) {
                crossLineY.set(
                    ctx.rectChart.left() + 1,
                    ctx.pointHover.y,
                    ctx.rectChart.right() - 1,
                    ctx.pointHover.y
                );

                crossYLBG.set(
                    ctx.rectYLAxis.left() + 1,
                    ctx.pointHover.y - bgHalfHeight,
                    ctx.rectYLAxis.width() - 2,
                    bgHeight
                ).moveInside(ctx.rectYLAxis);
                crossYLText = NumberUtils::toString(ctx.hoverPrice, props.precision);

                crossYRBG.set(
                    ctx.rectYRAxis.left() + 1,
                    ctx.pointHover.y - bgHalfHeight,
                    ctx.rectYRAxis.width() - 2,
                    bgHeight
                ).moveInside(ctx.rectYRAxis);
                crossYRText = NumberUtils::toString(ctx.hoverPrice, props.precision);
            }
        }

        virtual void onPaint(std::shared_ptr<const ChartContext> context, Painter& painter) override
        {
            const auto& ctx = *context;
            const auto& props = *m_props;

            // x
            painter.drawLine(crossLineX, props.crossLineStyle);

            painter.fillRect(crossXBG, props.crossTextBGStyle);
            painter.drawString(
                crossXBG,
                crossXText,
                props.xAxisHoverTextFont
            );

            // y
            painter.drawLine(crossLineY, props.crossLineStyle);

            painter.fillRect(crossYLBG, props.crossTextBGStyle);
            painter.drawString(
                crossYLBG,
                crossYLText,
                props.ylAxisHoverTextFont
            );

            painter.fillRect(crossYRBG, props.crossTextBGStyle);
            painter.drawString(
                crossYRBG,
                crossYRText,
                props.yrAxisHoverTextFont
            );
        }

    private:
        Line crossLineX;
        Rect crossXBG;
        std::string crossXText;

        Line crossLineY;
        Rect crossYLBG;
        Rect crossYRBG;
        std::string crossYLText;
        std::string crossYRText;
    };
}

