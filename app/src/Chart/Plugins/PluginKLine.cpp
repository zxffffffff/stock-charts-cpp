/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "PluginKLine.h"

using namespace StockCharts;

PluginKLine::PluginKLine(std::shared_ptr<const StockCore> stockCore)
    : ChartPlugin(stockCore)
{
}

std::pair<Number, Number> PluginKLine::getMinMax(int beginIndex, int endIndex)
{
    return m_stockCore->getMinMax(beginIndex, endIndex);
}

void PluginKLine::onContextChanged(std::shared_ptr<const ChartContext> context)
{
    auto& ctx = *context;
    m_areaIndexs.resize(1);
    m_areaIndexs[0].exps.resize(1);

    m_klineType = ctx.props.klineType;
    switch (m_klineType)
    {
    case EnKLineType::CandleStickHollow:
    case EnKLineType::CandleStick:
    case EnKLineType::BAR:
    {
        m_areaIndexs[0].exps[0] = createStickExp(
            context,
            m_stockCore->open,
            m_stockCore->high,
            m_stockCore->low,
            m_stockCore->close
        );
    } break;
    case EnKLineType::CloseLine:
    {
        m_areaIndexs[0].exps[0] = createLineExp(
            context,
            m_stockCore->close
        );
    } break;
    }
}

void PluginKLine::onPaint(std::shared_ptr<const ChartContext> context, Painter& painter)
{
    if (m_areaIndexs.size() != 1 || m_areaIndexs[0].exps.size() != 1)
        return;
    auto& exp = m_areaIndexs[0].exps[0];

    switch (m_klineType)
    {
    case EnKLineType::CandleStickHollow:
        painter.drawStickHollow(exp.sticks, Color(200, 0, 0), Color(0, 200, 0));
        break;
    case EnKLineType::CandleStick:
        painter.drawStick(exp.sticks, Color(200, 0, 0), Color(0, 200, 0));
        break;
    case EnKLineType::BAR:
        painter.drawBAR(exp.sticks, Color(200, 0, 0), Color(0, 200, 0));
        break;
    case EnKLineType::CloseLine:
        painter.drawPath(exp.lines, Color(100, 100, 200));
        break;
    }
}

void PluginKLine::onStockCoreChanged()
{
}
