/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include "../Core/StockCore.h"
#include "../Core/Graphics.h"
#include "../Core/Painter.h"
#include "Context/ChartContext.h"

namespace StockCharts
{
    class ChartModel;
    class ChartVM
    {
    public:
        ChartVM(std::shared_ptr<ChartModel> model);

        // [0]

        // [1]
        void calcContext();
        void calcPlugins();

        // [2]
        void OnResize(const Rect& rect);
        void OnPaint(Painter& painter);
        void OnMouseMove(const Point& point);
        void OnMouseLeave();

        void paintPlugins(Painter& painter);

    private:
        std::shared_ptr<ChartModel> m_model;
        std::shared_ptr<ChartContext> m_context;
    };
}

