#include "NumberCore.h"
#include <cmath>

using namespace StockCharts;

const Number NumberCore::EmptyValue = -DBL_MAX;
const std::string NumberCore::EmptyValueStr = std::to_string(NumberCore::EmptyValue);

NumberCore::NumberCore()
{

}

NumberCore::NumberCore(const Number val)
    : data({ val })
{

}

NumberCore::NumberCore(int cnt, const Number val)
    : data(cnt, val)
{

}

NumberCore::NumberCore(const NumberCore& rhs)
    : data(rhs.data)
    , other(rhs.other)
{

}

NumberCore::NumberCore(NumberCore&& rhs)
    : data(std::move(rhs.data))
    , other(std::move(rhs.other))
{

}

NumberCore::NumberCore(std::initializer_list<Number> list)
    : data(list)
{

}

NumberCore::NumberCore(const std::vector<Number>& v, bool reverse)
    : data(v)
{
    if (reverse)
        this->reverse();
}

NumberCore::NumberCore(std::vector<Number>&& v)
    : data(std::move(v))
{

}

bool NumberCore::isEmpty() const
{
    return this->data.empty();
}

int NumberCore::size() const
{
    return this->data.size();
}

void NumberCore::resize(int cnt, const Number val)
{
    this->data.resize(cnt, val);
}

void NumberCore::clear()
{
    this->data.clear();
    this->other.clear();
}

Number& NumberCore::at(int i)
{
    return this->data[i];
}

Number NumberCore::at(int i) const
{
    return this->data[i];
}

Number& NumberCore::safeAt(int i)
{
    if (i < 0 || i >= this->data.size()) {
        static Number d;
        d = NumberCore::EmptyValue;
        return d;
    }

    return this->data[i];
}

Number NumberCore::safeAt(int i) const
{
    if (i < 0 || i >= this->data.size())
        return NumberCore::EmptyValue;

    return this->data[i];
}

Number& NumberCore::front()
{
    return this->data.front();
}

Number NumberCore::front() const
{
    return this->data.front();
}

Number& NumberCore::back()
{
    return this->data.back();
}

Number NumberCore::back() const
{
    return this->data.back();
}

void NumberCore::fillBegin(int count, const Number newVal)
{
    fill(0, count, newVal);
}

void NumberCore::fillEnd(int count, const Number newVal)
{
    fill(size() - count, count, newVal);
}

void NumberCore::fill(int begin, int count, const Number newVal)
{
    const int sz = this->data.size();
    for (int i = 0; i < count; ++i) {
        int pos = begin + i;
        if (pos < 0 || pos >= sz)
            continue;
        this->data[pos] = newVal;
    }
}

void NumberCore::replace(const Number oldVal, const Number newVal)
{
    if (oldVal == newVal)
        return;

    for (auto ite = this->data.begin(); ite != this->data.end(); ++ite) {
        if (*ite == oldVal)
            *ite = newVal;
    }
}

void NumberCore::replaceNotNumber(const Number newVal)
{
    for (auto ite = this->data.begin(); ite != this->data.end(); ++ite) {
        if (std::isnan(*ite) || std::isinf(*ite))
            *ite = newVal;
    }
}

void NumberCore::replaceEmptyValue(const Number newVal)
{
    replace(NumberCore::EmptyValue, newVal);
}

void NumberCore::reverse()
{
    std::reverse(this->data.begin(), this->data.end());
}

Number NumberCore::max(const Number lhs, const Number rhs)
{
    if (lhs == NumberCore::EmptyValue)
        return rhs;
    else if (rhs == NumberCore::EmptyValue)
        return lhs;

    if (lhs > rhs) //
        return lhs;
    return rhs;
}

Number NumberCore::min(const Number lhs, const Number rhs)
{
    if (lhs == NumberCore::EmptyValue)
        return rhs;
    else if (rhs == NumberCore::EmptyValue)
        return lhs;

    if (lhs < rhs) //
        return lhs;
    return rhs;
}

