/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "ChartVM.h"
#include "ChartModel.h"
#include "Context/ChartCoordinate.h"
#include "Plugins/ChartPlugin.h"
#include <cassert>

using namespace StockCharts;

ChartVM::ChartVM(std::shared_ptr<ChartModel> model)
    : m_model(model)
    , m_context(std::make_shared<ChartContext>())
{
}

void ChartVM::calcContext()
{
    const auto& stockCore = m_model->getStockCore();
    const auto& plugins = m_model->getPlugins();
    auto& ctx = *m_context;

    // [0] content
    ctx.rectXAxis.set(ctx.rectView.left(), ctx.rectView.height() - ctx.props.xAxisHeight, ctx.rectView.width(), ctx.props.xAxisHeight);
    ctx.rectYAxis.set(ctx.rectView.width() - ctx.props.yAxisWidth, ctx.rectView.top(), ctx.props.yAxisWidth, ctx.rectView.height());
    ctx.rectChart.set(
        ctx.rectView.left(),
        ctx.rectView.top(),
        ctx.rectView.width() - ctx.props.yAxisWidth,
        ctx.rectView.height() - ctx.props.xAxisHeight
    );
    ctx.rectInnerChart.set(
        ctx.props.leftPadding,
        ctx.props.topPadding,
        ctx.rectView.width() - ctx.props.yAxisWidth - ctx.props.leftPadding - ctx.props.rightPadding,
        ctx.rectView.height() - ctx.props.xAxisHeight - ctx.props.topPadding - ctx.props.btmPadding
    );

    // invalid size
    if (!ctx.rectInnerChart.valid()) {
        // assert(false);
        return;
    }

    // [1] x
    const int stockCnt = stockCore->getSize();
    const Real stockWidth = stockCnt * ctx.nodeWidth;
    const Real viewWidth = ctx.rectInnerChart.width();
    if (stockWidth <= viewWidth) {
        ctx.viewCount = stockCnt;
        ctx.endIndex = stockCnt;
    }
    else {
        ctx.viewCount = std::floor(viewWidth / ctx.nodeWidth);
        if (ctx.endIndex < 0 || ctx.endIndex > stockCnt)
            ctx.endIndex = ctx.viewCount;
    }
    ctx.beginIndex = ctx.endIndex - ctx.viewCount;

    // [2] y
    ctx.minPrice = NumberNull;
    ctx.maxPrice = NumberNull;
    for (const auto& plugin : plugins) {
        auto minmax = plugin->getMinMax(ctx.beginIndex, ctx.endIndex);
        ctx.minPrice = NumberCore::min(minmax.first, ctx.minPrice);
        ctx.maxPrice = NumberCore::max(minmax.second, ctx.maxPrice);
    }

    calcPlugins();
}

void ChartVM::calcPlugins()
{
    const auto& plugins = m_model->getPlugins();

    for (const auto& plugin : plugins) {
        plugin->onContextChanged(m_context);
    }
}

void ChartVM::OnResize(const Rect& rect)
{
    auto& ctx = *m_context;
    ctx.rectView = rect;
    calcContext();
}

void ChartVM::OnPaint(Painter& painter)
{
    paintPlugins(painter);
}

void ChartVM::OnMouseMove(const Point& point)
{
    auto& ctx = *m_context;
    auto& plugins = m_model->getPlugins();
    ChartCoordinate coordinate(m_context);

    // mouse event
    ctx.pointHover = point;
    ctx.hoverIndex = coordinate.pos2index(point.x);
    if (ctx.hoverIndex > ctx.endIndex - 1)
        ctx.hoverIndex = ctx.endIndex - 1;
    ctx.hoverPrice = coordinate.pos2price(point.y);

    for (const auto& plugin : plugins) {
        plugin->onMouseMove(m_context);
    }
}

void ChartVM::OnMouseLeave()
{
    auto& ctx = *m_context;
    auto& plugins = m_model->getPlugins();
    ChartCoordinate coordinate(m_context);

    // mouse event
    ctx.pointHover = Point();
    ctx.hoverIndex = -1;
    ctx.hoverPrice = NumberNull;

    for (const auto& plugin : plugins) {
        plugin->onMouseLeave(m_context);
    }
}

void ChartVM::paintPlugins(Painter& painter)
{
    const auto& plugins = m_model->getPlugins();

    for (const auto& plugin : plugins) {
        painter.save();
        plugin->onPaint(painter);
        painter.restore();
    }
}
