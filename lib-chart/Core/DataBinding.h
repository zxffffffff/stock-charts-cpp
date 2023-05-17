/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
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
            for (auto it = listeners.begin(); it != listeners.end(); ++it)
                (*it)->senders.erase(this);
            for (auto it = senders.begin(); it != senders.end(); ++it)
                (*it)->listeners.erase(this);
        }

        void bind(DataBinding *sender)
        {
            sender->listeners.insert(this);
            this->senders.insert(sender);
        }

        void unbind(DataBinding *sender)
        {
            sender->listeners.erase(this);
            this->senders.erase(sender);
        }

        void fire(const std::string &id)
        {
            for (DataBinding *listener : listeners)
                listener->on(this, id);
        }

        virtual void on([[maybe_unused]] DataBinding *sender, [[maybe_unused]] const std::string &id)
        {
            // inherit
        }

        const std::set<DataBinding *> &getListeners() const
        {
            return listeners;
        }

        const std::set<DataBinding *> &getSenders() const
        {
            return senders;
        }

    private:
        std::set<DataBinding *> listeners;
        std::set<DataBinding *> senders;
    };
}
