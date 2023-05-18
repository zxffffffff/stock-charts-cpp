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
    class LayerSuperimposition : public ChartLayer
    {
    public:
        virtual void init(
            std::shared_ptr<const ChartModel> model,
            const ChartProps &props,
            const ChartContext &context) override
        {
        }
    };
}
