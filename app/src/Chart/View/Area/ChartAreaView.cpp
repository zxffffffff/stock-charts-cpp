/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "ChartAreaView.h"
#include "ChartAreaVM.h"

using namespace StockCharts;

ChartAreaView::ChartAreaView(std::shared_ptr<ChartAreaVM> vm)
{
    m_vm = vm;
}

void StockCharts::ChartAreaView::setVM(std::shared_ptr<ChartAreaVM> vm)
{
    m_vm = vm;
}

void ChartAreaView::OnResize(Rect rect)
{
    m_vm->calcContext(rect);
}

void ChartAreaView::OnPaint(Painter& painter)
{
    m_vm->paintPlugins(painter);
}

