/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include "../Core/StockCore.h"
#include "Context/ChartContext.h"

namespace StockCharts
{
    class ChartModel;
    class ChartVM
    {
    public:
        ChartVM(std::shared_ptr<ChartModel> model);

        // [0]
        void setDrawingType(EnKLineType type);
        void setCorrdinate(EnCoordinateType type);
        void setYLWidth(Real i);
        void setYRWidth(Real i);
        void setXHeight(Real i);
        void setPaddingLeft(Real i);
        void setPaddingTop(Real i);
        void setPaddingRight(Real i);
        void setPaddingBottom(Real i);
        void setNodeStickWidth(Real nodeWidth, Real stickWidth);

        // [1]
        void calcContext();
        void calcPlugins();

        // [2]
        void OnResize(const Rect& rect);
        void OnPaint(Painter& painter);
        void OnMouseMove(const Point& point);
        void OnMouseLeave();
        void OnScrollX(int step);
        void OnWheelY(int step);

        void paintPlugins(Painter& painter);

    private:
        std::shared_ptr<ChartModel> m_model;
        std::shared_ptr<ChartContext> m_context;
    };
}

