#include "KChartBodyVM.h"
#include "../Model/KChartModel.h"

using namespace StockCharts;

KChartBodyVM::KChartBodyVM()
{

}

void KChartBodyVM::setModel(std::shared_ptr<KChartModel> model)
{
    m_pKChartModel = model;
}

void StockCharts::KChartBodyVM::calcContext(SpKChartContext _context, Real _x, Real _y, Real _width, Real _height)
{
    auto& context = *_context;

    // view
    context.x = _x;
    context.y = _y;
    bool sizeChanged = (_width != context.width || _height != context.height);
    if (sizeChanged) {
        context.width = _width;
        context.height = _height;
    }

    // model
    auto stockCore = m_pKChartModel->getStockCore();
    const auto minmax = stockCore->getMinMax();
    const Number& min = minmax.first;
    const Number& max = minmax.second;
    const int cnt = stockCore->getSize();
    bool minmaxChanged = (min != context.min || max != context.max || cnt != context.cnt);
    if (minmaxChanged) {
        context.min = min;
        context.max = max;
        context.cnt = cnt;
    }

    // calc
    if (sizeChanged || minmaxChanged) {
        context.rectXAxis.set(context.x, _height - context.xAxisHeight, _width, context.xAxisHeight);
        context.rectYAxis.set(_width - context.yAxisWidth, context.y, context.yAxisWidth, _height);
        context.rectChart.set(
            context.x,
            context.y,
            _width - context.yAxisWidth,
            _height - context.xAxisHeight
        );
        context.klineChart.set(
            context.leftPadding,
            context.topPadding,
            _width - context.yAxisWidth - context.leftPadding - context.rightPadding,
            _height - context.xAxisHeight - context.topPadding - context.btmPadding
        );

        const auto& o = stockCore->open;
        const auto& h = stockCore->high;
        const auto& l = stockCore->low;
        const auto& c = stockCore->close;
        context.kline.resize(cnt);
        for (int i = 0; i < cnt; i += 1) {
            const Real xPos = getXPos(context, i);
            const Real oPos = getYPos(context, o[i]);
            const Real hPos = getYPos(context, h[i]);
            const Real lPos = getYPos(context, l[i]);
            const Real cPos = getYPos(context, c[i]);
            context.kline[i].set(
                xPos,
                std::min(oPos, cPos),
                context.stickWidth,
                std::abs(oPos - cPos),
                hPos,
                lPos
            );
        }
    }
}

Real StockCharts::KChartBodyVM::getXPos(const StKChartContext& context, int index) const
{
    return context.klineChart.left()
        + (context.klineChart.width() * index) / context.cnt
        + (context.stickWidth / 2);
}

Real StockCharts::KChartBodyVM::getYPos(const StKChartContext& context, Number price) const
{
    return context.klineChart.top()
        + (context.klineChart.bottom() - context.klineChart.top())
        * ((context.max - price) / (context.max - context.min));
}
