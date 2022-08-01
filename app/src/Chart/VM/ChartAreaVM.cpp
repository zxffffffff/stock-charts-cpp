#include "ChartAreaVM.h"
#include "../Model/ChartModel.h"

using namespace StockCharts;

ChartAreaVM::ChartAreaVM(std::shared_ptr<ChartModel> model)
{
    m_model = model;
    m_context = std::make_shared<ChartContext>();
}

void StockCharts::ChartAreaVM::setMain(bool b)
{
    auto& context = *m_context;

    context.main = b;
}

void ChartAreaVM::calcContext(Rect rectView, bool force)
{
    auto& context = *m_context;

    // view
    bool sizeChanged = (context.rectView != rectView);
    if (sizeChanged) {
        context.rectView = rectView;
    }

    // m_model
    auto stockCore = m_model->getStockCore();
    auto indicators = m_model->getIndicators();
    auto minmax = std::pair<Number, Number>({ NumberCore::EmptyNumber , NumberCore::EmptyNumber });
    if (context.main)
        minmax = stockCore->getMinMax();
    const int cnt = stockCore->getSize();
    for (auto& indexs : indicators) {
        for (auto& exp : indexs->indexCore.exps) {
            auto minmaxExp = exp.core.getMinMax();
            minmax.first = NumberCore::min(minmax.first, minmaxExp.first);
            minmax.second = NumberCore::max(minmax.second, minmaxExp.second);
        }
    }
    bool minmaxChanged = (minmax.first != context.model.min || minmax.second != context.model.max || cnt != context.model.cnt);
    if (minmaxChanged) {
        context.model.min = minmax.first;
        context.model.max = minmax.second;
        context.model.cnt = cnt;
    }

    if (!sizeChanged && !minmaxChanged && !force)
        return;

    // calc
    context.calc.rectXAxis.set(context.rectView.left(), rectView.height() - context.config.xAxisHeight, rectView.width(), context.config.xAxisHeight);
    context.calc.rectYAxis.set(rectView.width() - context.config.yAxisWidth, context.rectView.top(), context.config.yAxisWidth, rectView.height());
    context.calc.rectChart.set(
        context.rectView.left(),
        context.rectView.top(),
        rectView.width() - context.config.yAxisWidth,
        rectView.height() - context.config.xAxisHeight
    );
    context.calc.rectInnerChart.set(
        context.config.leftPadding,
        context.config.topPadding,
        rectView.width() - context.config.yAxisWidth - context.config.leftPadding - context.config.rightPadding,
        rectView.height() - context.config.xAxisHeight - context.config.topPadding - context.config.btmPadding
    );

    context.yCoordinate.init(
        context.model.min,
        context.model.max,
        context.calc.rectInnerChart.top(),
        context.calc.rectInnerChart.bottom()
    );

    // buffer
    if (context.main) {
        switch (context.config.klineType)
        {
        case EnKChartDrawingType::StickLine:
            context.kline.buffer.sticks = calcStickLine(stockCore);
            break;
        case EnKChartDrawingType::CloseLine:
            context.kline.buffer.lines = calcLine(stockCore->close);
            break;
        }
    }
    int indexCnt = indicators.size();
    context.indexs.resize(indexCnt);
    for (int i = 0; i < indexCnt; i += 1) {
        auto& indicator = indicators[i];
        auto& index = context.indexs[i];
        int expCnt = indicator->indexCore.exps.size();
        index.exps.resize(expCnt);
        for (int j = 0; j < expCnt; j += 1) {
            auto& indicatorExp = indicator->indexCore.exps[j];
            auto& exp = index.exps[j];

            switch (indicatorExp.drawing.type)
            {
            case EnDrawingType::None:
            case EnDrawingType::Number:
            case EnDrawingType::Text:
                exp.buffer.lines = calcLine(indicatorExp.core);
                break;
            case EnDrawingType::StickLine:
                // todo
                break;
            }
        }
    }
}

std::vector<Point> ChartAreaVM::calcLine(const NumberCore& core)
{
    auto& context = *m_context;
    const int cnt = core.size();
    std::vector<Point> points(cnt);
    for (int i = 0; i < cnt; i += 1) {
        const Real xPos = getXPos(i);
        const Real yPos = getYPos(core[i]);
        points[i].set(xPos, yPos);
    }
    return points;
}

std::vector<Stick> ChartAreaVM::calcStickLine(std::shared_ptr<const StockCore> stockCore)
{
    auto& context = *m_context;
    const int cnt = stockCore->getSize();
    std::vector<Stick> stickLine(cnt);
    for (int i = 0; i < cnt; i += 1) {
        const Real xPos = getXPos(i);
        const Number o = stockCore->open[i];
        const Number c = stockCore->close[i];
        const Real oPos = getYPos(o);
        const Real hPos = getYPos(stockCore->high[i]);
        const Real lPos = getYPos(stockCore->low[i]);
        const Real cPos = getYPos(c);
        stickLine[i].set(
            xPos,
            std::min(oPos, cPos),
            context.config.stickWidth,
            std::abs(oPos - cPos),
            hPos,
            lPos,
            (c - o)
        );
    }
    return stickLine;
}

Real ChartAreaVM::getXPos(int index)
{
    auto& context = *m_context;
    if (NumberCore::EmptyNumber == context.model.max || NumberCore::EmptyNumber == context.model.min)
        return 0;
    return context.calc.rectInnerChart.left()
        + (context.calc.rectInnerChart.width() * index) / context.model.cnt
        + (context.config.stickWidth / 2);
}

Real ChartAreaVM::getYPos(Number price)
{
    auto& context = *m_context;
    if (NumberCore::EmptyNumber == context.model.max || NumberCore::EmptyNumber == context.model.min)
        return 0;
    return context.yCoordinate.price2pos(price);
}

