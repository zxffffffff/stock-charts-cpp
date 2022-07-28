#pragma once
#include <string>
#include <vector>
#include <functional>
#include <cmath>
#include <cfloat>

namespace StockCharts
{
    using Number = double;
    using String = std::string;

    class NumberCore
    {
    public:
        static const Number	EmptyNumber;
        static const String	EmptyNumberStr;

    public:
        NumberCore();
        NumberCore(const Number val);
        NumberCore(int cnt, const Number val = EmptyNumber);
        NumberCore(const NumberCore& rhs);
        NumberCore(NumberCore&& rhs);
        NumberCore(std::initializer_list<Number> list);
        NumberCore(const std::vector<Number>& v, bool reverse = false);
        NumberCore(std::vector<Number>&& v);

        bool	isEmpty() const;
        int		size() const;
        void	resize(int cnt, const Number val = EmptyNumber);
        void	clear();

        Number& at(int i);
        Number	at(int i) const;
        Number& safeAt(int i);
        Number	safeAt(int i) const;

        Number& front();
        Number	front() const;

        Number& back();
        Number	back() const;

        void	fillBegin(int count, const Number newVal);
        void	fillEnd(int count, const Number newVal);
        void	fill(int begin, int count, const Number newVal);

        void	replace(const Number oldVal, const Number newVal);
        void	replaceNotNumber(const Number newVal);
        void	replaceEmptyValue(const Number newVal);

        void	reverse();

        static Number max(const Number lhs, const Number rhs);
        static Number min(const Number lhs, const Number rhs);
        static Number abs(const Number val);

        NumberCore& max(const NumberCore& rhs);
        NumberCore& min(const NumberCore& rhs);

        static NumberCore max(const NumberCore& lhs, const NumberCore& rhs);
        static NumberCore min(const NumberCore& lhs, const NumberCore& rhs);

        using OperatorCompare = std::function<Number(Number l, Number r)>;
        static NumberCore operatorFunc(const NumberCore& lhs, const NumberCore& rhs, OperatorCompare comp);

    public:
        NumberCore& operator=(const NumberCore& rhs);
        NumberCore& operator=(NumberCore&& rhs) noexcept;
        NumberCore& operator=(const Number val);
        NumberCore& operator=(std::initializer_list<Number> list);
        NumberCore& operator=(std::vector<Number>&& v);

        NumberCore	operator!() const;

        Number& operator[](int i);
        const Number& operator[](int i) const;

        NumberCore& operator+=(const NumberCore& rhs);
        NumberCore& operator-=(const NumberCore& rhs);
        NumberCore& operator*=(const NumberCore& rhs);
        NumberCore& operator/=(const NumberCore& rhs);
        NumberCore& operator%=(const NumberCore& rhs);

        friend NumberCore operator+ (const NumberCore& lhs, const NumberCore& rhs);
        friend NumberCore operator- (const NumberCore& lhs, const NumberCore& rhs);
        friend NumberCore operator* (const NumberCore& lhs, const NumberCore& rhs);
        friend NumberCore operator/ (const NumberCore& lhs, const NumberCore& rhs);
        friend NumberCore operator% (const NumberCore& lhs, const NumberCore& rhs);
        friend NumberCore operator&&(const NumberCore& lhs, const NumberCore& rhs);
        friend NumberCore operator||(const NumberCore& lhs, const NumberCore& rhs);
        friend NumberCore operator< (const NumberCore& lhs, const NumberCore& rhs);
        friend NumberCore operator<=(const NumberCore& lhs, const NumberCore& rhs);
        friend NumberCore operator> (const NumberCore& lhs, const NumberCore& rhs);
        friend NumberCore operator>=(const NumberCore& lhs, const NumberCore& rhs);
        friend NumberCore operator==(const NumberCore& lhs, const NumberCore& rhs);
        friend NumberCore operator!=(const NumberCore& lhs, const NumberCore& rhs);

    private:
        std::vector<Number> data;

    public:
        void    setOther(int i, const String& other);
        String  getOther(int i) const;

    private:
        std::vector<String> other;
    };
}
