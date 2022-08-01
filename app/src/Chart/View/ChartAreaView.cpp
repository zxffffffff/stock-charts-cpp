#include "ChartAreaView.h"
#include "../VM/ChartAreaVM.h"

using namespace StockCharts;

ChartAreaView::ChartAreaView(std::shared_ptr<ChartAreaVM> vm)
{
    m_vm = vm;
}

void StockCharts::ChartAreaView::setVM(std::shared_ptr<ChartAreaVM> vm)
{
    m_vm = vm;
}

void ChartAreaView::OnResize(Rect rect)
{
    m_vm->calcContext(rect);
}

void ChartAreaView::OnPaint(Painter& painter)
{
    auto& context = *m_vm->getContext();

    painter.fillRect(context.calc.rectXAxis, Color(150, 100, 100));
    painter.fillRect(context.calc.rectYAxis, Color(100, 150, 100));
    painter.fillRect(context.calc.rectChart, Color(100, 100, 150));
    painter.fillRect(context.calc.rectInnerChart, Color(100, 100, 200));

    if (context.main) {
        painter.drawStick(context.kline.buffer.sticks, Color(255, 0, 0), Color(0, 255, 0));
    }
}

