#include "kchartview.h"
#include "ui_kchartview.h"
#include <QResizeEvent>
#include <QDebug>

using namespace StockCharts;

namespace
{
    const int nAreaCount = 2; // body, body_2

    StockCore GenerateStock()
    {
        StockCore stock;
        stock.open = { 790.0, 803.5, 811.5, 838.0, 880.0, 830.5, 869.0, 827.5, 808.0, 818.0, 808.0, 800.5, 806.0, 817.0, 783.5, 802.0, 831.5, 900.0, 917.5, 940.0, 947.0, 895.0, 870.0, 837.0, 824.0, 820.0, 811.0, 789.5, 739.0, 758.0 };
        stock.high = { 800.0, 803.5, 829.5, 879.0, 888.0, 867.5, 869.0, 830.0, 811.0, 821.5, 808.0, 817.5, 814.5, 817.0, 790.0, 812.0, 867.0, 916.0, 940.0, 976.5, 949.0, 895.0, 870.0, 843.0, 835.0, 826.0, 814.5, 790.0, 739.5, 785.0 };
        stock.low = { 753.0, 790.0, 811.0, 838.0, 871.5, 830.0, 842.0, 811.0, 803.0, 803.0, 799.5, 799.0, 800.0, 808.0, 778.0, 802.0, 831.5, 894.5, 912.0, 932.0, 917.0, 884.0, 856.0, 823.0, 822.0, 816.0, 803.5, 761.5, 716.0, 755.0 };
        stock.close = { 800.0, 800.0, 828.0, 870.0, 874.0, 855.0, 850.0, 812.0, 809.0, 816.5, 801.0, 809.0, 809.5, 814.0, 790.0, 810.0, 860.0, 905.0, 932.0, 943.0, 941.5, 891.0, 860.0, 838.0, 828.0, 818.0, 808.0, 787.0, 729.0, 778.0 };
        return stock;
    }
}

KChartView::KChartView(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::KChartView)
{
    ui->setupUi(this);

    m_bodyVidgets.push_back(ui->body);
    m_bodyVidgets.push_back(ui->body_2);
    for (int i = 0; i < nAreaCount; i++) {
        QWidget* widget = m_bodyVidgets[i];
        widget->installEventFilter(this);
    }

    auto stockCore = std::make_shared<StockCore>(GenerateStock());
    m_model = std::make_shared<StockCharts::ChartModel>(stockCore);
    m_titleViews.resize(nAreaCount);
    m_areaViews.resize(nAreaCount);
    for (int i = 0; i < nAreaCount; i++) {
        m_titleViews[i]; // todo
        auto areaVM = std::make_shared<ChartAreaVM>(m_model);
        areaVM->setMain(i == 0);
        m_areaViews[i].setVM(areaVM);
    }
    m_timebarView; // todo
}

KChartView::~KChartView()
{
    delete ui;
}

bool KChartView::eventFilter(QObject* obj, QEvent* event)
{
    const auto type = event->type();
    if (type == QEvent::Resize) {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
        for (int i = 0; i < nAreaCount; i++) {
            QWidget* widget = m_bodyVidgets[i];
            if (widget != obj)
                continue;

            m_areaViews[i].OnResize(Rect(0, 0, resizeEvent->size().width(), resizeEvent->size().height()));
        }
    }
    else if (type == QEvent::Paint) {
        QPaintEvent* paintEvent = static_cast<QPaintEvent*>(event);
        for (int i = 0; i < nAreaCount; i++) {
            QWidget* widget = m_bodyVidgets[i];
            if (widget != obj)
                continue;

            PainterQt painter(widget);
            m_areaViews[i].OnPaint(painter);
        }
    }
    return false;
}

