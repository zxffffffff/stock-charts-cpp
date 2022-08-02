/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "ChartCoordinate.h"

using namespace StockCharts;

void ChartCoordinate::init(const Rect& _rect, Number _minPrice, Number _maxPrice, int _viewCount, Real _nodeWidth)
{
    rect = _rect;
    minPrice = _minPrice;
    maxPrice = _maxPrice;
    viewCount = _viewCount;
    nodeWidth = _nodeWidth;
    calcX();
    calcY();
}

void ChartCoordinate::setType(EnCoordinateYType _type)
{
    type = _type;
    calcY();
}

void ChartCoordinate::calcX()
{
    switch (type)
    {
    case StockCharts::EnCoordinateYType::Linear:
        yRatio = rect.height() / (maxPrice - minPrice);
        yBerth = minPrice;
        break;
    case StockCharts::EnCoordinateYType::Proportional:
        break;
    case StockCharts::EnCoordinateYType::Percentage:
        break;
    case StockCharts::EnCoordinateYType::LogLinear:
        break;
    case StockCharts::EnCoordinateYType::LogProportional:
        break;
    case StockCharts::EnCoordinateYType::LogPercentage:
        break;
    default:
        yRatio = 0;
        yBerth = 0;
        break;
    }
    yValid = (
        rect.height() > 0 &&
        minPrice != NumberCore::EmptyNumber &&
        maxPrice != NumberCore::EmptyNumber
    );
}

void ChartCoordinate::calcY()
{
    xValid = (
        nodeWidth > 0 &&
        viewCount > 0 &&
        rect.width() > 0
    );
}

Real ChartCoordinate::price2pos(Number price) const
{
    if (!yValid)
        return 0;

    switch (type)
    {
    case StockCharts::EnCoordinateYType::Linear:
    case StockCharts::EnCoordinateYType::Proportional:
    case StockCharts::EnCoordinateYType::Percentage:
        return rect.bottom() - (price - yBerth) * yRatio;
    case StockCharts::EnCoordinateYType::LogLinear:
    case StockCharts::EnCoordinateYType::LogProportional:
    case StockCharts::EnCoordinateYType::LogPercentage:
        return rect.bottom() - (std::log(price) - yBerth) * yRatio;
    default:
        return 0;
    }
}

Number ChartCoordinate::pos2price(Real pos) const
{
    if (!yValid)
        return NumberCore::EmptyNumber;

    Number cache;
    switch (type)
    {
    case StockCharts::EnCoordinateYType::Linear:
    case StockCharts::EnCoordinateYType::Proportional:
    case StockCharts::EnCoordinateYType::Percentage:
        return yBerth + (rect.bottom() - pos) / yRatio;
    case StockCharts::EnCoordinateYType::LogLinear:
    case StockCharts::EnCoordinateYType::LogProportional:
    case StockCharts::EnCoordinateYType::LogPercentage:
        cache = (rect.bottom() - pos) / yRatio;
        cache = std::exp(cache + yBerth);
        cache -= std::exp(yBerth);
        return yBerth + cache;
    default:
        return NumberCore::EmptyNumber;
    }
}

Real StockCharts::ChartCoordinate::index2pos(int index) const
{
    if (!xValid)
        return 0;

    return rect.left() + (rect.width() * index) / viewCount + (nodeWidth / 2);
}

int StockCharts::ChartCoordinate::pos2index(Real pos) const
{
    if (!xValid)
        return 0; // todo

    return 0;
}
