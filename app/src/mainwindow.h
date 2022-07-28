#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "CandlestickChart/View/kchartview.h"
#include "CandlestickChart/VM/KChartBodyVM.h"
#include "CandlestickChart/Model/KChartModel.h"
#include "TechnicalIndicators/IndicatorParser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent* event) override;

public slots:
    void slotBtnRun();

private:
    Ui::MainWindow ui;

    KChartView* m_pKChartView = nullptr;
    StockCharts::SpKChartModel m_spKChartModel = std::make_shared<StockCharts::KChartModel>();
    StockCharts::SpKChartBodyVM m_spKChartVM = std::make_shared<StockCharts::KChartBodyVM>();
    StockCharts::SpKChartBodyVM m_spKChartVM_2 = std::make_shared<StockCharts::KChartBodyVM>();
    StockCharts::SpKChartBodyVM m_spKChartVM_3 = std::make_shared<StockCharts::KChartBodyVM>();
};
#endif // MAINWINDOW_H
