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
        NumberCore timestamp;

        // by close
        int getSize() const;

        // by high, low
        std::pair<Number, Number> getMinMax() const;
    };
}
