/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "SubParser.h"

namespace StockCharts
{
    class DrawingParser : public SubParser
    {
    public:
        DrawingParser()
        {
            m_binds = {
                {"DRAWNUMBER", std::bind(&DrawingParser::number, this, std::placeholders::_1)},
                {"DRAWTEXT", std::bind(&DrawingParser::text, this, std::placeholders::_1)},
                {"STICKLINE", std::bind(&DrawingParser::stickLine, this, std::placeholders::_1)},
            };
        }

        bool check(const std::string &name)
        {
            return (m_binds.find(name) != m_binds.end());
        }

        std::tuple<bool, ExpDrawingType, NumberCore> process(const std::string &name, const std::vector<NumberCore> &inputs)
        {
            auto ite = m_binds.find(name);
            if (ite == m_binds.end())
                return emptyRet();
            F &f = ite->second;
            return f(inputs);
        }

    private:
        static std::tuple<bool, ExpDrawingType, NumberCore> emptyRet(bool ok = false)
        {
            return {ok, ExpDrawingType(), NumberCore()};
        }

        std::tuple<bool, ExpDrawingType, NumberCore> number(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 3)
                return emptyRet();

            const NumberCore &cond = inputs[0];
            const NumberCore &price = inputs[1];
            const NumberCore &num = inputs[2];

            if (cond.empty() || price.empty() || num.empty())
                return emptyRet(true);

            int rates_total = price.size();
            int num_total = num.size();
            NumberCore buffer(rates_total);

            for (int i = 0; i < rates_total; ++i)
            {
                Number b = cond.safeAt(i);
                if (b == 0.0 || b == NumberNull)
                    continue;
                buffer[i] = price[i];

                Number n = (num_total == 1 ? num[0] : num.safeAt(i));
                buffer.setOther(i, std::to_string(int64_t(n)));
            }

            ExpDrawingType drawingType;
            drawingType.type = EnDrawingType::Number;
            return {true, std::move(drawingType), buffer};
        }

        std::tuple<bool, ExpDrawingType, NumberCore> text(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 3)
                return emptyRet();

            const NumberCore &cond = inputs[0];
            const NumberCore &price = inputs[1];
            const NumberCore &text = inputs[2];

            if (cond.empty() || price.empty() || text.empty())
                return emptyRet(true);

            int rates_total = price.size();
            int text_total = text.size();
            NumberCore buffer(rates_total);

            ExpDrawingType drawingType;
            drawingType.type = EnDrawingType::Text;
            return {true, std::move(drawingType), buffer};
        }

        std::tuple<bool, ExpDrawingType, NumberCore> stickLine(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 5)
                return emptyRet();

            const NumberCore &cond = inputs[0];
            const NumberCore &price = inputs[1];
            const NumberCore &price2 = inputs[2];
            Number width = inputs[3].safeAt(0);
            Number empty = inputs[4].safeAt(0);

            if (cond.empty() || price.empty() || price2.empty() || width == NumberNull || empty == NumberNull)
                return emptyRet(true);

            int rates_total = std::max(price.size(), price2.size());
            NumberCore buffer(rates_total);

            for (int i = 0; i < rates_total; ++i)
            {
                Number b = cond.safeAt(i);
                if (!b || b == NumberNull)
                    continue;
                buffer[i] = price.safeAt(i);
                buffer.setOther(i, std::to_string(price2[i]));
            }

            ExpDrawingType drawingType;
            drawingType.type = EnDrawingType::Candlestick;
            drawingType.stickWidth = width;
            drawingType.stickEmpty = empty;
            return {true, std::move(drawingType), buffer};
        }

    private:
        using F = std::function<std::tuple<bool, ExpDrawingType, NumberCore>(const std::vector<NumberCore> &)>;
        std::map<std::string, F> m_binds;
    };
}
