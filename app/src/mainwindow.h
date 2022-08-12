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
#include "Core/DataBinding.h"

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

public slots:
    // indicator
    void slotIndicatorBtnAdd();
    void slotIndicatorBtnClear();

private:
    Ui::MainWindow ui;
    std::vector<KChart*> m_kcharts;
};
#endif // MAINWINDOW_H
