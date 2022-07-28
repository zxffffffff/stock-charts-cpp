#pragma once
#include "../../Core/Utils.h"
#include "../../Core/StockCore.h"

namespace StockCharts
{
    struct StKChartContext
    {
        // view
        Real x = 0;
        Real y = 0;
        Real width = 0;
        Real height = 0;
        Real xAxisHeight = 30;
        Real yAxisWidth = 50;
        Real topPadding = 10;
        Real btmPadding = 10;
        Real leftPadding = 0;
        Real rightPadding = 0;
        Real stickWidth = 5;
        // model
        int cnt = 0;
        Number min = 0.0;
        Number max = 0.0;
        // calc
        Rect rectXAxis;
        Rect rectYAxis;
        Rect rectChart;
        Rect klineChart;
        std::vector<Stick> kline;
    };
    using SpKChartContext = std::shared_ptr<StKChartContext>;
    using CpKChartContext = std::shared_ptr<const StKChartContext>;

    class KChartModel;
    class KChartBodyVM
    {
    public:
        KChartBodyVM();

        // [0]
        void setModel(std::shared_ptr<KChartModel> model);

        // [1]
        void calcContext(SpKChartContext context, Real x, Real y, Real width, Real height);

        // [2]


        Real getXPos(const StKChartContext& buffer, int index) const;
        Real getYPos(const StKChartContext& buffer, Number price) const;

    private:
        std::shared_ptr<KChartModel> m_pKChartModel;
        SpKChartContext context;
    };
    using SpKChartBodyVM = std::shared_ptr<KChartBodyVM>;
}

