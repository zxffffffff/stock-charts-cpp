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
    private:
        // [0]
        std::shared_ptr<ChartModel> m_model;

        // [1]
        ChartProps m_props;
        ChartContext m_context;
        std::vector<std::shared_ptr<ChartLayer>> m_layers;

    public:
        ChartViewModel(std::shared_ptr<ChartModel> model)
            : m_model(model)
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
                const auto &ctx = other->getContext();

                if (id == ID_OnMouseMove || id == ID_OnMouseLeave)
                    syncMouseMove(ctx.hoverNormal.index, ctx.hoverNormal.price);
                else if (id == ID_OnScrollX || id == ID_OnWheelY)
                    syncViewCount(ctx.viewCount, ctx.beginIndex, ctx.endIndex, ctx.nodeWidth, ctx.stickWidth);
                else if (id == ID_OnDBClick)
                    syncDBClick(ctx.crossLineVisible);
            }
        }

        // [0]
        const ChartProps &getProps() const
        {
            return m_props;
        }

        void setProps(const ChartProps &props, bool calcCtx = true)
        {
            m_props = props;
            if (calcCtx)
                calcContext();
        }

        const ChartContext &getContext() const
        {
            return m_context;
        }

        // [1]
        template <typename T = ChartLayer, typename... Args>
        std::shared_ptr<T> addLayer(Args &&...args)
        {
            assert(!getLayer<T>());
            auto layer = std::make_shared<T>(args...);
            m_layers.push_back(layer);
            listen(layer.get());
            layer->init(m_model, m_props, m_context);
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

            // [0] content
            m_context.rectXAxis.set(
                m_context.rectView.left() + m_props.ylAxisWidth,
                m_context.rectView.bottom() - m_props.xAxisHeight,
                m_context.rectView.width() - m_props.ylAxisWidth - m_props.yrAxisWidth,
                m_props.xAxisHeight);
            m_context.rectYLAxis.set(
                m_context.rectView.left(),
                m_context.rectView.top(),
                m_props.ylAxisWidth,
                m_context.rectView.height());
            m_context.rectYRAxis.set(
                m_context.rectView.right() - m_props.yrAxisWidth,
                m_context.rectView.top(),
                m_props.yrAxisWidth,
                m_context.rectView.height());
            m_context.rectChart.set(
                m_context.rectView.left() + m_props.ylAxisWidth,
                m_context.rectView.top(),
                m_context.rectView.width() - m_props.ylAxisWidth - m_props.yrAxisWidth,
                m_context.rectView.height() - m_props.xAxisHeight);
            m_context.rectInnerChart.set(
                m_context.rectChart.left() + m_props.paddingLeft,
                m_context.rectChart.top() + m_props.paddingTop,
                m_context.rectChart.width() - m_props.paddingLeft - m_props.paddingRight,
                m_context.rectChart.height() - m_props.paddingTop - m_props.paddingBottom);

            // invalid size
            if (!m_context.rectInnerChart.valid())
            {
                // assert(false);
                return;
            }

            // [1] x
            const int stockCnt = stockCore->getSize();
            const Real stockWidth = stockCnt * m_context.nodeWidth;
            const Real viewWidth = m_context.rectInnerChart.width();
            switch (m_props.xCoordType)
            {
            case EnXCoordinateType::Fill:
                m_context.nodeWidth = viewWidth / (stockCnt + 1);
                m_context.viewCount = stockCnt;
                m_context.endIndex = stockCnt;
                m_context.beginIndex = 0;
                break;
            case EnXCoordinateType::Cover:
            default:
                if (stockWidth <= viewWidth)
                {
                    if (m_context.viewCount != stockCnt)
                    {
                        m_context.viewCount = stockCnt;
                        m_context.endIndex = stockCnt;
                        m_context.beginIndex = 0;
                    }
                }
                else
                {
                    int viewCount = std::floor(viewWidth / m_context.nodeWidth);
                    if (m_context.viewCount != viewCount)
                    {
                        m_context.viewCount = viewCount;
                        m_context.endIndex = stockCnt;
                        m_context.beginIndex = m_context.endIndex - m_context.viewCount;
                    }
                }
                break;
            }
            assert(m_context.viewCount >= 0);
            assert(m_context.endIndex >= 0);
            assert(m_context.beginIndex >= 0);
            assert(m_context.beginIndex <= m_context.endIndex);
            assert(m_context.endIndex <= stockCnt);
            assert(m_context.endIndex - m_context.beginIndex <= m_context.viewCount);

            // [2] y
            m_context.minPrice = NumberNull;
            m_context.maxPrice = NumberNull;
            for (const auto &layer : m_layers)
            {
                auto minmax = layer->getMinMax(m_model, m_props, m_context);
                m_context.minPrice = NumberCore::min(minmax.first, m_context.minPrice);
                m_context.maxPrice = NumberCore::max(minmax.second, m_context.maxPrice);
            }

            for (const auto &layer : m_layers)
            {
                layer->onContextChanged(m_model, m_props, m_context);
            }
            fire(ID_ChartContextChanged);
        }

        // [3]
        void onPaint(Painter &painter)
        {
            for (const auto &layer : m_layers)
            {
                painter.save();
                layer->onPaint(m_model, m_props, m_context, painter);
                painter.restore();
            }
        }

        void onResize(const Rect &rect)
        {
            const Rect view(
                rect.left() + m_props.MarginLeft,
                rect.top() + m_props.MarginTop,
                rect.width() - m_props.MarginLeft - m_props.MarginRight,
                rect.height() - m_props.MarginTop - m_props.MarginBottom);
            if (m_context.rectView == view)
                return;
            m_context.rectView = view;

            calcContext();
            fire(ID_OnResize);
        }

        void onMouseMove(const Point &point)
        {
            if (m_context.hoverNormal.point == point)
                return;

            ChartCoordinate coordinate(m_props, m_context);

            // mouse event
            m_context.hoverNormal.point = point;
            if (m_context.rectInnerChart.contains(m_context.hoverNormal.point))
            {
                m_context.hoverNormal.index = coordinate.pos2index(point.x);
                if (m_context.hoverNormal.index > m_context.endIndex - 1)
                    m_context.hoverNormal.index = m_context.endIndex - 1;
                m_context.hoverNormal.price = coordinate.pos2price(point.y);
            }
            else
            {
                m_context.hoverNormal.point.clear();
                m_context.hoverNormal.index = -1;
                m_context.hoverNormal.price = NumberNull;
            }

            for (const auto &layer : m_layers)
            {
                layer->onMouseMove(m_model, m_props, m_context);
            }
            fire(ID_OnMouseMove);
        }

        void onMouseLeave()
        {
            ChartCoordinate coordinate(m_props, m_context);

            // mouse event
            m_context.hoverNormal.point.clear();
            m_context.hoverNormal.index = -1;
            m_context.hoverNormal.price = NumberNull;

            for (const auto &layer : m_layers)
            {
                layer->onMouseLeave(m_model, m_props, m_context);
            }
            fire(ID_OnMouseLeave);
        }

        void onScrollX(int step)
        {
            if (step == 0)
                return;

            const auto &stockCore = m_model->getStockCore();

            if (m_props.xCoordType == EnXCoordinateType::Fill)
                return;

            // [1] x
            const int stockCnt = stockCore->getSize();
            m_context.beginIndex = std::min(std::max(0, m_context.beginIndex + step * m_props.scrollXStep), stockCnt - 1);
            m_context.endIndex = std::min(m_context.beginIndex + m_context.viewCount, stockCnt);

            calcContext();
            fire(ID_OnScrollX);
        }

        void onWheelY(int step)
        {
            if (step == 0)
                return;

            const auto &stockCore = m_model->getStockCore();

            if (m_props.xCoordType == EnXCoordinateType::Fill)
                return;

            int stepWidth = 2 * std::abs(step);
            int nodeWidth = m_context.nodeWidth + step * m_props.wheelYStep;
            if (nodeWidth % 2 != 1)
                nodeWidth -= 1;
            m_context.nodeWidth = std::min(std::max(Real(1), Real(nodeWidth)), Real(99));
            int stickWidth = std::round(m_context.nodeWidth * Real(0.75));
            if (stickWidth % 2 != 1)
                stickWidth -= 1;
            m_context.stickWidth = std::min(std::max(Real(1), Real(stickWidth)), Real(99));

            // [1] x
            const int stockCnt = stockCore->getSize();
            const Real stockWidth = stockCnt * m_context.nodeWidth;
            const Real viewWidth = m_context.rectInnerChart.width();
            if (stockWidth <= viewWidth)
            {
                m_context.viewCount = stockCnt;
                m_context.endIndex = stockCnt;
                m_context.beginIndex = m_context.endIndex - m_context.viewCount;
            }
            else
            {
                const int viewCount = std::floor(viewWidth / m_context.nodeWidth);
                if (m_context.hoverNormal.index < 0)
                {
                    m_context.viewCount = viewCount;
                    m_context.endIndex = stockCnt;
                    m_context.beginIndex = m_context.endIndex - m_context.viewCount;
                }
                else
                {
                    const double percent = double(m_context.hoverNormal.index - m_context.beginIndex) / m_context.viewCount;
                    m_context.viewCount = viewCount;
                    m_context.beginIndex = m_context.hoverNormal.index - std::round(percent * viewCount);
                    if (m_context.beginIndex >= stockCnt)
                        m_context.beginIndex = stockCnt - 1;
                    if (m_context.beginIndex < 0)
                        m_context.beginIndex = 0;
                    m_context.endIndex = std::min(m_context.beginIndex + m_context.viewCount, stockCnt);
                }
            }

            calcContext();
            fire(ID_OnWheelY);
        }

        void onDBClick(const Point &point)
        {
            m_context.crossLineVisible = !m_context.crossLineVisible;

            calcContext();
            fire(ID_OnDBClick);
        }

        void setSyncOther(ChartViewModel *other)
        {
            listen(other);
        }

        void syncViewCount(int viewCount, int beginIndex, int endIndex, Real nodeWidth, Real stickWidth)
        {
            if (m_context.viewCount == viewCount &&
                m_context.beginIndex == beginIndex &&
                m_context.endIndex == endIndex &&
                m_context.nodeWidth == nodeWidth &&
                m_context.stickWidth == stickWidth)
                return;

            // [1] x
            m_context.viewCount = viewCount;
            m_context.beginIndex = beginIndex;
            m_context.endIndex = endIndex;
            m_context.nodeWidth = nodeWidth;
            m_context.stickWidth = stickWidth;

            calcContext();
        }

        void syncMouseMove(int hoverIndex, Number hoverPrice)
        {
            if (m_context.hoverSync.index == hoverIndex && m_context.hoverSync.price == hoverPrice)
                return;

            ChartCoordinate coordinate(m_props, m_context);

            // mouse event
            m_context.hoverSync.point = Point(
                coordinate.index2pos(hoverIndex),
                coordinate.price2pos(hoverPrice));
            m_context.hoverSync.index = hoverIndex;
            m_context.hoverSync.price = hoverPrice;

            for (const auto &layer : m_layers)
                layer->onMouseMove(m_model, m_props, m_context);
            fire(ID_ChartContextChanged);
        }

        void syncDBClick(bool crossLineVisible)
        {
            if (m_context.crossLineVisible == crossLineVisible)
                return;
            m_context.crossLineVisible = crossLineVisible;
            calcContext();
        }
    };
}
