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
    bind(m_model.get());
}

std::shared_ptr<const ChartContext> ChartVM::getContext() const
{
    return m_context;
}

void ChartVM::setDrawingType(EnKLineType type)
{
    auto& ctx = *m_context;
    if (ctx.props.klineType == type)
        return;
    ctx.props.klineType = type;

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::setCorrdinate(EnCoordinateType type)
{
    auto& ctx = *m_context;
    if (ctx.props.coordinateType == type)
        return;
    ctx.props.coordinateType = type;

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::setYLWidth(Real i)
{
    auto& ctx = *m_context;
    if (ctx.props.ylAxisWidth == i)
        return;
    ctx.props.ylAxisWidth = i;

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::setYRWidth(Real i)
{
    auto& ctx = *m_context;
    if (ctx.props.yrAxisWidth == i)
        return;
    ctx.props.yrAxisWidth = i;

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::setXHeight(Real i)
{
    auto& ctx = *m_context;
    if (ctx.props.xAxisHeight == i)
        return;
    ctx.props.xAxisHeight = i;

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::setPaddingLeft(Real i)
{
    auto& ctx = *m_context;
    if (ctx.props.leftPadding == i)
        return;
    ctx.props.leftPadding = i;

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::setPaddingTop(Real i)
{
    auto& ctx = *m_context;
    if (ctx.props.topPadding == i)
        return;
    ctx.props.topPadding = i;

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::setPaddingRight(Real i)
{
    auto& ctx = *m_context;
    if (ctx.props.rightPadding == i)
        return;
    ctx.props.rightPadding= i;

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::setPaddingBottom(Real i)
{
    auto& ctx = *m_context;
    if (ctx.props.btmPadding == i)
        return;
    ctx.props.btmPadding = i;

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::setNodeWidth(Real nodeWidth)
{
    auto& ctx = *m_context;
    if (ctx.props.nodeWidth == nodeWidth)
        return;
    ctx.props.nodeWidth = nodeWidth;

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::setStickWidth(Real stickWidth)
{
    auto& ctx = *m_context;
    if (ctx.props.stickWidth == stickWidth)
        return;
    ctx.props.stickWidth = stickWidth;

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::calcContext()
{
    const auto& stockCore = m_model->getStockCore();
    const auto& plugins = m_model->getPlugins();
    auto& ctx = *m_context;

    // [0] content
    ctx.rectXAxis.set(
        ctx.rectView.left() + ctx.props.ylAxisWidth,
        ctx.rectView.bottom() - ctx.props.xAxisHeight, 
        ctx.rectView.width() - ctx.props.ylAxisWidth - ctx.props.yrAxisWidth,
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
        if (ctx.viewCount != stockCnt) {
            ctx.viewCount = stockCnt;
            ctx.endIndex = stockCnt;
            ctx.beginIndex = ctx.endIndex - ctx.viewCount;
        }
    }
    else {
        int viewCount = std::floor(viewWidth / ctx.props.nodeWidth);
        if (ctx.viewCount != viewCount) {
            ctx.viewCount = viewCount;
            ctx.endIndex = stockCnt;
            ctx.beginIndex = ctx.endIndex - ctx.viewCount;
        }
    }
    assert(ctx.viewCount >= 0);
    assert(ctx.endIndex >= 0);
    assert(ctx.beginIndex >= 0);
    assert(ctx.beginIndex <= ctx.endIndex);
    assert(ctx.endIndex <= stockCnt);
    assert(ctx.endIndex - ctx.beginIndex <= ctx.viewCount);

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
    if (ctx.rectView == rect)
        return;
    ctx.rectView = rect;

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::OnPaint(Painter& painter)
{
    paintPlugins(painter);
}

void ChartVM::OnMouseMove(const Point& point)
{
    auto& ctx = *m_context;
    if (ctx.pointHover == point)
        return;

    auto& plugins = m_model->getPlugins();
    ChartCoordinate coordinate(m_context);

    // mouse event
    ctx.syncHover = false;
    ctx.pointHover = point;
    if (ctx.rectInnerChart.contains(ctx.pointHover))
    {
        ctx.hoverIndex = coordinate.pos2index(point.x);
        if (ctx.hoverIndex > ctx.endIndex - 1)
            ctx.hoverIndex = ctx.endIndex - 1;
        ctx.hoverPrice = coordinate.pos2price(point.y);
    }
    else {
        ctx.hoverIndex = -1;
        ctx.hoverPrice = NumberNull;
    }

    for (const auto& plugin : plugins) {
        plugin->onMouseMove(m_context);
    }
    fire(ID_ChartContextChanged);
}

void ChartVM::OnMouseLeave()
{
    auto& ctx = *m_context;
    auto& plugins = m_model->getPlugins();
    ChartCoordinate coordinate(m_context);

    // mouse event
    ctx.syncHover = false;
    ctx.pointHover = Point();
    ctx.hoverIndex = -1;
    ctx.hoverPrice = NumberNull;

    for (const auto& plugin : plugins) {
        plugin->onMouseLeave(m_context);
    }
    fire(ID_ChartContextChanged);
}

void ChartVM::OnScrollX(int step)
{
    if (step == 0)
        return;

    const auto& stockCore = m_model->getStockCore();
    auto& ctx = *m_context;

    // [1] x
    const int stockCnt = stockCore->getSize();
    if (step > 0) {
        if (ctx.beginIndex == stockCnt - 1)
            return;
        ctx.beginIndex += 1;
    }
    else {
        if (ctx.beginIndex == 0)
            return;
        ctx.beginIndex -= 1;
    }
    ctx.endIndex = std::min(ctx.beginIndex + ctx.viewCount, stockCnt);

    calcContext();
    fire(ID_ChartContextChanged);
}

void ChartVM::OnWheelY(int step)
{
    if (step == 0)
        return;

    const auto& stockCore = m_model->getStockCore();
    auto& ctx = *m_context;

    int stepWidth = 2 * std::abs(step);
    if (step > 0) {
        ctx.props.nodeWidth = std::min(ctx.props.nodeWidth + stepWidth, Real(99));
        ctx.props.stickWidth = std::min(ctx.props.stickWidth + stepWidth, Real(99));
    }
    else {
        ctx.props.nodeWidth = std::max(ctx.props.nodeWidth - stepWidth, Real(1));
        ctx.props.stickWidth = std::max(ctx.props.stickWidth - stepWidth, Real(1));
    }

    // [1] x
    const int stockCnt = stockCore->getSize();
    const Real stockWidth = stockCnt * ctx.props.nodeWidth;
    const Real viewWidth = ctx.rectInnerChart.width();
    if (stockWidth <= viewWidth) {
        ctx.viewCount = stockCnt;
        ctx.endIndex = stockCnt;
        ctx.beginIndex = ctx.endIndex - ctx.viewCount;
    }
    else {
        const int viewCount = std::floor(viewWidth / ctx.props.nodeWidth);
        if (ctx.hoverIndex < 0) {
            ctx.viewCount = viewCount;
            ctx.endIndex = stockCnt;
            ctx.beginIndex = ctx.endIndex - ctx.viewCount;
        }
        else {
            const double percent = double(ctx.hoverIndex - ctx.beginIndex) / ctx.viewCount;
            ctx.viewCount = viewCount;
            ctx.beginIndex = ctx.hoverIndex - std::round(percent * viewCount);
            if (ctx.beginIndex >= stockCnt)
                ctx.beginIndex = stockCnt - 1;
            if (ctx.beginIndex < 0)
                ctx.beginIndex = 0;
            ctx.endIndex = std::min(ctx.beginIndex + ctx.viewCount, stockCnt);
        }
    }

    calcContext();
    fire(ID_ChartContextChanged);
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

void ChartVM::SyncViewCount(int viewCount, int beginIndex, int endIndex)
{
    auto& ctx = *m_context;
    if (ctx.beginIndex == beginIndex && ctx.endIndex == endIndex)
        return;
    const auto& stockCore = m_model->getStockCore();

    // [1] x
    const int stockCnt = stockCore->getSize();
    if (ctx.viewCount == viewCount) {
        ctx.beginIndex = beginIndex;
        ctx.endIndex = endIndex;
    }
    else {
        // todo
    }

    calcContext();
    // fire(ID_ChartContextChanged);
}

void ChartVM::SyncMouseMove(int hoverIndex, Number hoverPrice)
{
    auto& ctx = *m_context;
    if (ctx.hoverIndex == hoverIndex && ctx.hoverPrice == hoverPrice)
        return;

    auto& plugins = m_model->getPlugins();
    ChartCoordinate coordinate(m_context);

    // mouse event
    ctx.syncHover = true;
    ctx.pointHover = Point(
        coordinate.index2pos(hoverIndex),
        coordinate.price2pos(hoverPrice)
    );
    ctx.hoverIndex = hoverIndex;
    ctx.hoverPrice = hoverPrice;

    for (const auto& plugin : plugins) {
        plugin->onMouseMove(m_context);
    }
    // fire(ID_ChartContextChanged);
}
