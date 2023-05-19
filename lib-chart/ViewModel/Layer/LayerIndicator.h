/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../ChartLayer.h"
#include "../../Model/Plugin/PluginIndicator.h"

namespace StockCharts
{
    class LayerIndicator : public ChartLayer
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
            auto plugin = model->getPlugin<PluginIndicator>();
            auto indicators = plugin->getIndicators();

            auto minmax = std::pair<Number, Number>{NumberNull, NumberNull};
            for (auto &indicator : indicators)
            {
                for (auto &exp : indicator->indexCore.exps)
                {
                    auto minmax2 = exp.core.getMinMax(context.beginIndex, context.endIndex);
                    minmax.first = NumberCore::min(minmax.first, minmax2.first);
                    minmax.second = NumberCore::max(minmax.second, minmax2.second);
                }
            }
            return minmax;
        }

        virtual void onContextChanged(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context) override
        {
            auto plugin = model->getPlugin<PluginIndicator>();
            auto indicators = plugin->getIndicators();

            const int indexCnt = indicators.size();
            m_areaIndexs.resize(indexCnt);
            for (int i = 0; i < indexCnt; i += 1)
            {
                auto &areaExps = m_areaIndexs[i].exps;
                const auto &indicator = indicators[i];
                const int expCnt = indicator->indexCore.exps.size();
                areaExps.resize(expCnt);
                for (int j = 0; j < expCnt; j += 1)
                {
                    auto &exp = areaExps[j];
                    auto &indicatorExp = indicator->indexCore.exps[j];
                    switch (indicatorExp.drawingType.type)
                    {
                    case EnDrawingType::None:
                    case EnDrawingType::Number:
                    case EnDrawingType::Text:
                        exp = createLineExp(
                            model,
                            props,
                            context,
                            indicatorExp.core);
                        break;
                    case EnDrawingType::Candlestick:
                        // zxf todo
                        break;
                    }
                    exp.colors[0] = Color(indicatorExp.colorType.color.c_str());
                }
            }
        }

        virtual void onPaint(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context,
            Painter &painter) override
        {
            for (auto &index : m_areaIndexs)
            {
                for (auto &exp : index.exps)
                {
                    switch (exp.type)
                    {
                    case EnChartAreaExpType::Stick:
                        painter.drawStick(exp.sticks, props.riseColor, props.fallColor);
                        break;
                    case EnChartAreaExpType::Line:
                        painter.drawPath(exp.lines, exp.colors[0]);
                        break;
                    }
                }
            }
        }
    };
}
