/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include <QWidget>
#include "lib-chart/Core/DataBinding.h"
#include "lib-chart/Graphics/PainterQt.h"
#include "lib-chart/Model/ChartModel.h"
#include "lib-chart/View/ChartView.h"

class ChartWidget : public QWidget, public StockCharts::DataBinding
{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget* parent = nullptr);
    ~ChartWidget();

    void init(std::shared_ptr<StockCharts::ChartView> vm);
    void syncSubChart(ChartWidget* otherChart);

    virtual void on(DataBinding* sender, const std::string& id) override;

    std::shared_ptr<const StockCharts::ChartProps> getProps() const
    {
        return m_view->getProps();
    }

    std::shared_ptr<const StockCharts::ChartContext> getContext() const
    {
        return m_view->getContext();
    }

public slots:
    void slotDrawingType(int i);
    void slotCorrdinate(int i);
    void slotYLWidth(int i);
    void slotYRWidth(int i);
    void slotXHeight(int i);
    void slotPaddingLeft(int i);
    void slotPaddingTop(int i);
    void slotPaddingRight(int i);
    void slotPaddingBottom(int i);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    std::shared_ptr<StockCharts::ChartView> m_view;
};

