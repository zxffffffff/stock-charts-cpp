/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../ChartPlugin.h"

namespace StockCharts
{
    class PluginPainting : public ChartPlugin
    {
    public:
        virtual void init(std::shared_ptr<const StockCore> stockCore) override
        {
        }

        virtual void onStockCoreChanged(std::shared_ptr<const StockCore> stockCore) override
        {
        }
    };
}
