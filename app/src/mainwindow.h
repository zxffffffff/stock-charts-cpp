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
#include "KChart/KChartWidget.h"

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
    void slotDrawingType(int i);
    void slotCorrdinate(int i);
    void slotYLWidth(int i);
    void slotYRWidth(int i);
    void slotXHeight(int i);
    void slotPaddingLeft(int i);
    void slotPaddingTop(int i);
    void slotPaddingRight(int i);
    void slotPaddingBottom(int i);

    // tab-indicator
    void slotIndicatorBtnAdd();
    void slotIndicatorBtnClear();

private:
    Ui::MainWindow ui;

    struct StKChart 
    {
        bool main = false;
        KChartWidget* widget = nullptr;
        std::shared_ptr<StockCharts::ChartModel> model;
    };
    std::shared_ptr<StockCharts::StockCore> m_stockCore;
    std::vector<StKChart> m_kcharts;
};
#endif // MAINWINDOW_H
