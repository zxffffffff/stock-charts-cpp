#pragma once
#include <QWidget>


namespace Ui {
    class KChartView;
}
namespace StockCharts {
    class KChartBodyVM;
    struct StKChartContext;
}

class KChartView : public QWidget
{
    Q_OBJECT

public:
    explicit KChartView(QWidget* parent = nullptr);
    ~KChartView();

    void setVM(std::shared_ptr<StockCharts::KChartBodyVM> vm) { m_pKChartVM = vm; }
    void setVM_2(std::shared_ptr<StockCharts::KChartBodyVM> vm) { m_pKChartVM_2 = vm; }
    void setVM_3(std::shared_ptr<StockCharts::KChartBodyVM> vm) { m_pKChartVM_3 = vm; }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void drawBG(std::shared_ptr<StockCharts::StKChartContext> context);
    void drawKLine(std::shared_ptr<StockCharts::StKChartContext> context);
    void drawIndex(std::shared_ptr<StockCharts::StKChartContext> context);

private:
    Ui::KChartView* ui;

    std::shared_ptr<StockCharts::KChartBodyVM> m_pKChartVM;
    std::shared_ptr<StockCharts::KChartBodyVM> m_pKChartVM_2;
    std::shared_ptr<StockCharts::KChartBodyVM> m_pKChartVM_3;
    std::shared_ptr<StockCharts::StKChartContext> m_pKChartContext;
    std::shared_ptr<StockCharts::StKChartContext> m_pKChartContext_2;
    std::shared_ptr<StockCharts::StKChartContext> m_pKChartContext_3;
};

