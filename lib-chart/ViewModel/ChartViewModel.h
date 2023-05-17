/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "ChartLayer.h"

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
    class ChartViewModel : public DataBinding
    {
    public:
        ChartViewModel(std::shared_ptr<ChartModel> model)
            : m_model(model), m_props(std::make_shared<ChartProps>()), m_context(std::make_shared<ChartContext>())
        {
            listen(m_model.get());
        }
        virtual ~ChartViewModel() = default;

        virtual void on(DataBinding *sender, const std::string &id) override
        {
            if (m_model.get() == sender)
            {
                if (id == ID_StockCoreChanged || id == ID_ChartPluginChanged)
                    calcContext();
                return;
            }

            for (auto &layer : m_layers)
            {
                if (layer.get() == sender)
                {
                    if (id == ID_ChartLayerChanged)
                        calcContext();
                    return;
                }
            }

            {
                auto other = static_cast<ChartViewModel *>(sender);
                const auto &ctx = *other->getContext();

                if (id == ID_OnMouseMove || id == ID_OnMouseLeave)
                    syncMouseMove(ctx.hoverNormal.index, ctx.hoverNormal.price);
                else if (id == ID_OnScrollX || id == ID_OnWheelY)
                    syncViewCount(ctx.viewCount, ctx.beginIndex, ctx.endIndex, ctx.nodeWidth, ctx.stickWidth);
                else if (id == ID_OnDBClick)
                    syncDBClick(ctx.crossLineVisible);
            }
        }

        // [0]
        std::shared_ptr<const ChartProps> getProps() const
        {
            return m_props;
        }

        void setProps(const ChartProps &props, bool calcCtx = true)
        {
            *m_props = props;
            if (calcCtx)
                calcContext();
        }

        std::shared_ptr<const ChartContext> getContext() const
        {
            return m_context;
        }

        // [1]
        template <typename T = ChartLayer, typename... Args>
        std::shared_ptr<T> addLayer(Args &&...args)
        {
            assert(!getLayer<T>());
            auto layer = std::make_shared<T>(m_model, m_props, m_context, args...);
            m_layers.push_back(layer);
            listen(layer.get());
            return getLayer<T>();
        }

        template <typename T = ChartLayer>
        std::shared_ptr<T> getLayer()
        {
            for (auto &layer : m_layers)
            {
                // RTTI
                auto derived = std::dynamic_pointer_cast<T>(layer);
                if (derived)
                    return derived;
            }
            return nullptr;
        }

        const std::vector<std::shared_ptr<ChartLayer>> &getLayers()
        {
            return m_layers;
        }

        // [2]
        void calcContext()
        {
            const auto &stockCore = m_model->getStockCore();
            const auto &props = *m_props;
            auto &ctx = *m_context;

            // [0] content
            ctx.rectXAxis.set(
                ctx.rectView.left() + props.ylAxisWidth,
                ctx.rectView.bottom() - props.xAxisHeight,
                ctx.rectView.width() - props.ylAxisWidth - props.yrAxisWidth,
                props.xAxisHeight);
            ctx.rectYLAxis.set(
                ctx.rectView.left(),
                ctx.rectView.top(),
                props.ylAxisWidth,
                ctx.rectView.height());
            ctx.rectYRAxis.set(
                ctx.rectView.right() - props.yrAxisWidth,
                ctx.rectView.top(),
                props.yrAxisWidth,
                ctx.rectView.height());
            ctx.rectChart.set(
                ctx.rectView.left() + props.ylAxisWidth,
                ctx.rectView.top(),
                ctx.rectView.width() - props.ylAxisWidth - props.yrAxisWidth,
                ctx.rectView.height() - props.xAxisHeight);
            ctx.rectInnerChart.set(
                ctx.rectChart.left() + props.paddingLeft,
                ctx.rectChart.top() + props.paddingTop,
                ctx.rectChart.width() - props.paddingLeft - props.paddingRight,
                ctx.rectChart.height() - props.paddingTop - props.paddingBottom);

            // invalid size
            if (!ctx.rectInnerChart.valid())
            {
                // assert(false);
                return;
            }

            // [1] x
            const int stockCnt = stockCore->getSize();
            const Real stockWidth = stockCnt * ctx.nodeWidth;
            const Real viewWidth = ctx.rectInnerChart.width();
            switch (props.xCoordType)
            {
            case EnXCoordinateType::Fill:
                ctx.nodeWidth = viewWidth / (stockCnt + 1);
                ctx.viewCount = stockCnt;
                ctx.endIndex = stockCnt;
                ctx.beginIndex = 0;
                break;
            case EnXCoordinateType::Cover:
            default:
                if (stockWidth <= viewWidth)
                {
                    if (ctx.viewCount != stockCnt)
                    {
                        ctx.viewCount = stockCnt;
                        ctx.endIndex = stockCnt;
                        ctx.beginIndex = 0;
                    }
                }
                else
                {
                    int viewCount = std::floor(viewWidth / ctx.nodeWidth);
                    if (ctx.viewCount != viewCount)
                    {
                        ctx.viewCount = viewCount;
                        ctx.endIndex = stockCnt;
                        ctx.beginIndex = ctx.endIndex - ctx.viewCount;
                    }
                }
                break;
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
            for (const auto &layer : m_layers)
            {
                auto minmax = layer->getMinMax(ctx.beginIndex, ctx.endIndex);
                ctx.minPrice = NumberCore::min(minmax.first, ctx.minPrice);
                ctx.maxPrice = NumberCore::max(minmax.second, ctx.maxPrice);
            }

            calcLayers();
            fire(ID_ChartContextChanged);
        }

        void calcLayers()
        {
            for (const auto &layer : m_layers)
            {
                layer->onContextChanged();
            }
        }

        // [3]
        void onPaint(Painter &painter)
        {
            for (const auto &layer : m_layers)
            {
                painter.save();
                layer->onPaint(painter);
                painter.restore();
            }
        }

        void onResize(const Rect &rect)
        {
            const auto &props = *m_props;
            auto &ctx = *m_context;

            const Rect view(
                rect.left() + props.MarginLeft,
                rect.top() + props.MarginTop,
                rect.width() - props.MarginLeft - props.MarginRight,
                rect.height() - props.MarginTop - props.MarginBottom);
            if (ctx.rectView == view)
                return;
            ctx.rectView = view;

            calcContext();
            fire(ID_OnResize);
        }

        void onMouseMove(const Point &point)
        {
            const auto &props = *m_props;
            auto &ctx = *m_context;
            if (ctx.hoverNormal.point == point)
                return;

            ChartCoordinate coordinate(m_props, m_context);

            // mouse event
            ctx.hoverNormal.point = point;
            if (ctx.rectInnerChart.contains(ctx.hoverNormal.point))
            {
                ctx.hoverNormal.index = coordinate.pos2index(point.x);
                if (ctx.hoverNormal.index > ctx.endIndex - 1)
                    ctx.hoverNormal.index = ctx.endIndex - 1;
                ctx.hoverNormal.price = coordinate.pos2price(point.y);
            }
            else
            {
                ctx.hoverNormal.point.clear();
                ctx.hoverNormal.index = -1;
                ctx.hoverNormal.price = NumberNull;
            }

            for (const auto &layer : m_layers)
            {
                layer->onMouseMove();
            }
            fire(ID_OnMouseMove);
        }

        void onMouseLeave()
        {
            auto &ctx = *m_context;
            ChartCoordinate coordinate(m_props, m_context);

            // mouse event
            ctx.hoverNormal.point.clear();
            ctx.hoverNormal.index = -1;
            ctx.hoverNormal.price = NumberNull;

            for (const auto &layer : m_layers)
            {
                layer->onMouseLeave();
            }
            fire(ID_OnMouseLeave);
        }

        void onScrollX(int step)
        {
            if (step == 0)
                return;

            const auto &stockCore = m_model->getStockCore();
            const auto &props = *m_props;
            auto &ctx = *m_context;

            if (props.xCoordType == EnXCoordinateType::Fill)
                return;

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

            const auto &stockCore = m_model->getStockCore();
            const auto &props = *m_props;
            auto &ctx = *m_context;

            if (props.xCoordType == EnXCoordinateType::Fill)
                return;

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
            if (stockWidth <= viewWidth)
            {
                ctx.viewCount = stockCnt;
                ctx.endIndex = stockCnt;
                ctx.beginIndex = ctx.endIndex - ctx.viewCount;
            }
            else
            {
                const int viewCount = std::floor(viewWidth / ctx.nodeWidth);
                if (ctx.hoverNormal.index < 0)
                {
                    ctx.viewCount = viewCount;
                    ctx.endIndex = stockCnt;
                    ctx.beginIndex = ctx.endIndex - ctx.viewCount;
                }
                else
                {
                    const double percent = double(ctx.hoverNormal.index - ctx.beginIndex) / ctx.viewCount;
                    ctx.viewCount = viewCount;
                    ctx.beginIndex = ctx.hoverNormal.index - std::round(percent * viewCount);
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

        void onDBClick(const Point &point)
        {
            auto &ctx = *m_context;
            ctx.crossLineVisible = !ctx.crossLineVisible;

            calcContext();
            fire(ID_OnDBClick);
        }

        void setSyncOther(ChartViewModel *other)
        {
            listen(other);
        }

        void syncViewCount(int viewCount, int beginIndex, int endIndex, Real nodeWidth, Real stickWidth)
        {
            auto &ctx = *m_context;
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
            auto &ctx = *m_context;
            if (ctx.hoverSync.index == hoverIndex && ctx.hoverSync.price == hoverPrice)
                return;

            ChartCoordinate coordinate(m_props, m_context);

            // mouse event
            ctx.hoverSync.point = Point(
                coordinate.index2pos(hoverIndex),
                coordinate.price2pos(hoverPrice));
            ctx.hoverSync.index = hoverIndex;
            ctx.hoverSync.price = hoverPrice;

            for (const auto &layer : m_layers)
                layer->onMouseMove();
            fire(ID_ChartContextChanged);
        }

        void syncDBClick(bool crossLineVisible)
        {
            auto &ctx = *m_context;
            if (ctx.crossLineVisible == crossLineVisible)
                return;
            ctx.crossLineVisible = crossLineVisible;
            calcContext();
        }

    private:
        // [0]
        std::shared_ptr<ChartModel> m_model;
        std::shared_ptr<ChartProps> m_props;
        std::shared_ptr<ChartContext> m_context;

        // [1]
        std::vector<std::shared_ptr<ChartLayer>> m_layers;
    };
}
