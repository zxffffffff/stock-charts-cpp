/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "ChartAreaVM.h"
#include "../../Model/ChartModel.h"
#include "../../Plugin/PluginLayer.h"

using namespace StockCharts;

ChartAreaVM::ChartAreaVM(std::shared_ptr<ChartModel> model)
    : m_model(model)
    , m_context(std::make_shared<ChartAreaContext>())
{
}

void ChartAreaVM::setViewSize(int _beginIndex, int _endIndex)
{
    auto& context = *m_context;
    context.beginIndex = _beginIndex;
    context.beginIndex = _beginIndex;
    context.endIndex = _endIndex;
    context.viewCount = _endIndex - _beginIndex;
}

void ChartAreaVM::calcContext(Rect rectView)
{
    auto stockCore = m_model->getStockCore();
    const auto& props = *m_model->getProps();
    const auto& plugins = m_model->getPlugins();
    auto& ctx = *m_context;

    ctx.rectView = rectView;
    ctx.rectXAxis.set(ctx.rectView.left(), rectView.height() - props.xAxisHeight, rectView.width(), props.xAxisHeight);
    ctx.rectYAxis.set(rectView.width() - props.yAxisWidth, ctx.rectView.top(), props.yAxisWidth, rectView.height());
    ctx.rectChart.set(
        ctx.rectView.left(),
        ctx.rectView.top(),
        rectView.width() - props.yAxisWidth,
        rectView.height() - props.xAxisHeight
    );
    ctx.rectInnerChart.set(
        props.leftPadding,
        props.topPadding,
        rectView.width() - props.yAxisWidth - props.leftPadding - props.rightPadding,
        rectView.height() - props.xAxisHeight - props.topPadding - props.btmPadding
    );

    calcMinMax();
    calcCoordinate();
    calcPlugins();
}

void ChartAreaVM::calcMinMax()
{
    auto& ctx = *m_context;
    const auto& plugins = m_model->getPlugins();

    ctx.minPrice = NumberCore::EmptyNumber;
    ctx.maxPrice = NumberCore::EmptyNumber;
    for (const auto& plugin : plugins) {
        auto minmax = plugin->getMinMax(ctx.beginIndex, ctx.endIndex);
        ctx.minPrice = NumberCore::min(minmax.first, ctx.minPrice);
        ctx.maxPrice = NumberCore::max(minmax.second, ctx.maxPrice);
    }
}

void ChartAreaVM::calcCoordinate()
{
    auto& ctx = *m_context;
    const auto& props = *m_model->getProps();

    ctx.coordinate.init(
        ctx.rectInnerChart,
        ctx.minPrice,
        ctx.maxPrice,
        ctx.viewCount,
        props.nodeWidth
    );
}

void ChartAreaVM::calcPlugins()
{
    const auto& plugins = m_model->getPlugins();

    for (const auto& plugin : plugins) {
        plugin->onCalcArea(m_context);
    }
}

void StockCharts::ChartAreaVM::paintPlugins(Painter& painter)
{
    const auto& plugins = m_model->getPlugins();

    for (const auto& plugin : plugins) {
        painter.save();
        plugin->onPaintArea(painter);
        painter.restore();
    }
}
