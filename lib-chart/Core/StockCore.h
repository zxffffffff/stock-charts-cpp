/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "NumberCore.h"
#include <memory>

namespace StockCharts
{
    struct StockCore
    {
        NumberCore open;
        NumberCore high;
        NumberCore low;
        NumberCore close;
        NumberCore vol;
        NumberCore amount;
        NumberCore timestamp;

        StockCore &reverse()
        {
            open.reverse();
            high.reverse();
            low.reverse();
            close.reverse();
            vol.reverse();
            amount.reverse();
            timestamp.reverse();
            return *this;
        }

        int getSize() const
        {
            return close.size();
        }

        std::pair<Number, Number> getMinMax(int beginIndex, int endIndex) const
        {
            auto minmax = high.getMinMax(beginIndex, endIndex);
            auto minmax2 = low.getMinMax(beginIndex, endIndex);
            return {
                NumberCore::min(minmax.first, minmax2.first),
                NumberCore::max(minmax.second, minmax2.second)};
        }
    };
}
