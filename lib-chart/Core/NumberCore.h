/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include <limits>
#include <string>
#include <vector>
#include <functional>
#include <utility>
#include <cmath>
#include <cfloat>

namespace StockCharts
{
    using Number = double;
    constexpr inline Number NumberNull = std::numeric_limits<Number>::lowest();

    class NumberCore
    {
    private:
        std::vector<Number> data;
        std::vector<std::string> other;

    public:
        NumberCore() = default;
        NumberCore(const NumberCore &) = default;
        NumberCore(NumberCore &&) = default;
        NumberCore(const Number val)
            : data({val})
        {
        }
        NumberCore(int cnt, const Number val = NumberNull)
            : data(cnt, val)
        {
        }
        NumberCore(std::initializer_list<Number> list)
            : data(list)
        {
        }
        NumberCore(const std::vector<Number> &v, bool reverse = false)
            : data(v)
        {
            if (reverse)
                this->reverse();
        }
        NumberCore(std::vector<Number> &&v)
            : data(std::move(v))
        {
        }
        NumberCore(std::vector<Number> &&v, std::vector<std::string> &&other)
            : data(std::move(v)), other(std::move(other))
        {
        }
        ~NumberCore() = default;

        bool empty() const
        {
            return this->data.empty();
        }
        int size() const
        {
            return this->data.size();
        }
        void resize(int cnt, const Number val = NumberNull)
        {
            this->data.resize(cnt, val);
        }
        void clear()
        {
            this->data.clear();
            this->other.clear();
        }

        Number &at(int i)
        {
            return this->data[i];
        }
        Number at(int i) const
        {
            return this->data[i];
        }
        Number &safeAt(int i)
        {
            if (i < 0 || i >= this->data.size())
            {
                static Number d;
                d = NumberNull;
                return d;
            }
            return this->data[i];
        }
        Number safeAt(int i) const
        {
            if (i < 0 || i >= this->data.size())
                return NumberNull;
            return this->data[i];
        }

        Number &front()
        {
            return this->data.front();
        }
        Number front() const
        {
            return this->data.front();
        }

        Number &back()
        {
            return this->data.back();
        }
        Number back() const
        {
            return this->data.back();
        }

        void fillBegin(int count, const Number newVal)
        {
            fill(0, count, newVal);
        }
        void fillEnd(int count, const Number newVal)
        {
            fill(size() - count, count, newVal);
        }
        void fill(int begin, int count, const Number newVal)
        {
            const int sz = this->data.size();
            for (int i = 0; i < count; ++i)
            {
                int pos = begin + i;
                if (pos < 0 || pos >= sz)
                    continue;
                this->data[pos] = newVal;
            }
        }

        void replace(const Number oldVal, const Number newVal)
        {
            if (oldVal == newVal)
                return;
            for (auto ite = this->data.begin(); ite != this->data.end(); ++ite)
            {
                if (*ite == oldVal)
                    *ite = newVal;
            }
        }
        void replaceNotNumber(const Number newVal)
        {
            for (auto ite = this->data.begin(); ite != this->data.end(); ++ite)
            {
                if (std::isnan(*ite) || std::isinf(*ite))
                    *ite = newVal;
            }
        }
        void replaceEmptyValue(const Number newVal)
        {
            replace(NumberNull, newVal);
        }

        void swap(NumberCore &rhs)
        {
            std::swap(this->data, rhs.data);
            std::swap(this->other, rhs.other);
        }
        void reverse()
        {
            std::reverse(this->data.begin(), this->data.end());
            std::reverse(this->other.begin(), this->other.end());
        }

        std::pair<Number, Number> getMinMax(int beginIndex, int endIndex) const
        {
            const int len = size();
            if (len == 0 || beginIndex < 0 || endIndex < 0 || beginIndex >= len || endIndex > len)
                return {NumberNull, NumberNull};
            Number min = this->data[beginIndex];
            Number max = min;
            for (int i = beginIndex + 1; i < endIndex; i += 1)
            {
                const auto &n = this->data[i];
                min = NumberCore::min(min, n);
                max = NumberCore::max(max, n);
            }
            return {min, max};
        }

