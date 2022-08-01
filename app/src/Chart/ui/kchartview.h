#pragma once
#include <QWidget>
#include <QPainter>
#include "../../Core/Painter.h"
#include "../Model/ChartModel.h"
#include "../VM/ChartAreaVM.h"
#include "../View/ChartTitleView.h"
#include "../View/ChartAreaView.h"
#include "../View/ChartTimebarView.h"

namespace Ui {
    class KChartView;
}
namespace StockCharts {
    class ChartAreaVM;
}

class KChartView : public QWidget
{
    Q_OBJECT

public:
    explicit KChartView(QWidget* parent = nullptr);
    ~KChartView();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    Ui::KChartView* ui;
    std::vector<QWidget*> m_bodyVidgets;

    std::shared_ptr<StockCharts::ChartModel> m_model;

    std::vector<StockCharts::ChartTitleView> m_titleViews;
    std::vector<StockCharts::ChartAreaView> m_areaViews;
    StockCharts::ChartTimebarView m_timebarView;
};

