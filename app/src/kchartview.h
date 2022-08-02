#pragma once
#include <QWidget>
#include "Core/PainterQt.h"
#include "Chart/Model/ChartModel.h"
#include "Chart/View/Title/ChartTitleView.h"
#include "Chart/View/Area/ChartAreaView.h"
#include "Chart/View/Area/ChartAreaVM.h";
#include "Chart/View/Timebar/ChartTimebarView.h"
#include "Chart/Plugin/Background/PluginBG.h"
#include "Chart/Plugin/CandleStick/PluginKLine.h"
#include "Plugin/Indicator/PluginIndicator.h"

namespace Ui {
    class KChartView;
}

class KChartView : public QWidget
{
    Q_OBJECT

public:
    explicit KChartView(QWidget* parent = nullptr);
    ~KChartView();

    std::shared_ptr<const StockCharts::StIndicator> addIndicator(int areaIndex, const StockCharts::IndexFormula& formula);
    void clearIndicators();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    Ui::KChartView* ui;

    static const int nAreaCount = 2; // body, body_2

    std::array<QWidget*, nAreaCount>                                    m_bodyVidgets;
    std::array<std::shared_ptr<StockCharts::ChartModel>, nAreaCount>    m_models;
    std::array<std::shared_ptr<StockCharts::ChartAreaVM>, nAreaCount>   m_areaVMs;
    std::array<StockCharts::ChartTitleView, nAreaCount>                 m_titleViews;
    std::array<StockCharts::ChartAreaView, nAreaCount>                  m_areaViews;
    StockCharts::ChartTimebarView                                       m_timebarView;
};