        static Number max(const Number lhs, const Number rhs)
        {
            if (lhs == NumberNull)
                return rhs;
            else if (rhs == NumberNull)
                return lhs;
            if (lhs > rhs) //
                return lhs;
            return rhs;
        }
        static Number min(const Number lhs, const Number rhs)
        {
            if (lhs == NumberNull)
                return rhs;
            else if (rhs == NumberNull)
                return lhs;
            if (lhs < rhs) //
                return lhs;
            return rhs;
        }
        static Number abs(const Number val)
        {
            if (val == NumberNull)
                return val;
            if (val < 0)
                return -val;
            return val;
        }

        NumberCore &max(const NumberCore &rhs)
        {
            auto comp = [this](Number l, Number r) -> Number
            {
                return max(l, r);
            };
            *this = operatorFunc(*this, rhs, comp);
            return *this;
        }
        NumberCore &min(const NumberCore &rhs)
        {
            auto comp = [this](Number l, Number r) -> Number
            {
                return min(l, r);
            };
            *this = operatorFunc(*this, rhs, comp);
            return *this;
        }

        static NumberCore max(const NumberCore &lhs, const NumberCore &rhs)
        {
            const int lCnt = lhs.size();
            const int rCnt = rhs.size();
            const int maxCnt = std::max(lCnt, rCnt);

            if (lCnt == 0)
            {
                return rhs;
            }
            else if (rCnt == 0)
            {
                return lhs;
            }
            else if (lCnt == 1 && rCnt > 1)
            {
                NumberCore buffer(maxCnt);
                for (int i = 0; i < maxCnt; ++i)
                    buffer[i] = max(lhs[0], rhs[i]); //
                return buffer;
            }
            else if (lCnt > 1 && rCnt == 1)
            {
                NumberCore buffer(maxCnt);
                for (int i = 0; i < maxCnt; ++i)
                    buffer[i] = max(lhs[i], rhs[0]); //
                return buffer;
            }
            else
            {
                NumberCore buffer(maxCnt);
                for (int i = 0; i < maxCnt; ++i)
                {
                    if (i < lCnt && i < rCnt)
                        buffer[i] = max(lhs[i], rhs[i]); //
                }
                return buffer;
            }
        }
        static NumberCore min(const NumberCore &lhs, const NumberCore &rhs)
        {
            const int lCnt = lhs.size();
            const int rCnt = rhs.size();
            const int maxCnt = std::max(lCnt, rCnt);

            if (lCnt == 0)
            {
                return rhs;
            }
            else if (rCnt == 0)
            {
                return lhs;
            }
            else if (lCnt == 1 && rCnt > 1)
            {
                NumberCore buffer(maxCnt);
                for (int i = 0; i < maxCnt; ++i)
                    buffer[i] = min(lhs[0], rhs[i]); //
                return buffer;
            }
            else if (lCnt > 1 && rCnt == 1)
            {
                NumberCore buffer(maxCnt);
                for (int i = 0; i < maxCnt; ++i)
                    buffer[i] = min(lhs[i], rhs[0]); //
                return buffer;
            }
            else
            {
                NumberCore buffer(maxCnt);
                for (int i = 0; i < maxCnt; ++i)
                {
                    if (i < lCnt && i < rCnt)
                        buffer[i] = min(lhs[i], rhs[i]); //
                }
                return buffer;
            }
        }

