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
#include "Plugins/ChartPlugin.h"
#include <cassert>

namespace StockCharts
{
    constexpr inline char ID_StockCoreChanged[] = "ID_StockCoreChanged";

    class ChartModel : public DataBinding
    {
    public:
        ChartModel(std::shared_ptr<StockCore> stockCore)
            : m_stockCore(stockCore)
        {
            assert(m_stockCore);
        }
        virtual ~ChartModel() = default;

        const std::shared_ptr<StockCore>& getStockCore() const
        {
            return m_stockCore;
        }

        template<typename T = ChartPlugin, typename... Args>
        std::shared_ptr<T> addPlugin(Args&&... args)
        {
            assert(!getPlugin<T>());
            auto plugin = std::make_shared<T>(getStockCore(), args...);
            m_plugins.push_back(plugin);
            return getPlugin<T>();
        }

        const std::vector<std::shared_ptr<ChartPlugin>>& getPlugins()
        {
            return m_plugins;
        }

        template<typename T = ChartPlugin>
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

    private:
        std::shared_ptr<StockCore>  m_stockCore;
        std::vector<std::shared_ptr<ChartPlugin>> m_plugins;
    };
}

