#pragma once
#include "../../Core/NumberCore.h"
#include "../../Core/Graphics.h"

namespace StockCharts
{
    enum class EnCoordinateType
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
        void init(Number _min, Number _max, Real _top, Real _bottom)
        {
            min = _min;
            max = _max;
            top = _top;
            bottom = _bottom;
            calc();
        }

        void setType(EnCoordinateType _type)
        {
            type = _type;
            calc();
        }

        void calc()
        {
            const Real minus = max - min;
            const Real height = bottom - top;
            switch (type)
            {
            case StockCharts::EnCoordinateType::Linear:
                ratio = height / minus;
                berth = min;
                break;
            case StockCharts::EnCoordinateType::Proportional:
                break;
            case StockCharts::EnCoordinateType::Percentage:
                break;
            case StockCharts::EnCoordinateType::LogLinear:
                break;
            case StockCharts::EnCoordinateType::LogProportional:
                break;
            case StockCharts::EnCoordinateType::LogPercentage:
                break;
            }
        }

        Real price2pos(Number price)
        {
            switch (type)
            {
            case StockCharts::EnCoordinateType::Linear:
            case StockCharts::EnCoordinateType::Proportional:
            case StockCharts::EnCoordinateType::Percentage:
                return bottom - (price - berth) * ratio;
            case StockCharts::EnCoordinateType::LogLinear:
            case StockCharts::EnCoordinateType::LogProportional:
            case StockCharts::EnCoordinateType::LogPercentage:
                return bottom - (std::log(price) - berth) * ratio;
            }
            return 0;
        }

        Number pos2price(Real pos)
        {
            Number cache;
            switch (type)
            {
            case StockCharts::EnCoordinateType::Linear:
            case StockCharts::EnCoordinateType::Proportional:
            case StockCharts::EnCoordinateType::Percentage:
                return berth + (bottom - pos) / ratio;
            case StockCharts::EnCoordinateType::LogLinear:
            case StockCharts::EnCoordinateType::LogProportional:
            case StockCharts::EnCoordinateType::LogPercentage:
                cache = (bottom - pos) / ratio;
                cache = std::exp(cache + berth);
                cache -= std::exp(berth);
                return berth + cache;
            }
            return NumberCore::EmptyNumber;
        }

    protected:
        EnCoordinateType type = EnCoordinateType::Linear;
        Number min = 0;
        Number max = 0;
        Real top = 0;
        Real bottom = 0;
        // cache
        Real ratio = 0;
        Real berth = 0;
    };
}
