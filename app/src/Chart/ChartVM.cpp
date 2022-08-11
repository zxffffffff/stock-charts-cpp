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

void ChartVM::setDrawingType(EnKLineType type)
{
    auto& ctx = *m_context;
    ctx.props.klineType = type;
    calcContext();
}

void ChartVM::setCorrdinate(EnCoordinateType type)
{
    auto& ctx = *m_context;
    ctx.props.coordinateType = type;
    calcContext();
}

void ChartVM::setYLWidth(Real i)
{
    auto& ctx = *m_context;
    ctx.props.ylAxisWidth = i;
    calcContext();
}

void ChartVM::setYRWidth(Real i)
{
    auto& ctx = *m_context;
    ctx.props.yrAxisWidth = i;
    calcContext();
}

void ChartVM::setXHeight(Real i)
{
    auto& ctx = *m_context;
    ctx.props.xAxisHeight = i;
    calcContext();
}

void ChartVM::setPaddingLeft(Real i)
{
    auto& ctx = *m_context;
    ctx.props.leftPadding = i;
    calcContext();
}

void ChartVM::setPaddingTop(Real i)
{
    auto& ctx = *m_context;
    ctx.props.topPadding = i;
    calcContext();
}

void ChartVM::setPaddingRight(Real i)
{
    auto& ctx = *m_context;
    ctx.props.rightPadding= i;
    calcContext();
}

void ChartVM::setPaddingBottom(Real i)
{
    auto& ctx = *m_context;
    ctx.props.btmPadding = i;
    calcContext();
}

void ChartVM::setNodeStickWidth(Real nodeWidth, Real stickWidth)
{
    auto& ctx = *m_context;
    ctx.props.nodeWidth = nodeWidth;
    ctx.props.stickWidth = stickWidth;
    calcContext();
}

void ChartVM::calcContext()
{
    const auto& stockCore = m_model->getStockCore();
    const auto& plugins = m_model->getPlugins();
    auto& ctx = *m_context;

    // [0] content
    ctx.rectXAxis.set(
        ctx.rectView.left(),
        ctx.rectView.bottom() - ctx.props.xAxisHeight, 
        ctx.rectView.width(),
        ctx.props.xAxisHeight
    );
    ctx.rectYLAxis.set(
        ctx.rectView.left(), 
        ctx.rectView.top(), 
        ctx.props.ylAxisWidth,
        ctx.rectView.height()
    );
    ctx.rectYRAxis.set(
        ctx.rectView.right() - ctx.props.yrAxisWidth,
        ctx.rectView.top(),
        ctx.props.yrAxisWidth,
        ctx.rectView.height()
    );
    ctx.rectChart.set(
        ctx.rectView.left() + ctx.props.ylAxisWidth,
        ctx.rectView.top(),
        ctx.rectView.width() - ctx.props.ylAxisWidth - ctx.props.yrAxisWidth,
        ctx.rectView.height() - ctx.props.xAxisHeight
    );
    ctx.rectInnerChart.set(
        ctx.rectChart.left() + ctx.props.leftPadding,
        ctx.rectChart.top() + ctx.props.topPadding,
        ctx.rectChart.width() - ctx.props.leftPadding - ctx.props.rightPadding,
        ctx.rectChart.height() - ctx.props.topPadding - ctx.props.btmPadding
    );

    // invalid size
    if (!ctx.rectInnerChart.valid()) {
        // assert(false);
        return;
    }

    // [1] x
    const int stockCnt = stockCore->getSize();
    const Real stockWidth = stockCnt * ctx.props.nodeWidth;
    const Real viewWidth = ctx.rectInnerChart.width();
    if (stockWidth <= viewWidth) {
        ctx.viewCount = stockCnt;
        ctx.endIndex = stockCnt;
    }
    else {
        ctx.viewCount = std::floor(viewWidth / ctx.props.nodeWidth);
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
        plugin->onPaint(m_context, painter);
        painter.restore();
    }
}
