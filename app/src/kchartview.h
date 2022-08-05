/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include <QWidget>
#include "Core/PainterQt.h"
#include "Chart/ChartModel.h"
#include "Chart/ChartVM.h"
#include "Chart/Plugins/PluginBG.h"
#include "Chart/Plugins/PluginKLine.h"
#include "Chart/Plugins/PluginIndicator.h"
#include "Chart/Plugins/PluginCrossLine.h"

namespace Ui {
    class KChartView;
}

class KChartView : public QWidget
{
    Q_OBJECT

public:
    explicit KChartView(QWidget* parent = nullptr);
    ~KChartView();

    void init(std::shared_ptr<StockCharts::StockCore> stockCore, bool main);

    // indicator
    std::shared_ptr<const StockCharts::StIndicator> addIndicator(const StockCharts::IndexFormula& formula);
    void clearIndicators();

public slots:
    // general
    void slotDrawingType(int i);
    void slotCorrdinate(int i);
    void slotYLWidth(int i);
    void slotYRWidth(int i);
    void slotXHeight(int i);
    void slotPaddingLeft(int i);
    void slotPaddingTop(int i);
    void slotPaddingRight(int i);
    void slotPaddingBottom(int i);
    void slotNodeStickWidth(int nodeWidth, int stickWidth);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    Ui::KChartView* ui;

    std::shared_ptr<StockCharts::StockCore> m_stockCore;
    std::shared_ptr<StockCharts::ChartModel> m_model;
    std::shared_ptr<StockCharts::ChartVM> m_vm;
};

