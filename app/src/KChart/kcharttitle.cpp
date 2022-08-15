/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include "kcharttitle.h"

KChartTitle::KChartTitle(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

KChartTitle::~KChartTitle()
{
}

void KChartTitle::init(std::shared_ptr<StockCharts::ChartVM> vm)
{
    m_vm = vm;
    bind(m_vm.get());
}

void KChartTitle::on(DataBinding* sender, const std::string& id)
{
    if (m_vm.get() == sender)
        fire(id);
}
