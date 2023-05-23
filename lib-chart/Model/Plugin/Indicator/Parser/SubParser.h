/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../../../../Core/StockCore.h"
#include "../Core/IndexCore.h"

namespace StockCharts
{
    class SubParser
    {
    public:
        SubParser() = default;
        virtual ~SubParser() = default;

        void setStockCore(std::shared_ptr<const StockCore> p)
        {
            m_spStockCore = p;
        }
        void setStockExt(std::shared_ptr<StockRelyData> p)
        {
            m_spStockExt = p;
        }

    protected:
        std::shared_ptr<const StockCore> m_spStockCore;
        std::shared_ptr<StockRelyData> m_spStockExt;
    };
}
