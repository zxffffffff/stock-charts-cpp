#include "kchartview.h"
#include "ui_kchartview.h"
#include "../VM/KChartBodyVM.h"
#include <QPainter>

using namespace StockCharts;

KChartView::KChartView(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::KChartView)
{
    ui->setupUi(this);

    m_pKChartContext = std::make_shared<StKChartContext>();
    m_pKChartContext_2 = std::make_shared<StKChartContext>();
    m_pKChartContext_3 = std::make_shared<StKChartContext>();
}

KChartView::~KChartView()
{
    delete ui;
}

void KChartView::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QColor colorRise(241, 62, 58);

    QRect bodyRect(ui->body->pos(), ui->body->size());
    QRect bodyRect_2(ui->body_2->pos(), ui->body_2->size());
    QRect bodyRect_3(ui->body_3->pos(), ui->body_3->size());

    painter.fillRect(bodyRect, colorRise);
    painter.fillRect(bodyRect_2, colorRise);
    painter.fillRect(bodyRect_3, colorRise);

    m_pKChartVM->calcContext(m_pKChartContext, bodyRect.x(), bodyRect.y(), bodyRect.width(), bodyRect.height());
    m_pKChartVM_2->calcContext(m_pKChartContext_2, bodyRect_2.x(), bodyRect_2.y(), bodyRect_2.width(), bodyRect_2.height());
    m_pKChartVM_3->calcContext(m_pKChartContext_3, bodyRect_3.x(), bodyRect_3.y(), bodyRect_3.width(), bodyRect_3.height());
}

void KChartView::drawBG(std::shared_ptr<StKChartContext> context)
{
}

void KChartView::drawKLine(std::shared_ptr<StKChartContext> context)
{
}

void KChartView::drawIndex(std::shared_ptr<StKChartContext> context)
{
}

