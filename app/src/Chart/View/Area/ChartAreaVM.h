/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include "../../../Core/StockCore.h"
#include "../../../Core/Graphics.h"
#include "../../../Core/Painter.h"
#include "../../Model/Area/ChartAreaContext.h"

namespace StockCharts
{
    class ChartModel;
    class ChartAreaVM
    {
    public:
        ChartAreaVM(std::shared_ptr<ChartModel> model);

        // [0]
        void setViewSize(int beginIndex, int endIndex);

        // [1]
        void calcContext(Rect rectView);
        void calcMinMax();
        void calcCoordinate();
        void calcPlugins();

        // [2]
        void paintPlugins(Painter& painter);

    private:
        std::shared_ptr<ChartModel> m_model;
        std::shared_ptr<ChartAreaContext> m_context;
    };
}

