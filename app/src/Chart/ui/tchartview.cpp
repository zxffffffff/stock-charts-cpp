#include "tchartview.h"
#include "ui_tchartview.h"

using namespace StockCharts;

TChartView::TChartView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TChartView)
{
    ui->setupUi(this);
}

TChartView::~TChartView()
{
    delete ui;
}

