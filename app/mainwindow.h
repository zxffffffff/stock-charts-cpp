/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "version.h"
#include "chartwidget.h"
#include "lib-chart/Model/Superimposition/PluginSuperimposition.h"
#include "lib-chart/Model/Indicator/PluginIndicator.h"
#include "lib-chart/Model/Painting/PluginPainting.h"
#include "lib-chart/View/BG/LayerBG.h"
#include "lib-chart/View/Stock/LayerStock.h"
#include "lib-chart/View/Superimposition/LayerSuperimposition.h"
#include "lib-chart/View/Indicator/LayerIndicator.h"
#include "lib-chart/View/Painting/LayerPainting.h"
#include "lib-chart/View/CrossLine/LayerCrossLine.h"
#include "lib-chart/View/Title/LayerTitle.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public StockCharts::DataBinding
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void on(DataBinding* sender, const std::string& id) override;

    // tab-indicator
    std::shared_ptr<const StockCharts::StIndicator> addIndicator(const StockCharts::IndexFormula& formula, bool main);
    void clearIndicators();

public slots:
    void updateUI();

    // tab-general
    void slotGeneralPeriod();

    // tab-indicator
    void slotIndicatorBtnAdd();
    void slotIndicatorBtnClear();

private:
    Ui::MainWindow ui;

    struct StChart 
    {
        bool main = false;
        ChartWidget* widget = nullptr;
        std::shared_ptr<StockCharts::ChartModel> model;
    };
    std::shared_ptr<StockCharts::StockCore> m_kStock;
    std::shared_ptr<StockCharts::StockCore> m_tStock;
    std::vector<StChart> m_kcharts;
    std::vector<StChart> m_tcharts;
};
#endif // MAINWINDOW_H
