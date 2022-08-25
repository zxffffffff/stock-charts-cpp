/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../ChartLayer.h"
#include "../../Graphics/Widget.h"
#include <vector>

namespace StockCharts
{
    using ChartTitleItemFlags = int;
    enum ChartTitleItemFlag : int
    {
        ChartTitleItemFlagNull = 0x0,
        ChartTitleItemFlagStock = 0x1,
        ChartTitleItemFlagIndicator = 0x2,
    };

    struct ChartTitleItem
    {
        Widget btnSetting;
        Widget btnClose;
        std::vector<Widget> items;
    };

    class LayerTitle : public ChartLayer
    {
    public:
        LayerTitle(std::shared_ptr<const ChartModel> model, std::shared_ptr<const ChartProps> props, std::shared_ptr<const ChartContext> context,
            ChartTitleItemFlags flags)
            : ChartLayer(model, props, context)
            , m_flags(flags)
        {
        }
        virtual ~LayerTitle() = default;

        virtual void onContextChanged() override
        {
            onMouseMove();
        }

        virtual void onMouseMove() override
        {
            auto stockCore = *m_model->getStockCore();
            auto pluginIndicator = m_model->getPlugin<PluginIndicator>();
            const auto& ctx = *m_context;
            const auto& props = *m_props;

            int index = ctx.hoverNormal.index;
            if (index < 0)
                index = ctx.hoverSync.index;
            if (index < 0)
                index = stockCore.getSize() - 1;
            if (index < 0)
                return;

            const Real itemWidth = 80;
            const Real rowHeight = 24;

            m_rows.clear();

            if (m_flags & ChartTitleItemFlagStock)
            {
                ChartTitleItem titItem;
                titItem.items.resize(4);
                for (int i = 0; i < titItem.items.size(); i++) {
                    auto& item = titItem.items[i];
                    bool rise = stockCore.close.safeAt(index) > stockCore.open.safeAt(index);
                    switch (i)
                    {
                    case 0:
                        item.text = "open:" + NumberUtils::toString(stockCore.open.safeAt(index), props.precision);
                        break;
                    case 1:
                        item.text = "high:" + NumberUtils::toString(stockCore.high.safeAt(index), props.precision);
                        break;
                    case 2:
                        item.text = "low:" + NumberUtils::toString(stockCore.low.safeAt(index), props.precision);
                        break;
                    case 3:
                        item.text = "close:" + NumberUtils::toString(stockCore.close.safeAt(index), props.precision);
                        break;
                    }
                    item.font.dir = PaintDirection::CenterLeft;
                    item.rect.set(
                        ctx.rectChart.left() + 1 + (i * itemWidth),
                        ctx.rectChart.top() + 1,
                        itemWidth - 1,
                        rowHeight);
                    item.rect.clipInside(ctx.rectChart);
                    item.bg.normal.colorBorder = rise ? props.riseColor : props.fallColor;
                    item.bg.normal.colorBG = item.bg.normal.colorBorder;
                    item.bg.normal.colorBG.a = 25;
                }
                m_rows.push_back(std::move(titItem));
            }

            if ((m_flags & ChartTitleItemFlagIndicator) && pluginIndicator) {
                auto indicators = pluginIndicator->getIndicators();
                for (auto& indicator : indicators) {
                    ChartTitleItem titItem;
                    titItem.items.resize(1 + indicator->indexCore.exps.size());
                    {
                        auto& item = titItem.items[0];
                        item.text = indicator->formula.name;
                        item.font.dir = PaintDirection::CenterLeft;
                        item.rect.set(
                            ctx.rectChart.left() + 1,
                            ctx.rectChart.top() + 1 + (m_rows.size() * rowHeight),
                            itemWidth - 1,
                            rowHeight);
                        item.rect.clipInside(ctx.rectChart);
                        item.bg.normal.colorBorder = ColorDarkGray;
                        item.bg.normal.colorBG = item.bg.normal.colorBorder;
                        item.bg.normal.colorBG.a = 25;
                    }
                    for (int i = 0; i < indicator->indexCore.exps.size(); i++) {
                        auto& exp = indicator->indexCore.exps[i];
                        int iItem = i + 1;
                        auto& item = titItem.items[iItem];
                        item.text = exp.info.rename + ":" + NumberUtils::toString(exp.core[index], props.precision);
                        item.font.dir = PaintDirection::CenterLeft;
                        item.rect.set(
                            ctx.rectChart.left() + 1 + (iItem * itemWidth),
                            ctx.rectChart.top() + 1 + (m_rows.size() * rowHeight),
                            itemWidth - 1,
                            rowHeight);
                        item.rect.clipInside(ctx.rectChart);
                        item.bg.normal.colorBorder = Color(exp.colorType.color.c_str());
                        item.bg.normal.colorBG = item.bg.normal.colorBorder;
                        item.bg.normal.colorBG.a = 25;
                    }
                    m_rows.push_back(std::move(titItem));
                }
            }
        }

        virtual void onPaint(Painter& painter) override
        {
            for (auto& row : m_rows) {
                row.btnSetting.paint(painter);
                row.btnClose.paint(painter);
                for (auto& item : row.items)
                    item.paint(painter);
            }
        }

    private:
        ChartTitleItemFlags m_flags;
        std::vector<ChartTitleItem> m_rows;
    };
}

