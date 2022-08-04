/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include "ChartContext.h"

namespace StockCharts
{
    struct ChartCoordinate
    {
        std::shared_ptr<const ChartContext> context;

        ChartCoordinate(std::shared_ptr<const ChartContext> _context)
            : context(_context)
        {
        }
        ~ChartCoordinate() = default;

        bool validX() const;
        bool validY() const;

        Real price2pos(Number price) const;
        Number pos2price(Real pos) const;
        Real index2pos(int index) const;
        int pos2index(Real pos) const;
    };
}

