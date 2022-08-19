/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../ChartLayer.h"
#include "../../Graphics/Widget.h"
#include <vector>

namespace StockCharts
{
    struct ChartTitleItem
    {
        Widget btnSetting;
        Widget btnClose;
        std::vector<Widget> items;
    };

    class LayerTitle : public ChartLayer
    {
    public:
        LayerTitle(std::shared_ptr<const ChartModel> model, std::shared_ptr<const ChartProps> props, std::shared_ptr<const ChartContext> context)
            : ChartLayer(model, props, context)
        {
        }
        virtual ~LayerTitle() = default;

        virtual void onContextChanged() override
        {
        }

        virtual void onPaint(Painter& painter) override
        {
        }

    private:
        EnStockLineType m_klineType = EnStockLineType::Candlestick;

        std::vector<ChartTitleItem> m_rows;
    };

}

