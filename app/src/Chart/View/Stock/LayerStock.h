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
    class LayerStock : public ChartLayer
    {
    public:
        LayerStock(std::shared_ptr<const ChartModel> model, std::shared_ptr<const ChartProps> props, std::shared_ptr<const ChartContext> context)
            : ChartLayer(model, props, context)
        {
        }
        virtual ~LayerStock() = default;

        virtual std::pair<Number, Number> getMinMax(int beginIndex, int endIndex) override
        {
            auto stockCore = *m_model->getStockCore();

            return stockCore.getMinMax(beginIndex, endIndex);
        }

        virtual void onContextChanged() override
        {
            auto stockCore = *m_model->getStockCore();
            const auto& ctx = *m_context;
            const auto& props = *m_props;
            m_areaIndexs.resize(1);
            m_areaIndexs[0].exps.resize(1);

            switch (props.lineType)
            {
            case EnStockLineType::CandlestickHollow:
            case EnStockLineType::Candlestick:
            case EnStockLineType::BAR:
                m_areaIndexs[0].exps[0] = createStickExp(stockCore.open, stockCore.high, stockCore.low, stockCore.close);
                break;
            case EnStockLineType::Line:
                m_areaIndexs[0].exps[0] = createLineExp(stockCore.close);
                break;
            }
        }

        virtual void onPaint(Painter& painter) override
        {
            const auto& props = *m_props;

            if (m_areaIndexs.size() != 1 || m_areaIndexs[0].exps.size() != 1)
                return;
            auto& exp = m_areaIndexs[0].exps[0];

            switch (props.lineType)
            {
            case EnStockLineType::CandlestickHollow:
                painter.drawStickHollow(exp.sticks, props.riseColor, props.fallColor);
                break;
            case EnStockLineType::Candlestick:
                painter.drawStick(exp.sticks, props.riseColor, props.fallColor);
                break;
            case EnStockLineType::BAR:
                painter.drawBAR(exp.sticks, props.riseColor, props.fallColor);
                break;
            case EnStockLineType::Line:
                painter.drawPath(exp.lines, Color(100, 100, 200));
                break;
            }
        }
    };
}

