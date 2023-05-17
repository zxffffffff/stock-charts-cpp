/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../Core/StockCore.h"
#include "../Core/DataBinding.h"
#include "../Core/Utils.h"

namespace StockCharts
{
    constexpr inline char ID_ChartPluginChanged[] = "ID_ChartPluginChanged";

    class ChartPlugin : public DataBinding
    {
    public:
        virtual void init(std::shared_ptr<const StockCore> stockCore) = 0;

        virtual void onStockCoreChanged(std::shared_ptr<const StockCore> stockCore) {}
    };
}
