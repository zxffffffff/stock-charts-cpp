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
#include "Context/ChartTitleContext.h"

namespace StockCharts
{
    constexpr inline char ID_ChartContextChanged[] = "ID_ChartContextChanged";
    constexpr inline char ID_OnResize[] = "ID_OnResize";
    constexpr inline char ID_OnMouseMove[] = "ID_OnMouseMove";
    constexpr inline char ID_OnMouseLeave[] = "ID_OnMouseLeave";
    constexpr inline char ID_OnScrollX[] = "ID_OnScrollX";
    constexpr inline char ID_OnWheelY[] = "ID_OnWheelY";
    constexpr inline char ID_OnDBClick[] = "ID_OnDBClick";

    class ChartModel;
    class ChartVM : public DataBinding
    {
    public:
        ChartVM(std::shared_ptr<ChartModel> model)
            : m_model(model)
            , m_context(std::make_shared<ChartContext>())
            , m_title(std::make_shared<ChartTitleContext>())
        {
            bind(m_model.get());
        }
        virtual ~ChartVM() = default;

        virtual void on(DataBinding* sender, const std::string& id) override
        {
            if (m_model.get() == sender) {
                if (id == ID_StockCoreChanged || id == ID_ChartPropsChanged || id == ID_ChartPluginChanged)
                    calcContext();
            }
            else {
                auto other = static_cast<ChartVM*>(sender);
                const auto& ctx = *other->getContext();

                if (id == ID_OnMouseMove || id == ID_OnMouseLeave)
                    syncMouseMove(ctx.hoverIndex, ctx.hoverPrice);
                else if (id == ID_OnScrollX || id == ID_OnWheelY)
                    syncViewCount(ctx.viewCount, ctx.beginIndex, ctx.endIndex, ctx.nodeWidth, ctx.stickWidth);
                else if (id == ID_OnDBClick)
                    syncDBClick(ctx.crossLineVisible);
            }
        }

        std::shared_ptr<const ChartContext> getContext() const
        {
            return m_context;
        }

