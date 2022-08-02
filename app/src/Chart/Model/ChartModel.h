/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include "../Plugin/PluginLayer.h"

namespace StockCharts
{
    class PluginLayer;
    struct ChartProps;

    class ChartModel
    {
    public:
        ChartModel(std::shared_ptr<StockCore> stockCore = nullptr, std::shared_ptr<ChartProps> props = nullptr);
        virtual ~ChartModel();

        // [0]
        template<typename T = PluginLayer, typename... Args>
        void addPlugin(Args&&... args)
        {
            auto plugin = std::make_shared<T>(getStockCore(), getProps(), args...);
            m_plugins.push_back(plugin);
        }

        const std::vector<std::shared_ptr<PluginLayer>>& getPlugins()
        {
            return m_plugins;
        }

        template<typename T = PluginLayer>
        std::shared_ptr<T> getPlugin()
        {
            for (auto& plugin : m_plugins) {
                // RTTI
                auto derived = std::dynamic_pointer_cast<T>(plugin);
                if (derived)
                    return derived;
            }
            return nullptr;
        }

        // [1]
        void setStockCore(const StockCore& stockCore);
        void setProps(const ChartProps& props);

        std::shared_ptr<const StockCore> getStockCore() const
        {
            return m_stockCore;
        }
        std::shared_ptr<const ChartProps> getProps() const
        {
            return m_props;
        }

    private:
        std::vector<std::shared_ptr<PluginLayer>> m_plugins;

        std::shared_ptr<StockCore> m_stockCore;
        std::shared_ptr<ChartProps> m_props;
    };
}

