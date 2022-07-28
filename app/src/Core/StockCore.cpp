#include "StockCore.h"

using namespace StockCharts;

int StockCharts::StockCore::getSize() const
{
    return close.size();
}

std::pair<Number, Number> StockCore::getMinMax(int _begin/* = 0*/, int _end/* = -1*/) const
{
    const int len = getSize();

    int begin = _begin;
    int end = _end;
    if (end < 0)
        end = len;
    if (begin < 0 || begin > len) 
        return{ 0,0 };
    if (end < 0 || end > len)
        return{ 0,0 };

    Number min = low[begin];
    Number max = high[begin];
    for (int i = begin + 1; i < end; i += 1) {
        min = NumberCore::min(min, low[i]);
        max = NumberCore::max(max, high[i]);
    }
    return{ min, max };
}
