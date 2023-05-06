/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "NumberCore.h"
#include <string>
#include <variant>
#include <map>
#include <set>

namespace StockCharts
{
    class DataBinding
    {
    public:
        DataBinding() = default;
        virtual ~DataBinding()
        {
            for (DataBinding* listener : listeners)
                listener->listening.erase(this);
            for (DataBinding* other : listening)
                other->listeners.erase(this);
        }

        void bind(DataBinding* other)
        {
            other->listeners.insert(this);
            this->listening.insert(other);
        }

        void unbind(DataBinding* other)
        {
            other->listeners.erase(this);
            this->listening.erase(other);
        }

        void fire(const std::string& id)
        {
            for (auto listener : listeners)
                listener->on(this, id);
        }

        virtual void on(DataBinding* sender, const std::string& id)
        {
            // inherit
        }

    private:
        std::set<DataBinding*> listeners;
        std::set<DataBinding*> listening;
    };
}

