#pragma once
#include <QWidget>

namespace StockCharts
{
    namespace Ui {
        class TChartView;
    }

    class TChartView : public QWidget
    {
        Q_OBJECT

    public:
        explicit TChartView(QWidget* parent = nullptr);
        ~TChartView();

    private:
        Ui::TChartView* ui;
    };
}