        using OperatorCompare = std::function<Number(Number l, Number r)>;
        static NumberCore operatorFunc(const NumberCore &lhs, const NumberCore &rhs, OperatorCompare comp)
        {
            const int lCnt = lhs.size();
            const int rCnt = rhs.size();
            const int maxCnt = std::max(lCnt, rCnt);
            Number l = NumberNull;
            Number r = NumberNull;

            NumberCore buffer(maxCnt);
            if (lCnt == 0)
            {
                return rhs;
            }
            else if (rCnt == 0)
            {
                return lhs;
            }
            else if (lCnt == 1 && rCnt > 1)
            {
                l = lhs[0];
                for (int i = 0; i < maxCnt; ++i)
                {
                    r = rhs[i];
                    if (l != NumberNull && r != NumberNull)
                        buffer[i] = comp(l, r); //
                }
            }
            else if (lCnt > 1 && rCnt == 1)
            {
                Number r = rhs[0];
                for (int i = 0; i < maxCnt; ++i)
                {
                    l = lhs[i];
                    if (l != NumberNull && r != NumberNull)
                        buffer[i] = comp(l, r); //
                }
            }
            else
            {
                for (int i = 0; i < maxCnt; ++i)
                {
                    l = lhs.safeAt(i);
                    r = rhs.safeAt(i);
                    if (i < lCnt && i < rCnt && l != NumberNull && r != NumberNull)
                        buffer[i] = comp(l, r); //
                }
            }
            return buffer;
        }

    public:
        NumberCore &operator=(const NumberCore &) = default;
        NumberCore &operator=(NumberCore &&) = default;
        NumberCore &operator=(const Number val)
        {
            this->data = {val};
            return *this;
        }
        NumberCore &operator=(std::initializer_list<Number> list)
        {
            this->data = list;
            return *this;
        }
        NumberCore &operator=(std::vector<Number> &&v)
        {
            this->data = std::move(v);
            return *this;
        }

        Number &operator[](int i)
        {
            return this->data[i];
        }
        const Number &operator[](int i) const
        {
            return this->data[i];
        }

        NumberCore &operator+=(const NumberCore &rhs)
        {
            *this = *this + rhs;
            return *this;
        }
        NumberCore &operator-=(const NumberCore &rhs)
        {
            *this = *this - rhs;
            return *this;
        }
        NumberCore &operator*=(const NumberCore &rhs)
        {
            *this = *this * rhs;
            return *this;
        }
        NumberCore &operator/=(const NumberCore &rhs)
        {
            *this = *this / rhs;
            return *this;
        }
        NumberCore &operator%=(const NumberCore &rhs)
        {
            *this = *this % rhs;
            return *this;
        }

        NumberCore operator!() const
        {
            const int cnt = this->data.size();
            NumberCore ret(cnt);
            for (int i = 0; i < cnt; ++i)
                ret[i] = !this->data[i];
            return ret;
        }

        friend NumberCore operator+(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                return (l + r); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }
        friend NumberCore operator-(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                return (l - r); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }
        friend NumberCore operator*(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                return (l * r); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }
        friend NumberCore operator/(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                if (l == 0.0)
                    return 0.0;
                if (r == 0.0)
                    return NumberNull;
                return (l / r); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }
        friend NumberCore operator%(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                if (l == 0.0)
                    return 0.0;
                if (int64_t(r) == 0)
                    return NumberNull;
                return (int64_t(l) % int64_t(r)); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }
        friend NumberCore operator&&(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                return (l && r); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }
        friend NumberCore operator||(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                return (l || r); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }
        friend NumberCore operator<(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                return (l < r); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }
        friend NumberCore operator<=(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                return (l <= r); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }
        friend NumberCore operator>(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                return (l > r); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }
        friend NumberCore operator>=(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                return (l >= r); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }
        friend NumberCore operator==(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                return (l == r); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }
        friend NumberCore operator!=(const NumberCore &lhs, const NumberCore &rhs)
        {
            auto comp = [](Number l, Number r) -> Number
            {
                return (l != r); //
            };
            return NumberCore::operatorFunc(lhs, rhs, comp);
        }

        void setOther(int i, const std::string &other)
        {
            int sz = this->data.size();
            if (i < 0 || i >= sz)
                return;

            if (this->other.size() != sz)
                this->other.resize(sz);
            this->other[i] = other;
        }
        std::string getOther(int i) const
        {
            int sz = std::min(this->other.size(), this->data.size());
            if (i < 0 || i >= sz)
                return std::string();
            return this->other[i];
        }
    };
}
