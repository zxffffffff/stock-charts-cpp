/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include <QWidget>
#include "ui_kchart.h"
#include "kcharttitle.h"
#include "kchartview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class KChart; }
QT_END_NAMESPACE

class KChart : public QWidget, public StockCharts::DataBinding
{
    Q_OBJECT

public:
    KChart(QWidget *parent = nullptr);
    ~KChart();

    void init(std::shared_ptr<StockCharts::StockCore> stockCore, bool main);

    std::shared_ptr<const StockCharts::ChartContext> getContext() const
    {
        return ui.chart->getContext();
    }

    KChartTitle* getChartTitle()
    {
        return ui.title;
    }
    KChartView* getChartView()
    {
        return ui.chart;
    }

    void addSyncChart(KChart* otherChart);

    virtual void on(DataBinding* sender, const std::string& id) override;

    // indicator
    std::shared_ptr<const StockCharts::StIndicator> addIndicator(const StockCharts::IndexFormula& formula);
    void clearIndicators();

private:
    Ui::KChart ui;

    std::shared_ptr<StockCharts::StockCore> m_stockCore;
    std::shared_ptr<StockCharts::ChartModel> m_model;
};