        // [1]
        void calcContext()
        {
            const auto& stockCore = m_model->getStockCore();
            const auto& plugins = m_model->getPlugins();
            const auto& props = *m_model->getProps();
            auto& ctx = *m_context;

            // [0] content
            ctx.rectXAxis.set(
                ctx.rectView.left() + props.ylAxisWidth,
                ctx.rectView.bottom() - props.xAxisHeight,
                ctx.rectView.width() - props.ylAxisWidth - props.yrAxisWidth,
                props.xAxisHeight
            );
            ctx.rectYLAxis.set(
                ctx.rectView.left(),
                ctx.rectView.top(),
                props.ylAxisWidth,
                ctx.rectView.height()
            );
            ctx.rectYRAxis.set(
                ctx.rectView.right() - props.yrAxisWidth,
                ctx.rectView.top(),
                props.yrAxisWidth,
                ctx.rectView.height()
            );
            ctx.rectChart.set(
                ctx.rectView.left() + props.ylAxisWidth,
                ctx.rectView.top(),
                ctx.rectView.width() - props.ylAxisWidth - props.yrAxisWidth,
                ctx.rectView.height() - props.xAxisHeight
            );
            ctx.rectInnerChart.set(
                ctx.rectChart.left() + props.paddingLeft,
                ctx.rectChart.top() + props.paddingTop,
                ctx.rectChart.width() - props.paddingLeft - props.paddingRight,
                ctx.rectChart.height() - props.paddingTop - props.paddingBottom
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
                if (ctx.viewCount != stockCnt) {
                    ctx.viewCount = stockCnt;
                    ctx.endIndex = stockCnt;
                    ctx.beginIndex = ctx.endIndex - ctx.viewCount;
                }
            }
            else {
                int viewCount = std::floor(viewWidth / ctx.nodeWidth);
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
            fire(ID_ChartContextChanged);
        }

        void calcPlugins()
        {
            const auto& plugins = m_model->getPlugins();

            for (const auto& plugin : plugins) {
                plugin->onContextChanged(m_context);
            }
        }

        // [2]
        void onPaint(Painter& painter)
        {
            paintPlugins(painter);
            paintTitle(painter);
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

        void paintTitle(Painter& painter)
        {
            const auto& plugins = m_model->getPlugins();
        }

        void onResize(const Rect& rect)
        {
            auto& ctx = *m_context;
            const auto& props = *m_model->getProps();

            const Rect view(
                rect.left() + props.MarginLeft,
                rect.top() + props.MarginTop,
                rect.width() - props.MarginLeft - props.MarginRight,
                rect.height() - props.MarginTop - props.MarginBottom
            );
            if (ctx.rectView == view)
                return;
            ctx.rectView = view;

            calcContext();
            fire(ID_OnResize);
        }

        void onMouseMove(const Point& point)
        {
            auto& ctx = *m_context;
            if (ctx.pointHover == point)
                return;

            auto& plugins = m_model->getPlugins();
            ChartCoordinate coordinate(m_context, m_model->getProps());

            // mouse event
            ctx.hoverType = MouseHoverType::Normal;
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
            fire(ID_OnMouseMove);
        }

        void onMouseLeave()
        {
            auto& ctx = *m_context;
            auto& plugins = m_model->getPlugins();
            ChartCoordinate coordinate(m_context, m_model->getProps());

            // mouse event
            ctx.hoverType = MouseHoverType::Normal;
            ctx.pointHover = Point();
            ctx.hoverIndex = -1;
            ctx.hoverPrice = NumberNull;

            for (const auto& plugin : plugins) {
                plugin->onMouseLeave(m_context);
            }
            fire(ID_OnMouseLeave);
        }

        void onScrollX(int step)
        {
            if (step == 0)
                return;

            const auto& stockCore = m_model->getStockCore();
            const auto& props = *m_model->getProps();
            auto& ctx = *m_context;

            // [1] x
            const int stockCnt = stockCore->getSize();
            ctx.beginIndex = std::min(std::max(0, ctx.beginIndex + step * props.scrollXStep), stockCnt - 1);
            ctx.endIndex = std::min(ctx.beginIndex + ctx.viewCount, stockCnt);

            calcContext();
            fire(ID_OnScrollX);
        }

        void onWheelY(int step)
        {
            if (step == 0)
                return;

            const auto& stockCore = m_model->getStockCore();
            const auto& props = *m_model->getProps();
            auto& ctx = *m_context;

            int stepWidth = 2 * std::abs(step);
            int nodeWidth = ctx.nodeWidth + step * props.wheelYStep;
            if (nodeWidth % 2 != 1)
                nodeWidth -= 1;
            ctx.nodeWidth = std::min(std::max(Real(1), Real(nodeWidth)), Real(99));
            int stickWidth = std::round(ctx.nodeWidth * Real(0.75));
            if (stickWidth % 2 != 1)
                stickWidth -= 1;
            ctx.stickWidth = std::min(std::max(Real(1), Real(stickWidth)), Real(99));

            // [1] x
            const int stockCnt = stockCore->getSize();
            const Real stockWidth = stockCnt * ctx.nodeWidth;
            const Real viewWidth = ctx.rectInnerChart.width();
            if (stockWidth <= viewWidth) {
                ctx.viewCount = stockCnt;
                ctx.endIndex = stockCnt;
                ctx.beginIndex = ctx.endIndex - ctx.viewCount;
            }
            else {
                const int viewCount = std::floor(viewWidth / ctx.nodeWidth);
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
            fire(ID_OnWheelY);
        }

        void onDBClick(const Point& point)
        {
            auto& ctx = *m_context;
            ctx.crossLineVisible = !ctx.crossLineVisible;

            calcContext();
            fire(ID_OnDBClick);
        }

        // [3]
        void setSyncOther(ChartVM* other)
        {
            bind(other);
        }

        void syncViewCount(int viewCount, int beginIndex, int endIndex, Real nodeWidth, Real stickWidth)
        {
            auto& ctx = *m_context;
            if (ctx.viewCount == viewCount &&
                ctx.beginIndex == beginIndex &&
                ctx.endIndex == endIndex &&
                ctx.nodeWidth == nodeWidth &&
                ctx.stickWidth == stickWidth)
                return;

            // [1] x
            ctx.viewCount = viewCount;
            ctx.beginIndex = beginIndex;
            ctx.endIndex = endIndex;
            ctx.nodeWidth = nodeWidth;
            ctx.stickWidth = stickWidth;

            calcContext();
        }

        void syncMouseMove(int hoverIndex, Number hoverPrice)
        {
            auto& ctx = *m_context;
            if (ctx.hoverIndex == hoverIndex && ctx.hoverPrice == hoverPrice)
                return;

            auto& plugins = m_model->getPlugins();
            ChartCoordinate coordinate(m_context, m_model->getProps());

            // mouse event
            ctx.hoverType = MouseHoverType::SyncOther;
            ctx.pointHover = Point(
                coordinate.index2pos(hoverIndex),
                coordinate.price2pos(hoverPrice)
            );
            ctx.hoverIndex = hoverIndex;
            ctx.hoverPrice = hoverPrice;

            for (const auto& plugin : plugins)
                plugin->onMouseMove(m_context);
            fire(ID_ChartContextChanged);
        }

        void syncDBClick(bool crossLineVisible)
        {
            auto& ctx = *m_context;
            if (ctx.crossLineVisible == crossLineVisible)
                return;
            ctx.crossLineVisible = crossLineVisible;
            calcContext();
        }

    private:
        std::shared_ptr<ChartModel> m_model;
        std::shared_ptr<ChartContext> m_context;
        std::shared_ptr<ChartTitleContext> m_title;
    };
}

