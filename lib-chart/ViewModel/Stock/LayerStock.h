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
        virtual void init(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context) override
        {
        }

        virtual std::pair<Number, Number> getMinMax(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context) override
        {
            auto stockCore = *model->getStockCore();

            return stockCore.getMinMax(context.beginIndex, context.endIndex);
        }

        virtual void onContextChanged(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context) override
        {
            auto stockCore = *model->getStockCore();
            m_areaIndexs.resize(1);
            m_areaIndexs[0].exps.resize(1);

            switch (props.lineType)
            {
            case EnStockLineType::CandlestickHollow:
            case EnStockLineType::Candlestick:
            case EnStockLineType::BAR:
                m_areaIndexs[0].exps[0] = createStickExp(
                    model,
                    props,
                    context,
                    stockCore.open,
                    stockCore.high,
                    stockCore.low,
                    stockCore.close);
                break;
            case EnStockLineType::Line:
                m_areaIndexs[0].exps[0] = createLineExp(
                    model,
                    props,
                    context,
                    stockCore.close);
                break;
            }
        }

        virtual void onPaint(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context,
            Painter &painter) override
        {
            if (m_areaIndexs.size() != 1 || m_areaIndexs[0].exps.size() != 1)
                return;
            auto &exp = m_areaIndexs[0].exps[0];

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
