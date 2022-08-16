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
#include "Context/ChartProps.h"
#include "Plugins/ChartPlugin.h"

namespace StockCharts
{
    constexpr inline char ID_StockCoreChanged[] = "ID_StockCoreChanged";
    constexpr inline char ID_ChartPropsChanged[] = "ID_ChartPropsChanged";

    class ChartModel : public DataBinding
    {
    public:
        ChartModel(std::shared_ptr<StockCore> stockCore)
            : m_stockCore(stockCore)
            , m_props(std::make_shared<ChartProps>())
        {
            assert(m_stockCore);
        }
        virtual ~ChartModel() = default;

        virtual void on(DataBinding* sender, const std::string& id) override
        {
            if (id == ID_ChartPluginChanged)
                fire(id);
        }

        // [0]
        std::shared_ptr<const StockCore> getStockCore() const
        {
            return m_stockCore;
        }

        void setStockCore(const StockCore& stockCore)
        {
            *m_stockCore = stockCore;
            fire(ID_StockCoreChanged);
        }

        // [1]
        std::shared_ptr<const ChartProps> getProps() const
        {
            return m_props;
        }

        void setProps(const ChartProps& props)
        {
            *m_props = props;
            fire(ID_ChartPropsChanged);
        }

        // [2]
        template<typename T = ChartPlugin, typename... Args>
        std::shared_ptr<T> addPlugin(Args&&... args)
        {
            assert(!getPlugin<T>());
            auto plugin = std::make_shared<T>(getStockCore(), getProps(), args...);
            m_plugins.push_back(plugin);
            bind(plugin.get());
            return getPlugin<T>();
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

        const std::vector<std::shared_ptr<ChartPlugin>>& getPlugins()
        {
            return m_plugins;
        }

    private:
        std::shared_ptr<StockCore> m_stockCore;
        std::shared_ptr<ChartProps> m_props;
        std::vector<std::shared_ptr<ChartPlugin>> m_plugins;
    };
}

