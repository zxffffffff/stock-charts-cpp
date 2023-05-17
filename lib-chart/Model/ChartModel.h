/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../Core/StockCore.h"
#include "../Core/DataBinding.h"
#include "ChartPlugin.h"

namespace StockCharts
{
    constexpr inline char ID_StockCoreChanged[] = "ID_StockCoreChanged";

    class ChartModel : public DataBinding
    {
    private:
        // [0]
        std::shared_ptr<StockCore> m_stockCore;

        // [1]
        std::vector<std::shared_ptr<ChartPlugin>> m_plugins;

    public:
        ChartModel(std::shared_ptr<StockCore> stockCore)
            : m_stockCore(stockCore)
        {
            assert(m_stockCore);
        }
        virtual ~ChartModel() = default;

        virtual void on(DataBinding *sender, const std::string &id) override
        {
            if (id == ID_ChartPluginChanged)
                fire(id);
        }

        // [0]
        std::shared_ptr<const StockCore> getStockCore() const
        {
            return m_stockCore;
        }

        void setStockCore(const StockCore &stockCore)
        {
            *m_stockCore = stockCore;
            for (auto &plugin : m_plugins)
                plugin->onStockCoreChanged(m_stockCore);
            fire(ID_StockCoreChanged);
        }

        // [1]
        template <typename T = ChartPlugin, typename... Args>
        std::shared_ptr<T> addPlugin(Args &&...args)
        {
            assert(!getPlugin<T>());
            auto plugin = std::make_shared<T>(args...);
            m_plugins.push_back(plugin);
            listen(plugin.get());
            plugin->init(m_stockCore);
            return getPlugin<T>();
        }

        template <typename T = ChartPlugin>
        std::shared_ptr<T> getPlugin()
        {
            for (auto &plugin : m_plugins)
            {
                // RTTI
                auto derived = std::dynamic_pointer_cast<T>(plugin);
                if (derived)
                    return derived;
            }
            return nullptr;
        }

        template <typename T = ChartPlugin>
        std::shared_ptr<const T> getPlugin() const
        {
            for (auto &plugin : m_plugins)
            {
                // RTTI
                auto derived = std::dynamic_pointer_cast<const T>(plugin);
                if (derived)
                    return derived;
            }
            return nullptr;
        }

        const std::vector<std::shared_ptr<ChartPlugin>> &getPlugins()
        {
            return m_plugins;
        }
    };
}
