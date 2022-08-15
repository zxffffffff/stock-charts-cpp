/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include <QWidget>
#include "../Core/PainterQt.h"
#include "../Core/DataBinding.h"
#include "../Chart/ChartModel.h"
#include "../Chart/ChartVM.h"
#include "../Chart/Plugins/PluginBG.h"
#include "../Chart/Plugins/PluginKLine.h"
#include "../Chart/Plugins/PluginIndicator.h"
#include "../Chart/Plugins/PluginCrossLine.h"

class KChartView : public QWidget, public StockCharts::DataBinding
{
    Q_OBJECT

public:
    explicit KChartView(QWidget* parent = nullptr);
    ~KChartView();

    void init(std::shared_ptr<StockCharts::ChartVM> vm);

    virtual void on(DataBinding* sender, const std::string& id) override;

    std::shared_ptr<const StockCharts::ChartContext> getContext() const
    {
        return m_vm->getContext();
    }

public slots:
    void slotCalcContext();

    void slotDrawingType(int i);
    void slotCorrdinate(int i);
    void slotYLWidth(int i);
    void slotYRWidth(int i);
    void slotXHeight(int i);
    void slotPaddingLeft(int i);
    void slotPaddingTop(int i);
    void slotPaddingRight(int i);
    void slotPaddingBottom(int i);
    void slotNodeWidth(int nodeWidth);
    void slotStickWidth(int stickWidth);

    void slotSyncViewCount(
        int viewCount,
        int beginIndex,
        int endIndex,
        StockCharts::Real nodeWidth, 
        StockCharts::Real stickWidth
    );
    void slotSyncMouseMove(int hoverIndex, StockCharts::Number hoverPrice);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

private:
    std::shared_ptr<StockCharts::ChartVM> m_vm;
};

