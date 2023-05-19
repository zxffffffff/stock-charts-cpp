/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "ExpCore.h"
#include <map>
#include <set>

namespace StockCharts
{
    enum class EnStockRely
    {
        None,
        VOLV,    // 虚拟成交量
        NoAdj,   // 依赖不复权数据
        UQStock, // （期权）依赖正股数据
    };
    using StockRelyData = std::map<EnStockRely, NumberCore>;

    struct IndexFormula
    {
        std::string name;
        std::string expression;
        std::map<std::string, int> params;
    };

    struct IndexCore
    {
        std::vector<ExpCore> exps;
        bool err = false;
        std::string errExpression;
        std::string errWord;
    };

    struct StIndicator
    {
        IndexFormula formula;
        IndexCore indexCore;
    };
}
