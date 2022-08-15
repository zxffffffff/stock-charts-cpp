/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../Core/StockCore.h"
#include "../Core/DataBinding.h"
#include "Context/ChartContext.h"

namespace StockCharts
{
    constexpr inline char ID_ChartContextChanged[] = "ID_ChartContextChanged";

    class ChartModel;
    class ChartVM : public DataBinding
    {
    public:
        ChartVM(std::shared_ptr<ChartModel> model)
            : m_model(model)
            , m_context(std::make_shared<ChartContext>())
        {
            bind(m_model.get());
        }
        virtual ~ChartVM() = default;

        std::shared_ptr<const ChartContext> getContext() const
        {
            return m_context;
        }

        // [0]
        void setProps(const ChartProps& props)
        {
            auto& ctx = *m_context;
            if (ctx.props == props)
                return;
            ctx.props = props;
            calcContext();
            fire(ID_ChartContextChanged);
        }
        void setContext(const ChartContext& context)
        {
            auto& ctx = *m_context;
            if (ctx == context)
                return;
            ctx = context;
            calcContext();
            fire(ID_ChartContextChanged);
        }

        // [1]
        void calcContext()
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

        void calcPlugins()
        {
            const auto& plugins = m_model->getPlugins();

            for (const auto& plugin : plugins) {
                plugin->onContextChanged(m_context);
            }
        }

        // [2]
        void OnPaint(Painter& painter)
        {
            paintPlugins(painter);
        }

        void OnResize(const Rect& rect)
        {
            auto& ctx = *m_context;
            if (ctx.rectView == rect)
                return;
            ctx.rectView = rect;

            calcContext();
            fire(ID_ChartContextChanged);
        }

        void OnMouseMove(const Point& point)
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

        void OnMouseLeave()
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

        void OnScrollX(int step)
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

        void OnWheelY(int step)
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

        void paintPlugins(Painter& painter)
        {
            const auto& plugins = m_model->getPlugins();

            for (const auto& plugin : plugins) {
                painter.save();
                plugin->onPaint(m_context, painter);
                painter.restore();
            }
        }

        // [3]
        void SyncViewCount(int viewCount, int beginIndex, int endIndex)
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

        void SyncMouseMove(int hoverIndex, Number hoverPrice)
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

    private:
        std::shared_ptr<ChartModel> m_model;
        std::shared_ptr<ChartContext> m_context;
    };
}

