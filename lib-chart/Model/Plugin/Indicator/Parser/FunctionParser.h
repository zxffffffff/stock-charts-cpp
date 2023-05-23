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
    class FunctionParser : public SubParser
    {
    public:
        FunctionParser()
        {
            m_binds = {
                {"MA", std::bind(&FunctionParser::MA, this, std::placeholders::_1)},
                {"EMA", std::bind(&FunctionParser::EMA, this, std::placeholders::_1)},
                {"SMA", std::bind(&FunctionParser::SMA, this, std::placeholders::_1)},
                {"LLV", std::bind(&FunctionParser::LLV, this, std::placeholders::_1)},
                {"LOWEST", std::bind(&FunctionParser::LLV, this, std::placeholders::_1)},
                {"HHV", std::bind(&FunctionParser::HHV, this, std::placeholders::_1)},
                {"HIGHEST", std::bind(&FunctionParser::HHV, this, std::placeholders::_1)},
                {"REF", std::bind(&FunctionParser::REF, this, std::placeholders::_1)},
                {"REFX", std::bind(&FunctionParser::REFX, this, std::placeholders::_1)},
                {"STD", std::bind(&FunctionParser::STD, this, std::placeholders::_1)},
                {"STDP", std::bind(&FunctionParser::STDP, this, std::placeholders::_1)},
                {"MAX", std::bind(&FunctionParser::processMAX, this, std::placeholders::_1)},
                {"MIN", std::bind(&FunctionParser::processMIN, this, std::placeholders::_1)},
                {"ABS", std::bind(&FunctionParser::processABS, this, std::placeholders::_1)},
                {"SUM", std::bind(&FunctionParser::processSUM, this, std::placeholders::_1)},
                {"BARSLASTCOUNT", std::bind(&FunctionParser::BARSLASTCOUNT, this, std::placeholders::_1)},
                {"BETWEEN", std::bind(&FunctionParser::BETWEEN, this, std::placeholders::_1)},
                {"BACKSET", std::bind(&FunctionParser::BACKSET, this, std::placeholders::_1)},
            };
        }

        bool check(const std::string &name)
        {
            return (m_binds.find(name) != m_binds.end());
        }

        std::tuple<bool, NumberCore> process(const std::string &name, const std::vector<NumberCore> &inputs)
        {
            auto ite = m_binds.find(name);
            if (ite == m_binds.end())
                return emptyRet();
            F &f = ite->second;
            return f(inputs);
        }

    private:
        static std::tuple<bool, NumberCore> emptyRet(bool ok = false)
        {
            return {ok, NumberCore()};
        }

        std::tuple<bool, NumberCore> MA(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 2)
                return emptyRet();

            const NumberCore &price = inputs[0];
            int period = inputs[1].safeAt(0);

            if (price.empty() || period < 0)
                return emptyRet(true);

            int rates_total = price.size();
            int prev_calculated = 0;
            int begin = 0;
            NumberCore buffer(rates_total);

            if (period <= 1)
            {
                buffer = price;
            }
            else if (rates_total - begin < period)
            {
                //
            }
            else
            {
                int start_position;
                if (prev_calculated == 0)
                {
                    start_position = period + begin;

                    for (int i = 0; i < start_position - 1; i++)
                        buffer[i] = NumberNull;

                    Number first_value = 0;
                    for (int i = begin; i < start_position; i++)
                        first_value += price[i];

                    buffer[start_position - 1] = first_value / period;
                }
                else
                {
                    start_position = prev_calculated - 1;
                }

                for (int i = start_position; i < rates_total; i++)
                    buffer[i] = buffer[i - 1] + (price[i] - price[i - period]) / period;
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> EMA(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 2)
                return emptyRet();

            const NumberCore &price = inputs[0];
            int period = inputs[1].safeAt(0);

            if (price.empty() || period < 0)
                return emptyRet(true);

            int rates_total = price.size();
            int prev_calculated = 0;
            int begin = 0;
            NumberCore buffer(rates_total);

            if (period <= 1)
            {
                buffer = price;
            }
            else if (rates_total - begin < period)
            {
                //
            }
            else
            {
                int i, limit;
                Number dSmoothFactor = 2.0 / (1.0 + period);
                if (prev_calculated == 0)
                {
                    limit = period + begin;

                    for (i = 0; i < begin; i++)
                        buffer[i] = 0.0;

                    buffer[begin] = price[begin];
                    for (i = begin + 1; i < limit; i++)
                        buffer[i] = price[i] * dSmoothFactor + buffer[i - 1] * (1.0 - dSmoothFactor);
                }
                else
                    limit = prev_calculated - 1;

                for (i = limit; i < rates_total; i++)
                    buffer[i] = price[i] * dSmoothFactor + buffer[i - 1] * (1.0 - dSmoothFactor);
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> SMA(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 3)
                return emptyRet();

            const NumberCore &price = inputs[0];
            int period = inputs[1].safeAt(0);
            int M = inputs[2].safeAt(0);

            if (price.empty() || period < 0 || M < 0)
                return emptyRet(true);

            int rates_total = price.size();
            int prev_calculated = 0;
            int begin = 0;
            NumberCore buffer(rates_total);

            Number Ypre = 0.0;
            Number Y = 0.0;
            Number N = (Number)period;
            int nStar = 0;
            for (long idx = nStar; idx < rates_total; idx++)
            {
                if (nStar == idx)
                {
                    Y = price[nStar];
                    if (Y == NumberNull)
                        Y = 0.0;
                    Ypre = Y;
                    buffer[idx] = Y;
                    continue;
                }
                Y = (price[idx] * M + Ypre * (N - M)) / N;
                Ypre = Y;
                buffer[idx] = Y;
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> LLV(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 2)
                return emptyRet();

            const NumberCore &price = inputs[0];
            int period = inputs[1].safeAt(0);

            if (price.empty() || period < 0)
                return emptyRet(true);

            int rates_total = price.size();
            int prev_calculated = 0;
            int begin = 0;
            NumberCore buffer(rates_total);

            auto LLV_Func = [](const NumberCore &price, int pos, int period) -> Number
            {
                const int sz = price.size();
                Number ret = price[pos];
                for (int i = 0; i < period; ++i)
                {
                    int idx = pos - i;
                    if (idx >= 0 && idx < sz)
                        ret = NumberCore::min(price[idx], ret);
                }
                return ret;
            };

            if (period <= 1)
            {
                buffer = price;
            }
            else
            {
                for (int i = 0; i < rates_total; ++i)
                    buffer[i] = LLV_Func(price, i, period);
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> HHV(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 2)
                return emptyRet();

            const NumberCore &price = inputs[0];
            int period = inputs[1].safeAt(0);

            if (price.empty() || period < 0)
                return emptyRet(true);

            int rates_total = price.size();
            int prev_calculated = 0;
            int begin = 0;
            NumberCore buffer(rates_total);

            auto HHV_Func = [](const NumberCore &price, int pos, int period) -> Number
            {
                const int sz = price.size();
                Number ret = price.safeAt(pos);
                for (int i = 0; i < period; ++i)
                {
                    int idx = pos - i;
                    if (idx >= 0 && idx < sz)
                        ret = NumberCore::max(price[idx], ret);
                }
                return ret;
            };

            if (period <= 1)
            {
                buffer = price;
            }
            else
            {
                for (int i = 0; i < rates_total; ++i)
                    buffer[i] = HHV_Func(price, i, period);
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> REF(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 2)
                return emptyRet();

            const NumberCore &price = inputs[0];
            int period = inputs[1].safeAt(0);

            if (price.empty() || period < 0)
                return emptyRet(true);

            int rates_total = price.size();
            int prev_calculated = 0;
            int begin = 0;
            NumberCore buffer(rates_total);

            if (period < 1)
            {
                buffer = price;
            }
            else
            {
                Number val = price.safeAt(0);
                for (int i = 0; i < rates_total; ++i)
                {
                    int offset = i - period;
                    if (offset >= 0 && offset < rates_total)
                        val = price[offset];
                    buffer[i] = val;
                }
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> REFX(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 2)
                return emptyRet();

            const NumberCore &price = inputs[0];
            int period = inputs[1].safeAt(0);

            if (price.empty() || period < 0)
                return emptyRet(true);

            int rates_total = price.size();
            int prev_calculated = 0;
            int begin = 0;
            NumberCore buffer(rates_total);

            if (period < 1)
            {
                buffer = price;
            }
            else
            {
                Number val = price.safeAt(0);
                for (int i = 0; i < rates_total; ++i)
                {
                    int offset = i + period;
                    if (offset >= 0 && offset < rates_total)
                        val = price[offset];
                    buffer[i] = val;
                }
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> STD(const std::vector<NumberCore> &inputs)
        {
            // zxf todo
            return STDP(inputs);
        }

        std::tuple<bool, NumberCore> STDP(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 2)
                return emptyRet();

            const NumberCore &price = inputs[0];
            int period = inputs[1].safeAt(0);

            if (price.empty() || period < 0)
                return emptyRet(true);

            int rates_total = price.size();
            int prev_calculated = 0;
            int begin = 0;
            NumberCore buffer(rates_total);

            auto StdDev_Func = [](const int position, const NumberCore &price, const NumberCore &ma_price, const int period)
            {
                Number std_dev = 0.0;
                if (position >= period - 1)
                {
                    for (int i = 0; i < period; i++)
                        std_dev += std::pow(price[position - i] - ma_price[position], 2.0);
                    std_dev = std::sqrt(std_dev / period);
                }
                return (std_dev);
            };

            if (period <= 1)
            {
                buffer = price;
            }
            else
            {
                NumberCore MAprice;
                bool ok;
                std::tie(ok, MAprice) = MA({price, Number(period)});
                if (!ok)
                    return emptyRet();

                for (int i = 0; i < rates_total; ++i)
                    buffer[i] = StdDev_Func(i, price, MAprice, period);
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> processMAX(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() < 2)
                return emptyRet();

            NumberCore buffer = inputs[0];
            for (int i = 1; i < inputs.size(); ++i)
            {
                buffer.max(inputs[i]);
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> processMIN(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() < 2)
                return emptyRet();

            NumberCore buffer = inputs[0];
            for (int i = 1; i < inputs.size(); ++i)
            {
                buffer.min(inputs[i]);
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> processABS(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 1)
                return emptyRet();

            NumberCore buffer = inputs[0];
            for (int i = 0; i < buffer.size(); ++i)
            {
                const Number d = buffer[i];

                if (d != NumberNull)
                    buffer[i] = std::abs(d);
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> processSUM(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 2)
                return emptyRet();

            const NumberCore &price = inputs[0];
            int period = inputs[1].safeAt(0);

            if (price.empty() || period < 0)
                return emptyRet(true);

            int rates_total = price.size();
            int prev_calculated = 0;
            int begin = 0;
            NumberCore buffer(rates_total);

            if (period < 1)
            {
                buffer = price;
            }
            else
            {
                for (int i = 0; i < rates_total; ++i)
                {
                    Number val = 0.0;
                    for (int j = 0; j < period; ++j)
                    {
                        int offset = i - j;
                        if (offset >= 0 && offset < rates_total)
                            val += price[offset];
                    }
                    buffer[i] = val;
                }
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> BARSLASTCOUNT(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 1)
                return emptyRet();

            const NumberCore &core = inputs[0];

            int rates_total = core.size();
            NumberCore buffer(rates_total, 0.0);

            int count = 0;
            for (int i = 0; i < rates_total; ++i)
            {
                if (core[i])
                    buffer[i] = ++count;
                else
                    count = 0;
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> BETWEEN(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 3)
                return emptyRet();

            const NumberCore &core = inputs[0];
            Number min = inputs[1].safeAt(0);
            Number max = inputs[2].safeAt(0);

            int rates_total = core.size();
            NumberCore buffer(rates_total, 0.0);

            for (int i = 0; i < rates_total; ++i)
            {
                if (core[i] >= min && core[i] <= max)
                    buffer[i] = 1.0;
            }
            return {true, buffer};
        }

        std::tuple<bool, NumberCore> BACKSET(const std::vector<NumberCore> &inputs)
        {
            if (inputs.size() != 2)
                return emptyRet();

            const NumberCore &core = inputs[0];
            int val = inputs[1].safeAt(0);

            int rates_total = core.size();
            NumberCore buffer(rates_total, 0.0);

            for (int i = 0; i < rates_total; ++i)
            {
                if (core[i])
                {
                    for (int j = 0; j < val; ++j)
                        buffer.safeAt(i - j) = 1.0;
                }
            }
            return {true, buffer};
        }

    private:
        using F = std::function<std::tuple<bool, NumberCore>(const std::vector<NumberCore> &)>;
        std::map<std::string, F> m_binds;
    };
}
