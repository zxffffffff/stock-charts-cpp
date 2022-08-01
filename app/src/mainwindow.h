#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "Chart/ui/kchartview.h"
#include "Chart/VM/ChartAreaVM.h"
#include "Chart/Model/ChartModel.h"
#include "Indicator/IndicatorParser.h"

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

    KChartView* m_chartWidget = nullptr;
};
#endif // MAINWINDOW_H
