#include "StockCore.h"

using namespace StockCharts;

int StockCore::getSize() const
{
    return close.size();
}

std::pair<Number, Number> StockCore::getMinMax() const
{
    auto minmax = high.getMinMax();
    auto minmax2 = low.getMinMax();
    return { NumberCore::min(minmax.first, minmax2.first), NumberCore::max(minmax.second, minmax2.second) };
}
