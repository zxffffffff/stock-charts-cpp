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
    class PluginSuperimposition : public ChartPlugin
    {
    public:
        virtual void init(std::weak_ptr<const StockCore> stockCore) override {}

        virtual void onStockCoreChanged(std::weak_ptr<const StockCore> stockCore) override {}
    };
}
