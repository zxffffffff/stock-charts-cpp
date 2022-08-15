/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include <QWidget>
#include "ui_kcharttitle.h"
#include "../Core/DataBinding.h"
#include "../Chart/ChartModel.h"
#include "../Chart/ChartVM.h"

QT_BEGIN_NAMESPACE
namespace Ui { class KChartTitle; }
QT_END_NAMESPACE

class KChartTitle  : public QWidget, public StockCharts::DataBinding
{
    Q_OBJECT

public:
    explicit KChartTitle(QWidget *parent = nullptr);
    ~KChartTitle();

    void init(std::shared_ptr<StockCharts::ChartVM> vm);

    virtual void on(DataBinding* sender, const std::string& id) override;

    std::shared_ptr<const StockCharts::ChartContext> getContext() const
    {
        return m_vm->getContext();
    }

private:
    Ui::KChartTitle ui;

    std::shared_ptr<StockCharts::ChartVM> m_vm;
};

