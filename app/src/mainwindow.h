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
#include "ChartImpl/ChartWidget.h"
#include "Chart/Model/Superimposition/PluginSuperimposition.h"
#include "Chart/Model/Indicator/PluginIndicator.h"
#include "Chart/Model/Painting/PluginPainting.h"
#include "Chart/View/BG/LayerBG.h"
#include "Chart/View/Stock/LayerStock.h"
#include "Chart/View/Superimposition/LayerSuperimposition.h"
#include "Chart/View/Indicator/LayerIndicator.h"
#include "Chart/View/Painting/LayerPainting.h"
#include "Chart/View/CrossLine/LayerCrossLine.h"
#include "Chart/View/Title/LayerTitle.h"

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