Number NumberCore::abs(const Number val)
{
    if (val == NumberCore::EmptyValue)
        return val;

    if (val < 0)
        return -val;
    return val;
}

NumberCore& NumberCore::max(const NumberCore& rhs)
{
    auto comp = [this](Number l, Number r) -> Number
    {
        return max(l, r);
    };
    *this = operatorFunc(*this, rhs, comp);
    return *this;
}

NumberCore& NumberCore::min(const NumberCore& rhs)
{
    auto comp = [this](Number l, Number r) -> Number
    {
        return min(l, r);
    };
    *this = operatorFunc(*this, rhs, comp);
    return *this;
}

NumberCore NumberCore::max(const NumberCore& lhs, const NumberCore& rhs)
{
    const int lCnt = lhs.size();
    const int rCnt = rhs.size();
    const int maxCnt = max(lCnt, rCnt);

    if (lCnt == 0) {
        return rhs;
    }
    else if (rCnt == 0) {
        return lhs;
    }
    else if (lCnt == 1 && rCnt > 1) {
        NumberCore buffer(maxCnt);
        for (int i = 0; i < maxCnt; ++i)
            buffer[i] = max(lhs[0], rhs[i]); //
        return buffer;
    }
    else if (lCnt > 1 && rCnt == 1) {
        NumberCore buffer(maxCnt);
        for (int i = 0; i < maxCnt; ++i)
            buffer[i] = max(lhs[i], rhs[0]); //
        return buffer;
    }
    else {
        NumberCore buffer(maxCnt);
        for (int i = 0; i < maxCnt; ++i) {
            if (i < lCnt && i < rCnt)
                buffer[i] = max(lhs[i], rhs[i]); //
        }
        return buffer;
    }
}

NumberCore NumberCore::min(const NumberCore& lhs, const NumberCore& rhs)
{
    const int lCnt = lhs.size();
    const int rCnt = rhs.size();
    const int maxCnt = max(lCnt, rCnt);

    if (lCnt == 0) {
        return rhs;
    }
    else if (rCnt == 0) {
        return lhs;
    }
    else if (lCnt == 1 && rCnt > 1) {
        NumberCore buffer(maxCnt);
        for (int i = 0; i < maxCnt; ++i)
            buffer[i] = min(lhs[0], rhs[i]); //
        return buffer;
    }
    else if (lCnt > 1 && rCnt == 1) {
        NumberCore buffer(maxCnt);
        for (int i = 0; i < maxCnt; ++i)
            buffer[i] = min(lhs[i], rhs[0]); //
        return buffer;
    }
    else {
        NumberCore buffer(maxCnt);
        for (int i = 0; i < maxCnt; ++i) {
            if (i < lCnt && i < rCnt)
                buffer[i] = min(lhs[i], rhs[i]); //
        }
        return buffer;
    }
}

NumberCore NumberCore::operatorFunc(const NumberCore& lhs, const NumberCore& rhs, OperatorCompare comp)
{
    const int lCnt = lhs.size();
    const int rCnt = rhs.size();
    const int maxCnt = max(lCnt, rCnt);
    Number l = NumberCore::EmptyValue;
    Number r = NumberCore::EmptyValue;

    NumberCore buffer(maxCnt);
    if (lCnt == 0) {
        return rhs;
    }
    else if (rCnt == 0) {
        return lhs;
    }
    else if (lCnt == 1 && rCnt > 1) {
        l = lhs[0];
        for (int i = 0; i < maxCnt; ++i) {
            r = rhs[i];
            if (l != NumberCore::EmptyValue && r != NumberCore::EmptyValue)
                buffer[i] = comp(l, r); //
        }
    }
    else if (lCnt > 1 && rCnt == 1) {
        Number r = rhs[0];
        for (int i = 0; i < maxCnt; ++i) {
            l = lhs[i];
            if (l != NumberCore::EmptyValue && r != NumberCore::EmptyValue)
                buffer[i] = comp(l, r); //
        }
    }
    else {
        for (int i = 0; i < maxCnt; ++i) {
            l = lhs.safeAt(i);
            r = rhs.safeAt(i);
            if (i < lCnt && i < rCnt && l != NumberCore::EmptyValue && r != NumberCore::EmptyValue)
                buffer[i] = comp(l, r); //
        }
    }
    return buffer;
}

