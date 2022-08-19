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
        ChartPlugin(std::shared_ptr<const StockCore> stockCore)
            : m_stockCore(stockCore)
        {
        }
        virtual ~ChartPlugin() = default;

        std::shared_ptr<const StockCore> getStockCore() const
        {
            return m_stockCore;
        }

        virtual void onStockCoreChanged() {}

    protected:
        std::shared_ptr<const StockCore> m_stockCore;
    };
}

