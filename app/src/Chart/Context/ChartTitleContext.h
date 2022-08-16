/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../../Core/Widget.h"
#include <vector>

namespace StockCharts
{
    struct ChartTitleItem
    {
        Widget btnSetting;
        Widget btnClose;
        std::vector<Widget> items;
    };

    struct ChartTitleContext
    {
        std::vector<ChartTitleItem> m_rows;
    };
}