NumberCore& NumberCore::operator=(const NumberCore& rhs)
{
    this->data = rhs.data;
    this->other = rhs.other;
    return *this;
}

NumberCore& NumberCore::operator=(NumberCore&& rhs) noexcept
{
    this->data = std::move(rhs.data);
    this->other = std::move(rhs.other);
    return *this;
}

NumberCore& NumberCore::operator=(const Number val)
{
    this->data = { val };
    return *this;
}

NumberCore& NumberCore::operator=(std::initializer_list<Number> list)
{
    this->data = list;
    return *this;
}

NumberCore& NumberCore::operator=(std::vector<Number>&& v)
{
    this->data = std::move(v);
    return *this;
}

NumberCore NumberCore::operator!() const
{
    const int cnt = this->data.size();

    NumberCore ret(cnt);
    for (int i = 0; i < cnt; ++i)
        ret[i] = !this->data[i];
    return ret;
}

Number& NumberCore::operator[](int i)
{
    return this->data[i];
}

const Number& NumberCore::operator[](int i) const
{
    return this->data[i];
}

NumberCore& NumberCore::operator+=(const NumberCore& rhs)
{
    *this = *this + rhs;
    return *this;
}

NumberCore& NumberCore::operator-=(const NumberCore& rhs)
{
    *this = *this - rhs;
    return *this;
}

NumberCore& NumberCore::operator*=(const NumberCore& rhs)
{
    *this = *this * rhs;
    return *this;
}

NumberCore& NumberCore::operator/=(const NumberCore& rhs)
{
    *this = *this / rhs;
    return *this;
}

NumberCore& NumberCore::operator%=(const NumberCore& rhs)
{
    *this = *this % rhs;
    return *this;
}

NumberCore StockCharts::operator+(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        return (l + r); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}

NumberCore StockCharts::operator-(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        return (l - r); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}

NumberCore StockCharts::operator*(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        return (l * r); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}

NumberCore StockCharts::operator/(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        if (l == 0.0)
            return 0.0;
        if (r == 0.0)
            return NumberCore::EmptyValue;
        return (l / r); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}

NumberCore StockCharts::operator%(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        if (l == 0.0)
            return 0.0;
        if (int64_t(r) == 0)
            return NumberCore::EmptyValue;
        return (int64_t(l) % int64_t(r)); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}

NumberCore StockCharts::operator&&(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        return (l && r); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}

NumberCore StockCharts::operator||(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        return (l || r); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}

NumberCore StockCharts::operator<(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        return (l < r); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}

NumberCore StockCharts::operator<=(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        return (l <= r); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}
NumberCore StockCharts::operator>(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        return (l > r); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}

NumberCore StockCharts::operator>=(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        return (l >= r); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}

NumberCore StockCharts::operator==(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        return (l == r); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}

NumberCore StockCharts::operator!=(const NumberCore& lhs, const NumberCore& rhs)
{
    auto comp = [](Number l, Number r) -> Number
    {
        return (l != r); //
    };
    return NumberCore::operatorFunc(lhs, rhs, comp);
}

void NumberCore::setOther(int i, const std::string& other)
{
    int sz = this->data.size();
    if (i < 0 || i >= sz)
        return;

    if (this->other.size() != sz)
        this->other.resize(sz);
    this->other[i] = other;
}

std::string NumberCore::getOther(int i) const
{
    int sz = min(this->other.size(), this->data.size());
    if (i < 0 || i >= sz)
        return std::string();
    return this->other[i];
}
