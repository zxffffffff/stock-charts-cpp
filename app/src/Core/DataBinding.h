/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "NumberCore.h"
#include "Graphics.h"
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
            for (DataBinding* data : datas)
                data->listeners.erase(this);
            for (DataBinding* listener : listeners)
                listener->datas.erase(this);
        }

        void bind(DataBinding* data)
        {
            this->datas.insert(data);
            data->listeners.insert(this);
        }

        void unbind(DataBinding* data)
        {
            this->datas.erase(data);
            data->listeners.erase(this);
        }

        void fire(const std::string& id)
        {
            for (auto listener : listeners) {
                listener->on(this, id);
            }
        }

        virtual void on(DataBinding* sender, const std::string& id)
        {
            // inherit
        }

    private:
        std::set<DataBinding*> datas;
        std::set<DataBinding*> listeners;
    };
}

