/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "PluginKLine.h"

using namespace StockCharts;

PluginKLine::PluginKLine(std::shared_ptr<const StockCore> stockCore, std::shared_ptr<const ChartProps> props)
    : PluginLayer(stockCore, props)
{
}

std::pair<Number, Number> PluginKLine::getMinMax(int beginIndex, int endIndex)
{
    return m_stockCore->getMinMax(beginIndex, endIndex);
}

void PluginKLine::onCalcArea(std::shared_ptr<const ChartAreaContext> context)
{
    m_areaIndexs.resize(1);
    m_areaIndexs[0].exps.resize(1);

    switch (m_config.klineType)
    {
    case EnKLineType::CandleStick:
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

void PluginKLine::onPaintArea(Painter& painter)
{
    if (m_areaIndexs.size() != 1 || m_areaIndexs[0].exps.size() != 1)
        return;
    auto& exp = m_areaIndexs[0].exps[0];

    switch (exp.type)
    {
    case EnChartAreaExpType::Stick:
        painter.drawStick(exp.sticks, Color(200, 0, 0), Color(0, 200, 0));
        break;
    case EnChartAreaExpType::Line:
        painter.drawLines(exp.lines, Color(100, 100, 200));
        break;
    }
}

void PluginKLine::setKLineType(EnKLineType type)
{
    m_config.klineType = type;
}

void PluginKLine::onStockCoreChanged()
{
}
