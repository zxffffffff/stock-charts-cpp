#pragma once
#include "../../../Core/NumberCore.h"
#include "../../../Core/Graphics.h"

namespace StockCharts
{
    enum class EnCoordinateYType
    {
        Linear,             // 等差
        Proportional,       // 等比
        Percentage,         // 百分比
        LogLinear,          // 等差（对数）
        LogProportional,    // 等比（对数）
        LogPercentage,      // 百分比（对数）
    };

    class ChartCoordinate
    {
    public:
        // [0]
        void init(const Rect& rect, Number minPrice, Number maxPrice, int viewCount, Real nodeWidth);
        void setType(EnCoordinateYType type);

        // [1]
        void calcX();
        void calcY();

        // [2]
        Real price2pos(Number price) const;
        Number pos2price(Real pos) const;
        Real index2pos(int index) const;
        int pos2index(Real pos) const;

    protected:
        EnCoordinateYType type = EnCoordinateYType::Linear;
        Rect rect;
        Number minPrice = 0;
        Number maxPrice = 0;
        int viewCount = 0;
        Real nodeWidth = 0;
        // cache
        bool xValid = false;
        bool yValid = false;
        Real yRatio = 0;
        Number yBerth = 0;
    };
}
