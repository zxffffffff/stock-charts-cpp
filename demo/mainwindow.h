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
#include "../lib-chart/Version.h"
#include "../lib-chart/Model/Plugin/PluginIndicator.h"
#include "../lib-chart/Model/Plugin/PluginPainting.h"
#include "../lib-chart/Model/Plugin/PluginSuperimposition.h"
#include "../lib-chart/ViewModel/Layer/LayerBG.h"
#include "../lib-chart/ViewModel/Layer/LayerStock.h"
#include "../lib-chart/ViewModel/Layer/LayerSuperimposition.h"
#include "../lib-chart/ViewModel/Layer/LayerIndicator.h"
#include "../lib-chart/ViewModel/Layer/LayerPainting.h"
#include "../lib-chart/ViewModel/Layer/LayerCrossLine.h"
#include "../lib-chart/ViewModel/Layer/LayerTitle.h"
#include "../lib-chart/View/ChartViewQt.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public StockCharts::DataBinding
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void on(DataBinding *sender, const std::string &id) override;

    // tab-indicator
    std::shared_ptr<const StockCharts::StIndicator> addIndicator(const StockCharts::IndexFormula &formula, bool main);
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
        StockCharts::ChartViewQt *view = nullptr;
        std::shared_ptr<StockCharts::ChartViewModel> vm;
        std::shared_ptr<StockCharts::ChartModel> model;
    };
    std::shared_ptr<StockCharts::StockCore> m_kStock;
    std::shared_ptr<StockCharts::StockCore> m_tStock;
    std::vector<StChart> m_kcharts;
    std::vector<StChart> m_tcharts;
};
#endif // MAINWINDOW_H
