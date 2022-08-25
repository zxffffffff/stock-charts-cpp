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
    class LayerPainting : public ChartLayer
    {
    public:
        LayerPainting(std::shared_ptr<const ChartModel> model, std::shared_ptr<const ChartProps> props, std::shared_ptr<const ChartContext> context)
            : ChartLayer(model, props, context)
        {
        }
        virtual ~LayerPainting() = default;
    };
}

