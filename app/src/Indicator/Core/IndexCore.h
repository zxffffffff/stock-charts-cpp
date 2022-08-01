#pragma once
#include "ExpCore.h"
#include <map>
#include <set>

namespace StockCharts
{
    enum class EnStockRely
    {
        None,
        VOLV,			// 虚拟成交量
        NoAdj,			// 依赖不复权数据
        UQStock,		// （期权）依赖正股数据
    };
    using StockRelyData = std::map<EnStockRely, NumberCore>;

    struct IndexFormula
    {
        String					name;
        bool					sub = true;
        String					expression;
        std::map<String, int>	params;
    };

    struct IndexCore
    {
        std::vector<ExpCore>	exps;
        bool					err = false;
        String					errExpression;
        String					errWord;
    };

    struct StIndicator
    {
        IndexFormula formula;
        IndexCore indexCore;
    };
}
