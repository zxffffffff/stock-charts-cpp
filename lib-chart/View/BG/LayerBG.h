/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../ChartLayer.h"

namespace StockCharts
{
    class LayerBG : public ChartLayer
    {
    public:
        LayerBG(std::shared_ptr<const ChartModel> model, std::shared_ptr<const ChartProps> props, std::shared_ptr<const ChartContext> context)
            : ChartLayer(model, props, context)
        {
        }
        virtual ~LayerBG() = default;

        virtual void onContextChanged() override
        {
            auto stockCore = *m_model->getStockCore();
            const auto& props = *m_props;
            auto& ctx = *m_context;
            ChartCoordinate coordinate(m_props, m_context);

            // x
            xAxisPos.clear();
            xAxisRect.clear();
            xAxisDate.clear();
            const Real textWidth = props.xAxisTextWidth;
            const Real textHalfWidth = textWidth / 2;
            switch (props.xAxisType)
            {
            case EnXAxisType::yyyyMM:
            default:
                for (int index = ctx.beginIndex; index < ctx.endIndex; index++) {
                    std::string dt = NumberUtils::toTimestamp(stockCore.timestamp[index], "%Y-%m");
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
            for (int i = 1; i < xAxisPos.size(); ) {
                auto& rect0 = xAxisRect[i - 1];
                auto& rect1 = xAxisRect[i];
                if (rect0.right() >= rect1.left()) {
                    xAxisPos.erase(xAxisPos.begin() + i);
                    xAxisRect.erase(xAxisRect.begin() + i);
                    xAxisDate.erase(xAxisDate.begin() + i);
                }
                else {
                    i++;
                }
            }

            //y
            yAxisPos.clear();
            ylAxisRect.clear();
            yrAxisRect.clear();
            ylAxisPrice.clear();
            yrAxisPrice.clear();
            const Real stepHeight = props.yAxisGridStepHeight;
            const Real stepHalfHeight = stepHeight / 2;
            for (Real y = ctx.rectChart.bottom() - props.yAxisGridStart; y >= ctx.rectChart.top() + stepHalfHeight; y -= stepHeight) {
                yAxisPos.push_back(y);
                ylAxisRect.push_back(Rect(ctx.rectYLAxis.left() + 1, y - stepHalfHeight, ctx.rectYLAxis.width() - 2, stepHeight));
                yrAxisRect.push_back(Rect(ctx.rectYRAxis.left() + 1, y - stepHalfHeight, ctx.rectYRAxis.width() - 2, stepHeight));
                ylAxisRect.back().moveInside(ctx.rectYLAxis);
                yrAxisRect.back().moveInside(ctx.rectYRAxis);
                ylAxisPrice.push_back(NumberUtils::toString(coordinate.pos2price(y), props.precision));
                yrAxisPrice.push_back(NumberUtils::toString(coordinate.pos2price(y), props.precision));
            }
        }

        virtual void onPaint(Painter& painter) override
        {
            const auto& props = *m_props;
            const auto& ctx = *m_context;

            painter.fillRect(ctx.rectView, props.colorViewBG);
            painter.fillRect(ctx.rectXAxis, props.colorXAxisBG);
            painter.fillRect(ctx.rectYLAxis, props.colorYLAxisBG);
            painter.fillRect(ctx.rectYRAxis, props.colorYRAxisBG);
            painter.fillRect(ctx.rectChart, props.colorChartBG);
            painter.fillRect(ctx.rectInnerChart, props.colorInnerChartBG);

            // x
            for (int i = 0; i < xAxisPos.size(); i++) {
                const auto& x = xAxisPos[i];
                painter.drawLine(
                    Line(x, ctx.rectChart.top(), x, ctx.rectChart.bottom()),
                    props.axisGridStyle
                );
            }
            for (int i = 0; i < xAxisDate.size(); i++) {
                painter.drawString(
                    xAxisRect[i],
                    xAxisDate[i],
                    props.xAxisTextFont
                );
            }
            painter.drawLine(
                Line(ctx.rectXAxis.topLeft(), ctx.rectXAxis.topRight()),
                props.axisLineStyle
            );

            // y
            for (int i = 0; i < yAxisPos.size(); i++) {
                const auto& y = yAxisPos[i];
                painter.drawLine(
                    Line(ctx.rectChart.left(), y, ctx.rectChart.right(), y),
                    props.axisGridStyle
                );
            }
            for (int i = 0; i < ylAxisPrice.size(); i++) {
                painter.drawString(
                    ylAxisRect[i],
                    ylAxisPrice[i],
                    props.ylAxisTextFont
                );
            }
            for (int i = 0; i < yrAxisPrice.size(); i++) {
                painter.drawString(
                    yrAxisRect[i],
                    yrAxisPrice[i],
                    props.yrAxisTextFont
                );
            }
            painter.drawLine(
                Line(ctx.rectYLAxis.topRight(), ctx.rectYLAxis.bottomRight()),
                props.axisLineStyle
            );
            painter.drawLine(
                Line(ctx.rectYRAxis.topLeft(), ctx.rectYRAxis.bottomLeft()),
                props.axisLineStyle
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

