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
        PluginSuperimposition(std::shared_ptr<const StockCore> stockCore)
            : ChartPlugin(stockCore)
        {
        }
        virtual ~PluginSuperimposition() = default;
    };
}

