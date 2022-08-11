/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include "PluginCrossLine.h"
#include "../Context/ChartCoordinate.h"
#include "../../Core/Utils.h"

using namespace StockCharts;

PluginCrossLine::PluginCrossLine(std::shared_ptr<const StockCore> stockCore)
    : ChartPlugin(stockCore)
{
}

void PluginCrossLine::onContextChanged(std::shared_ptr<const ChartContext> context)
{
    onMouseMove(context);
}

void PluginCrossLine::onMouseMove(std::shared_ptr<const ChartContext> context)
{
    const auto& ctx = *context;
    ChartCoordinate coordinate(context);

    Real x = coordinate.index2pos(ctx.hoverIndex);
    const Real stepHeight = ctx.props.yAxisGridStep;
    const Real halfStepHeight = stepHeight / 2;

    if (context->rectInnerChart.contains(context->pointHover)) {
        crossLine[0].set(
            x,
            ctx.rectChart.top() + 1,
            x,
            ctx.rectChart.bottom() - 1
        );
        crossLine[1].set(
            ctx.rectChart.left() + 1,
            ctx.pointHover.y,
            ctx.rectChart.right() - 1,
            ctx.pointHover.y
        );

        crossText[0].set(
            ctx.rectYLAxis.left() + 1,
            ctx.pointHover.y - halfStepHeight,
            ctx.rectYLAxis.width() - 2,
            stepHeight
        );
        crossText[1].set(
            ctx.rectYRAxis.left() + 1,
            ctx.pointHover.y - halfStepHeight,
            ctx.rectYRAxis.width() - 2,
            stepHeight
        );
    }
    else {
        crossLine = {};
        crossText = {};
    }
}

void PluginCrossLine::onPaint(std::shared_ptr<const ChartContext> context, Painter& painter)
{
    const auto& ctx = *context;

    // x
    painter.drawLine(crossLine[0], ctx.props.crossLineColor);

    // y
    painter.drawLine(crossLine[1], ctx.props.crossLineColor);

    // yl
    painter.fillRect(crossText[0], ctx.props.crossLineBGColor);
    painter.drawString(
        crossText[0],
        PaintDirection::CenterRight,
        NumberUtils::toString(ctx.hoverPrice, ctx.props.precision)
    );

    // yr
    painter.fillRect(crossText[1], ctx.props.crossLineBGColor);
    painter.drawString(
        crossText[1],
        PaintDirection::CenterLeft,
        NumberUtils::toString(ctx.hoverPrice, ctx.props.precision)
    );
}
