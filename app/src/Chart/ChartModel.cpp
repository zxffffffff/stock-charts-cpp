/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "ChartModel.h"
#include <cassert>

using namespace StockCharts;

ChartModel::ChartModel(std::shared_ptr<StockCore> stockCore)
    : m_stockCore(stockCore)
{
    assert(m_stockCore);
}
