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
        NumberCore open;	// necessary
        NumberCore high;	// necessary
        NumberCore low;		// necessary
        NumberCore close;	// necessary
        NumberCore vol;
        NumberCore amount;
        NumberCore timestamp;

        // by close
        int getSize() const
        {
            return close.size();
        }

        // by high, low
        std::pair<Number, Number> getMinMax(int beginIndex, int endIndex) const
        {
            auto minmax = high.getMinMax(beginIndex, endIndex);
            auto minmax2 = low.getMinMax(beginIndex, endIndex);
            return { 
                NumberCore::min(minmax.first, minmax2.first),
                NumberCore::max(minmax.second, minmax2.second)
            };
        }
    };
}
