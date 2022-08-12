/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "ChartPlugin.h"
#include <array>

namespace StockCharts
{
    class PluginCrossLine : public ChartPlugin
    {
    public:
        PluginCrossLine(std::shared_ptr<const StockCore> stockCore);

        // [2]
        virtual void onContextChanged(std::shared_ptr<const ChartContext> context) override;
        virtual void onMouseMove(std::shared_ptr<const ChartContext> context) override;
        virtual void onPaint(std::shared_ptr<const ChartContext> context, Painter& painter) override;

    private:
        Line crossLineX;
        Rect crossXBG;
        std::string crossXText;

        Line crossLineY;
        Rect crossYLBG;
        Rect crossYRBG;
        std::string crossYLText;
        std::string crossYRText;
    };
}

