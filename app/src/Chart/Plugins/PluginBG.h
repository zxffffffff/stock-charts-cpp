/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "ChartPlugin.h"

namespace StockCharts
{
    class PluginBG : public ChartPlugin
    {
    public:
        PluginBG(std::shared_ptr<const StockCore> stockCore)
            : ChartPlugin(stockCore)
        {
        }
        virtual ~PluginBG() = default;

        // [2]
        virtual void onContextChanged(std::shared_ptr<const ChartContext> context) override
        {
            auto& ctx = *context;
            ChartCoordinate coordinate(context);

            // x
            xAxisPos.clear();
            xAxisRect.clear();
            xAxisDate.clear();
            const Real textWidth = ctx.props.xAxisTextWidth;
            const Real textHalfWidth = textWidth / 2;
            switch (ctx.props.xAxisType)
            {
            case EnXAxisType::yyyyMM:
            default:
                for (int index = ctx.beginIndex; index < ctx.endIndex; index++) {
                    std::string dt = NumberUtils::toTimestamp(m_stockCore->timestamp[index], "%Y-%m");
                    if (!xAxisPos.empty()) {
                        if (dt == xAxisDate.back())
                            continue;
                    }
                    Real x = coordinate.index2pos(index);
                    xAxisPos.push_back(x);
                    xAxisRect.push_back(Rect(x - textHalfWidth, ctx.rectXAxis.top() + 1, textWidth, ctx.rectXAxis.height() - 2));
                    xAxisRect.back().moveInside(ctx.rectXAxis);
                    xAxisDate.push_back(dt);
                }
                if (xAxisPos.size() >= 2) {
                    xAxisPos.erase(xAxisPos.begin());
                    xAxisRect.erase(xAxisRect.begin());
                    xAxisDate.erase(xAxisDate.begin());
                }
                for (int i = xAxisPos.size() - 1; i >= 1; i--) {
                    auto& pre = xAxisDate[i - 1];
                    auto& cur = xAxisDate[i];
                    if (cur.substr(0, 4) == pre.substr(0, 4))
                        cur = cur.substr(5);
                }
                break;
            }

            //y
            yAxisPos.clear();
            ylAxisRect.clear();
            yrAxisRect.clear();
            ylAxisPrice.clear();
            yrAxisPrice.clear();
            const Real stepHeight = ctx.props.yAxisGridStepHeight;
            const Real stepHalfHeight = stepHeight / 2;
            for (Real y = ctx.rectChart.bottom() - ctx.props.yAxisGridStart; y >= ctx.rectChart.top() + stepHalfHeight; y -= stepHeight) {
                yAxisPos.push_back(y);
                ylAxisRect.push_back(Rect(ctx.rectYLAxis.left() + 1, y - stepHalfHeight, ctx.rectYLAxis.width() - 2, stepHeight));
                yrAxisRect.push_back(Rect(ctx.rectYRAxis.left() + 1, y - stepHalfHeight, ctx.rectYRAxis.width() - 2, stepHeight));
                ylAxisRect.back().moveInside(ctx.rectYLAxis);
                yrAxisRect.back().moveInside(ctx.rectYRAxis);
                ylAxisPrice.push_back(NumberUtils::toString(coordinate.pos2price(y), ctx.props.precision));
                yrAxisPrice.push_back(NumberUtils::toString(coordinate.pos2price(y), ctx.props.precision));
            }
        }

        virtual void onPaint(std::shared_ptr<const ChartContext> context, Painter& painter) override
        {
            auto& ctx = *context;

            painter.fillRect(ctx.rectView, Color(255, 255, 255, 255 * 0.2));
            painter.fillRect(ctx.rectXAxis, Color(150, 100, 100, 255 * 0.2));
            painter.fillRect(ctx.rectYLAxis, Color(100, 150, 100, 255 * 0.2));
            painter.fillRect(ctx.rectYRAxis, Color(100, 150, 100, 255 * 0.2));
            painter.fillRect(ctx.rectChart, Color(100, 100, 150, 255 * 0.2));
            painter.fillRect(ctx.rectInnerChart, Color(100, 100, 200, 255 * 0.2));

            // x
            for (int i = 0; i < xAxisPos.size(); i++) {
                const auto& x = xAxisPos[i];
                painter.drawLine(
                    Line(x, ctx.rectChart.top(), x, ctx.rectChart.bottom()),
                    ctx.props.axisGridStyle
                );
            }
            for (int i = 0; i < xAxisDate.size(); i++) {
                painter.drawString(
                    xAxisRect[i],
                    xAxisDate[i],
                    ctx.props.xAxisTextFont
                );
            }
            painter.drawLine(
                Line(ctx.rectXAxis.topLeft(), ctx.rectXAxis.topRight()),
                ctx.props.axisLineStyle
            );

            // y
            for (int i = 0; i < yAxisPos.size(); i++) {
                const auto& y = yAxisPos[i];
                painter.drawLine(
                    Line(ctx.rectChart.left(), y, ctx.rectChart.right(), y),
                    ctx.props.axisGridStyle
                );
            }
            for (int i = 0; i < ylAxisPrice.size(); i++) {
                painter.drawString(
                    ylAxisRect[i],
                    ylAxisPrice[i],
                    ctx.props.ylAxisTextFont
                );
            }
            for (int i = 0; i < yrAxisPrice.size(); i++) {
                painter.drawString(
                    yrAxisRect[i],
                    yrAxisPrice[i],
                    ctx.props.yrAxisTextFont
                );
            }
            painter.drawLine(
                Line(ctx.rectYLAxis.topRight(), ctx.rectYLAxis.bottomRight()),
                ctx.props.axisLineStyle
            );
            painter.drawLine(
                Line(ctx.rectYRAxis.topLeft(), ctx.rectYRAxis.bottomLeft()),
                ctx.props.axisLineStyle
            );
        }

    private:
        // x
        std::vector<Real> xAxisPos;
        std::vector<Rect> xAxisRect;
        std::vector<std::string> xAxisDate;
        // y
        std::vector<Real> yAxisPos;
        std::vector<Rect> ylAxisRect;
        std::vector<Rect> yrAxisRect;
        std::vector<std::string> ylAxisPrice;
        std::vector<std::string> yrAxisPrice;
    };
}
