/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include "PluginBG.h"
#include "../Context/ChartCoordinate.h"
#include "../../Core/Utils.h"

using namespace StockCharts;

PluginBG::PluginBG(std::shared_ptr<const StockCore> stockCore)
    : ChartPlugin(stockCore)
{
}

void PluginBG::onContextChanged(std::shared_ptr<const ChartContext> context)
{
    auto& ctx = *context;
    ChartCoordinate coordinate(context);

    // x
    xAxis.resize(ctx.viewCount);
    for (int i = 0; i < ctx.viewCount; i++) {
        int index = ctx.beginIndex + i;
        xAxis[i] = coordinate.index2pos(index);
    }

    //y
    const Real stepHeight = ctx.props.yAxisGridStep;
    const Real halfStepHeight = stepHeight / 2;

    auto minmax = m_stockCore->getMinMax(ctx.beginIndex, ctx.endIndex);
    Number basePrice = m_stockCore->open.safeAt(ctx.beginIndex);
    Real basePos = coordinate.price2pos(basePrice);
    yAxisPos.clear();
    yAxisPrice.clear();
    yAxisPos.push_back(basePos);
    yAxisPrice.push_back(basePrice);
    for (Real y = basePos - stepHeight; y >= ctx.rectChart.top() + halfStepHeight; y -= stepHeight) {
        yAxisPos.insert(yAxisPos.begin(), y);
        yAxisPrice.insert(yAxisPrice.begin(), coordinate.pos2price(y));
    }
    for (Real y = basePos + stepHeight; y < ctx.rectChart.bottom() - halfStepHeight; y += stepHeight) {
        yAxisPos.push_back(y);
        yAxisPrice.push_back(coordinate.pos2price(y));
    }
}

void PluginBG::onPaint(std::shared_ptr<const ChartContext> context, Painter& painter)
{
    auto& ctx = *context;

    painter.fillRect(ctx.rectView, Color(255, 255, 255, 255 * 0.2));
    painter.fillRect(ctx.rectXAxis, Color(150, 100, 100, 255 * 0.2));
    painter.fillRect(ctx.rectYLAxis, Color(100, 150, 100, 255 * 0.2));
    painter.fillRect(ctx.rectYRAxis, Color(100, 150, 100, 255 * 0.2));
    painter.fillRect(ctx.rectChart, Color(100, 100, 150, 255 * 0.2));
    painter.fillRect(ctx.rectInnerChart, Color(100, 100, 200, 255 * 0.2));

    // x
    painter.drawLine(
        Line(ctx.rectXAxis.topLeft(), ctx.rectXAxis.topRight()),
        ctx.props.axisLineStyle
    );

    // y
    const Real stepHeight = ctx.props.yAxisGridStep;
    const Real halfStepHeight = stepHeight / 2;
    for (int i = 0; i < yAxisPos.size(); i++) {
        const auto& y = yAxisPos[i];
        painter.drawLine(
            Line(ctx.rectChart.left(), y, ctx.rectChart.right(), y),
            ctx.props.axisGridStyle
        );
    }

    // yl
    painter.drawLine(
        Line(ctx.rectYLAxis.topRight(), ctx.rectYLAxis.bottomRight()),
        ctx.props.axisLineStyle
    );
    for (int i = 0; i < yAxisPos.size(); i++) {
        const auto& y = yAxisPos[i];
        const auto& price = yAxisPrice[i];
        painter.drawString(
            Rect(ctx.rectYLAxis.left() + 1, y - halfStepHeight, ctx.rectYLAxis.width() - 2, stepHeight),
            NumberUtils::toString(price, ctx.props.precision),
            ctx.props.ylAxisTextFont
        );
    }

    // yr
    painter.drawLine(
        Line(ctx.rectYRAxis.topLeft(), ctx.rectYRAxis.bottomLeft()),
        ctx.props.axisLineStyle
    );
    for (int i = 0; i < yAxisPos.size(); i++) {
        const auto& y = yAxisPos[i];
        const auto& price = yAxisPrice[i];
        painter.drawString(
            Rect(ctx.rectYRAxis.left() + 1, y - halfStepHeight, ctx.rectYRAxis.width() - 2, stepHeight),
            NumberUtils::toString(price, ctx.props.precision),
            ctx.props.yrAxisTextFont
        );
    }
}
