/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include <gtest/gtest.h>
#include <gmock/gmock.h>
using namespace testing;

#include "../lib-chart/Core/NumberCore.h"
using namespace StockCharts;

TEST(NumberCoreConstructorTest, DefaultConstructor)
{
    NumberCore nc;
    EXPECT_TRUE(nc.empty());
    EXPECT_EQ(nc.size(), 0);
}

TEST(NumberCoreConstructorTest, ValueConstructor)
{
    const Number val = 3.14159;
    NumberCore nc(val);
    EXPECT_FALSE(nc.empty());
    EXPECT_EQ(nc.size(), 1);
    EXPECT_EQ(nc[0], val);
}

TEST(NumberCoreConstructorTest, CountAndValueConstructor)
{
    const int cnt = 5;
    const Number val = 42.0;
    NumberCore nc(cnt, val);
    EXPECT_FALSE(nc.empty());
    EXPECT_EQ(nc.size(), cnt);
    for (int i = 0; i < cnt; ++i)
    {
        EXPECT_EQ(nc[i], val);
    }
}

TEST(NumberCoreConstructorTest, InitializerListConstructor)
{
    NumberCore nc{1.0, 2.0, 3.0};
    EXPECT_FALSE(nc.empty());
    EXPECT_EQ(nc.size(), 3);
    EXPECT_EQ(nc[0], 1.0);
    EXPECT_EQ(nc[1], 2.0);
    EXPECT_EQ(nc[2], 3.0);
}

TEST(NumberCoreConstructorTest, VectorConstructor)
{
    std::vector<Number> v{1.0, 2.0, 3.0};
    NumberCore nc(v);
    EXPECT_FALSE(nc.empty());
    EXPECT_EQ(nc.size(), v.size());
    for (int i = 0; i < v.size(); ++i)
    {
        EXPECT_EQ(nc[i], v[i]);
    }
}

TEST(NumberCoreConstructorTest, MoveVectorConstructor)
{
    std::vector<Number> v{1.0, 2.0, 3.0};
    NumberCore nc(std::move(v));
    EXPECT_FALSE(nc.empty());
    EXPECT_EQ(nc.size(), 3);
    EXPECT_EQ(nc[0], 1.0);
    EXPECT_EQ(nc[1], 2.0);
    EXPECT_EQ(nc[2], 3.0);
}

TEST(NumberCoreConstructorTest, ReverseVectorConstructor)
{
    std::vector<Number> v{1.0, 2.0, 3.0};
    NumberCore nc(v, true);
    EXPECT_FALSE(nc.empty());
    EXPECT_EQ(nc.size(), v.size());
    EXPECT_EQ(nc[0], 3.0);
    EXPECT_EQ(nc[1], 2.0);
    EXPECT_EQ(nc[2], 1.0);
}

TEST(NumberCoreConstructorTest, MoveVectorAndOtherConstructor)
{
    std::vector<Number> v{1.0, 2.0, 3.0};
    std::vector<std::string> other{"a", "b", "c"};
    NumberCore nc(std::move(v), std::move(other));
    EXPECT_FALSE(nc.empty());
    EXPECT_EQ(nc.size(), 3);
    EXPECT_EQ(nc[0], 1.0);
    EXPECT_EQ(nc[1], 2.0);
    EXPECT_EQ(nc[2], 3.0);
    EXPECT_EQ(nc.getOther(0), "a");
    EXPECT_EQ(nc.getOther(1), "b");
    EXPECT_EQ(nc.getOther(2), "c");
}

TEST(NumberCoreTest, Empty)
{
    NumberCore nc;
    EXPECT_TRUE(nc.empty());

    nc = {1.0, 2.0, 3.0};
    EXPECT_FALSE(nc.empty());
}

TEST(NumberCoreTest, Size)
{
    NumberCore nc;
    EXPECT_EQ(nc.size(), 0);

    nc = {1.0, 2.0, 3.0};
    EXPECT_EQ(nc.size(), 3);
}

TEST(NumberCoreTest, Resize)
{
    NumberCore nc;
    nc.resize(3);
    EXPECT_EQ(nc.size(), 3);
    EXPECT_EQ(nc[0], NumberNull);
    EXPECT_EQ(nc[1], NumberNull);
    EXPECT_EQ(nc[2], NumberNull);

    const Number val = 42.0;
    nc.resize(5, val);
    EXPECT_EQ(nc.size(), 5);
    EXPECT_EQ(nc[0], NumberNull);
    EXPECT_EQ(nc[1], NumberNull);
    EXPECT_EQ(nc[2], NumberNull);
    EXPECT_EQ(nc[3], val);
    EXPECT_EQ(nc[4], val);

    nc.resize(2);
    EXPECT_EQ(nc.size(), 2);
    EXPECT_EQ(nc[0], NumberNull);
    EXPECT_EQ(nc[1], NumberNull);
}

TEST(NumberCoreTest, Clear)
{
    NumberCore nc{1.0, 2.0, 3.0};
    EXPECT_FALSE(nc.empty());
    EXPECT_EQ(nc.size(), 3);

    nc.clear();
    EXPECT_TRUE(nc.empty());
    EXPECT_EQ(nc.size(), 0);
}

TEST(NumberCoreTest, At)
{
    NumberCore nc{1.0, 2.0, 3.0};
    EXPECT_EQ(nc.at(0), 1.0);
    EXPECT_EQ(nc.at(1), 2.0);
    EXPECT_EQ(nc.at(2), 3.0);

    // Test non-const version
    nc.at(1) = 42.0;
    EXPECT_EQ(nc.at(1), 42.0);

    // Test const version
    const NumberCore &cnc = nc;
    EXPECT_EQ(cnc.at(0), 1.0);
    EXPECT_EQ(cnc.at(1), 42.0);
    EXPECT_EQ(cnc.at(2), 3.0);
}

TEST(NumberCoreTest, SafeAt)
{
    NumberCore nc{1.0, 2.0, 3.0};
    EXPECT_EQ(nc.safeAt(0), 1.0);
    EXPECT_EQ(nc.safeAt(1), 2.0);
    EXPECT_EQ(nc.safeAt(2), 3.0);
    EXPECT_EQ(nc.safeAt(-1), NumberNull);
    EXPECT_EQ(nc.safeAt(3), NumberNull);

    // Test non-const version
    nc.safeAt(1) = 42.0;
    EXPECT_EQ(nc.safeAt(1), 42.0);

    // Test const version
    const NumberCore &cnc = nc;
    EXPECT_EQ(cnc.safeAt(0), 1.0);
    EXPECT_EQ(cnc.safeAt(1), 42.0);
    EXPECT_EQ(cnc.safeAt(2), 3.0);
    EXPECT_EQ(cnc.safeAt(-1), NumberNull);
    EXPECT_EQ(cnc.safeAt(3), NumberNull);
}

TEST(NumberCoreTest, Front)
{
    NumberCore nc{1.0, 2.0, 3.0};
    EXPECT_EQ(nc.front(), 1.0);

    // Test non-const version
    nc.front() = 42.0;
    EXPECT_EQ(nc.front(), 42.0);

    // Test const version
    const NumberCore &cnc = nc;
    EXPECT_EQ(cnc.front(), 42.0);
}

TEST(NumberCoreTest, Back)
{
    NumberCore nc{1.0, 2.0, 3.0};
    EXPECT_EQ(nc.back(), 3.0);

    // Test non-const version
    nc.back() = 42.0;
    EXPECT_EQ(nc.back(), 42.0);

    // Test const version
    const NumberCore &cnc = nc;
    EXPECT_EQ(cnc.back(), 42.0);
}

TEST(NumberCoreTest, FillBeginTest)
{
    NumberCore nc({1, 2, 3, 4, 5});
    nc.fillBegin(2, 6);
    EXPECT_EQ(nc[0], 6);
    EXPECT_EQ(nc[1], 6);
    EXPECT_EQ(nc[2], 3);
    EXPECT_EQ(nc[3], 4);
    EXPECT_EQ(nc[4], 5);
}

TEST(NumberCoreTest, FillEndTest)
{
    NumberCore nc({1, 2, 3, 4, 5});
    nc.fillEnd(2, 6);
    EXPECT_EQ(nc[0], 1);
    EXPECT_EQ(nc[1], 2);
    EXPECT_EQ(nc[2], 3);
    EXPECT_EQ(nc[3], 6);
    EXPECT_EQ(nc[4], 6);
}

TEST(NumberCoreTest, FillTest)
{
    NumberCore nc({1, 2, 3, 4, 5});
    nc.fill(1, 3, 6);
    EXPECT_EQ(nc[0], 1);
    EXPECT_EQ(nc[1], 6);
    EXPECT_EQ(nc[2], 6);
    EXPECT_EQ(nc[3], 6);
    EXPECT_EQ(nc[4], 5);
}

TEST(NumberCoreTest, FillOutOfBoundsTest)
{
    NumberCore nc({1, 2, 3, 4, 5});
    nc.fill(-1, 2, 6);
    nc.fill(4, 2, 6);
    EXPECT_EQ(nc[0], 6);
    EXPECT_EQ(nc[1], 2);
    EXPECT_EQ(nc[2], 3);
    EXPECT_EQ(nc[3], 4);
    EXPECT_EQ(nc[4], 6);
}

TEST(NumberCoreTest, ReplaceTest)
{
    NumberCore nc{1, 2, 3, 4, 5};
    nc.replace(3, 8);
    EXPECT_EQ(nc[2], 8);
    nc.replace(8, 8);
    EXPECT_EQ(nc[2], 8);
}

TEST(NumberCoreTest, ReplaceNotNumberTest)
{
    NumberCore nc{1.0, 2.0, std::nan(""), 4.0, std::numeric_limits<Number>::infinity()};
    nc.replaceNotNumber(10);
    EXPECT_EQ(nc[2], 10);
    EXPECT_EQ(nc[4], 10);
    EXPECT_EQ(nc[0], 1);
}

TEST(NumberCoreTest, ReplaceEmptyValueTest)
{
    NumberCore nc{1, 2, NumberNull, 4, NumberNull};
    nc.replaceEmptyValue(5);
    EXPECT_EQ(nc[2], 5);
    EXPECT_EQ(nc[4], 5);
    EXPECT_EQ(nc[1], 2);
}

TEST(NumberCoreTest, SwapTest)
{
    NumberCore nc1{1, 2, 3};
    NumberCore nc2{4, 5, 6};
    nc1.swap(nc2);
    EXPECT_EQ(nc1[0], 4);
    EXPECT_EQ(nc1[1], 5);
    EXPECT_EQ(nc1[2], 6);
    EXPECT_EQ(nc2[0], 1);
    EXPECT_EQ(nc2[1], 2);
    EXPECT_EQ(nc2[2], 3);
}

TEST(NumberCoreTest, ReverseTest)
{
    NumberCore nc{1, 2, 3, 4, 5};
    nc.reverse();
    EXPECT_EQ(nc[0], 5);
    EXPECT_EQ(nc[1], 4);
    EXPECT_EQ(nc[2], 3);
    EXPECT_EQ(nc[3], 2);
    EXPECT_EQ(nc[4], 1);
}

TEST(NumberCoreTest, GetMinMaxTest)
{
    // Test an empty NumberCore
    NumberCore nc1;
    EXPECT_EQ(std::make_pair(NumberNull, NumberNull), nc1.getMinMax(0, 0));

    // Test with beginIndex and endIndex out of range
    NumberCore nc2{1.0, 2.0, 3.0};
    EXPECT_EQ(std::make_pair(NumberNull, NumberNull), nc2.getMinMax(-1, 4));

    // Test with beginIndex == endIndex
    EXPECT_EQ(std::make_pair(2.0, 2.0), nc2.getMinMax(1, 1));

    // Test with valid beginIndex and endIndex
    EXPECT_EQ(std::make_pair(2.0, 3.0), nc2.getMinMax(1, 3));

    // Test with reverse order NumberCore
    NumberCore nc3{3.0, 2.0, 1.0};
    EXPECT_EQ(std::make_pair(1.0, 3.0), nc3.getMinMax(0, 3));
}

TEST(NumberCoreTest, MaxTest)
{
    EXPECT_EQ(NumberCore::max(1.1, 2.2), 2.2);
    EXPECT_EQ(NumberCore::max(2.2, 3.3), 3.3);
    EXPECT_EQ(NumberCore::max(1.1, 3.3), 3.3);
    EXPECT_EQ(NumberCore::max(NumberNull, 1.1), 1.1);
    EXPECT_EQ(NumberCore::max(1.1, NumberNull), 1.1);
    EXPECT_EQ(NumberCore::max(NumberNull, NumberNull), NumberNull);
}

TEST(NumberCoreTest, MinTest)
{
    EXPECT_EQ(NumberCore::min(1.1, 2.2), 1.1);
    EXPECT_EQ(NumberCore::min(2.2, 3.3), 2.2);
    EXPECT_EQ(NumberCore::min(1.1, 3.3), 1.1);
    EXPECT_EQ(NumberCore::min(NumberNull, 1.1), 1.1);
    EXPECT_EQ(NumberCore::min(1.1, NumberNull), 1.1);
    EXPECT_EQ(NumberCore::min(NumberNull, NumberNull), NumberNull);
}

TEST(NumberCoreTest, AbsTest)
{
    EXPECT_DOUBLE_EQ(NumberCore::abs(1.1), 1.1);
    EXPECT_DOUBLE_EQ(NumberCore::abs(-2.2), 2.2);
    EXPECT_DOUBLE_EQ(NumberCore::abs(3.3), 3.3);
    EXPECT_DOUBLE_EQ(NumberCore::abs(NumberNull), NumberNull);
}

TEST(NumberCoreTest, MaxTest2)
{
    // Test with two NumberCore objects of equal size
    NumberCore nc1({1, 2, 3, 4, 5});
    NumberCore nc2({6, 7, 8, 9, 10});
    NumberCore result = NumberCore::max(nc1, nc2);
    EXPECT_EQ(result.size(), 5);
    EXPECT_EQ(result[0], 6);
    EXPECT_EQ(result[1], 7);
    EXPECT_EQ(result[2], 8);
    EXPECT_EQ(result[3], 9);
    EXPECT_EQ(result[4], 10);

    // Test with two NumberCore objects of different sizes
    NumberCore nc3({1, 2, 3, 4, 5});
    NumberCore nc4({6, 7});
    NumberCore result2 = NumberCore::max(nc3, nc4);
    EXPECT_EQ(result2.size(), 5);
    EXPECT_EQ(result2[0], 6);
    EXPECT_EQ(result2[1], 7);
    EXPECT_EQ(result2[2], NumberNull);
    EXPECT_EQ(result2[3], NumberNull);
    EXPECT_EQ(result2[4], NumberNull);

    // Test with one empty NumberCore object
    NumberCore nc5({1, 2, 3, 4, 5});
    NumberCore nc6({});
    NumberCore result3 = NumberCore::max(nc5, nc6);
    EXPECT_EQ(result3.size(), 5);
    EXPECT_EQ(result3[0], 1);
    EXPECT_EQ(result3[1], 2);
    EXPECT_EQ(result3[2], 3);
    EXPECT_EQ(result3[3], 4);
    EXPECT_EQ(result3[4], 5);

    // Test with both empty NumberCore objects
    NumberCore nc7({});
    NumberCore nc8({});
    NumberCore result4 = NumberCore::max(nc7, nc8);
    EXPECT_TRUE(result4.empty());

    // Test with one NumberCore object with single value
    NumberCore nc9({3});
    NumberCore nc10({1, 2, 3, 4, 5});
    NumberCore result5 = NumberCore::max(nc9, nc10);
    EXPECT_EQ(result5.size(), 5);
    EXPECT_EQ(result5[0], 3);
    EXPECT_EQ(result5[1], 3);
    EXPECT_EQ(result5[2], 3);
    EXPECT_EQ(result5[3], 4);
    EXPECT_EQ(result5[4], 5);
}

TEST(NumberCoreTest, MinTest2)
{
    // Test with two NumberCore objects of equal size
    NumberCore nc1({1, 2, 3, 4, 5});
    NumberCore nc2({6, 7, 8, 9, 10});
    NumberCore result = NumberCore::min(nc1, nc2);
    EXPECT_EQ(result.size(), 5);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 2);
    EXPECT_EQ(result[2], 3);
    EXPECT_EQ(result[3], 4);
    EXPECT_EQ(result[4], 5);

    // Test with two NumberCore objects of different sizes
    NumberCore nc3({6, 7, 3, 4, 5});
    NumberCore nc4({1, 2});
    NumberCore result2 = NumberCore::min(nc3, nc4);
    EXPECT_EQ(result2.size(), 5);
    EXPECT_EQ(result2[0], 1);
    EXPECT_EQ(result2[1], 2);
    EXPECT_EQ(result2[2], NumberNull);
    EXPECT_EQ(result2[3], NumberNull);
    EXPECT_EQ(result2[4], NumberNull);

    // Test with one empty NumberCore object
    NumberCore nc5({1, 2, 3, 4, 5});
    NumberCore nc6({});
    NumberCore result3 = NumberCore::min(nc5, nc6);
    EXPECT_EQ(result3.size(), 5);
    EXPECT_EQ(result3[0], 1);
    EXPECT_EQ(result3[1], 2);
    EXPECT_EQ(result3[2], 3);
    EXPECT_EQ(result3[3], 4);
    EXPECT_EQ(result3[4], 5);

    // Test with both empty NumberCore objects
    NumberCore nc7({});
    NumberCore nc8({});
    NumberCore result4 = NumberCore::min(nc7, nc8);
    EXPECT_TRUE(result4.empty());

    // Test with one NumberCore object with single value
    NumberCore nc9({3});
    NumberCore nc10({1, 2, 3, 4, 5});
    NumberCore result5 = NumberCore::min(nc9, nc10);
    EXPECT_EQ(result5.size(), 5);
    EXPECT_EQ(result5[0], 1);
    EXPECT_EQ(result5[1], 2);
    EXPECT_EQ(result5[2], 3);
    EXPECT_EQ(result5[3], 3);
    EXPECT_EQ(result5[4], 3);
}

TEST(NumberCoreTest, OperatorAddEqual)
{
    NumberCore v1({1, 2, 3}), v2({4, 5, 6}), v3({5, 7, 9});
    v1 += v2;
    EXPECT_EQ(v1[0], v3[0]);
    EXPECT_EQ(v1[1], v3[1]);
    EXPECT_EQ(v1[2], v3[2]);
}

TEST(NumberCoreTest, OperatorSubEqual)
{
    NumberCore v1({4, 5, 6}), v2({1, 2, 3}), v3({3, 3, 3});
    v1 -= v2;
    EXPECT_EQ(v1[0], v3[0]);
    EXPECT_EQ(v1[1], v3[1]);
    EXPECT_EQ(v1[2], v3[2]);
}

TEST(NumberCoreTest, OperatorMulEqual)
{
    NumberCore v1({1, 2, 3}), v2({2, 3, 4}), v3({2, 6, 12});
    v1 *= v2;
    EXPECT_EQ(v1[0], v3[0]);
    EXPECT_EQ(v1[1], v3[1]);
    EXPECT_EQ(v1[2], v3[2]);
}

TEST(NumberCoreTest, OperatorDivEqual)
{
    NumberCore v1({4, 6, 8}), v2({2, 2, 2}), v3({2, 3, 4});
    v1 /= v2;
    EXPECT_EQ(v1[0], v3[0]);
    EXPECT_EQ(v1[1], v3[1]);
    EXPECT_EQ(v1[2], v3[2]);
}

TEST(NumberCoreTest, OperatorModEqual)
{
    NumberCore v1({5, 7, 9}), v2({2, 3, 4}), v3({1, 1, 1});
    v1 %= v2;
    EXPECT_EQ(v1[0], v3[0]);
    EXPECT_EQ(v1[1], v3[1]);
    EXPECT_EQ(v1[2], v3[2]);
}

TEST(NumberCoreTest, OperatorNot)
{
    NumberCore v1({1, 0, 1}), v2({0, 1, 0});
    NumberCore v3 = !v2;
    EXPECT_EQ(v1[0], v3[0]);
    EXPECT_EQ(v1[1], v3[1]);
    EXPECT_EQ(v1[2], v3[2]);
}

TEST(NumberCoreTest, Addition)
{
    NumberCore a{1.0, 2.0, 3.0};
    NumberCore b{3.0, 2.0, 1.0};
    NumberCore c = a + b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 4.0);
    EXPECT_EQ(c[1], 4.0);
    EXPECT_EQ(c[2], 4.0);
}

TEST(NumberCoreTest, Subtraction)
{
    NumberCore a{1.0, 2.0, 3.0};
    NumberCore b{3.0, 2.0, 1.0};
    NumberCore c = a - b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], -2.0);
    EXPECT_EQ(c[1], 0.0);
    EXPECT_EQ(c[2], 2.0);
}

TEST(NumberCoreTest, Multiplication)
{
    NumberCore a{1.0, 2.0, 3.0};
    NumberCore b{3.0, 2.0, 1.0};
    NumberCore c = a * b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 3.0);
    EXPECT_EQ(c[1], 4.0);
    EXPECT_EQ(c[2], 3.0);
}

TEST(NumberCoreTest, Division)
{
    NumberCore a{1.0, 2.0, 3.0};
    NumberCore b{3.0, 2.0, 1.0};
    NumberCore c = a / b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_DOUBLE_EQ(c[0], 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(c[1], 1.0);
    EXPECT_DOUBLE_EQ(c[2], 3.0);
}

TEST(NumberCoreTest, Modulo)
{
    NumberCore a{7.0, 8.0, 9.0};
    NumberCore b{2.0, 3.0, 4.0};
    NumberCore c = a % b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1.0);
    EXPECT_EQ(c[1], 2.0);
    EXPECT_EQ(c[2], 1.0);
}

TEST(NumberCoreTest, LogicalAnd)
{
    NumberCore a{1, 1, 0};
    NumberCore b{0, 1, 1};
    NumberCore c = a && b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 0);
    EXPECT_EQ(c[1], 1);
    EXPECT_EQ(c[2], 0);
}

TEST(NumberCoreTest, LogicalOr)
{
    NumberCore a{1, 1, 0};
    NumberCore b{0, 1, 1};
    NumberCore c = a || b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 1);
    EXPECT_EQ(c[2], 1);
}

TEST(NumberCoreTest, LessThan)
{
    NumberCore a{1, 2, 3};
    NumberCore b{0, 2, 4};
    NumberCore c = a < b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 0);
    EXPECT_EQ(c[1], 0);
    EXPECT_EQ(c[2], 1);
}

TEST(NumberCoreTest, LessThanOrEqualTo)
{
    NumberCore a{1, 2, 3};
    NumberCore b{0, 2, 4};
    NumberCore c = a <= b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 0);
    EXPECT_EQ(c[1], 1);
    EXPECT_EQ(c[2], 1);
}

TEST(NumberCoreTest, GreaterThan)
{
    NumberCore a{1, 2, 3};
    NumberCore b{0, 2, 4};
    NumberCore c = a > b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 0);
    EXPECT_EQ(c[2], 0);
}

TEST(NumberCoreTest, GreaterThanOrEqualTo)
{
    NumberCore a{1, 2, 3};
    NumberCore b{0, 2, 4};
    NumberCore c = a >= b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 1);
    EXPECT_EQ(c[2], 0);
}

TEST(NumberCoreTest, EqualTo)
{
    NumberCore a{1, 2, 3};
    NumberCore b{0, 2, 4};
    NumberCore c = a == b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 0);
    EXPECT_EQ(c[1], 1);
    EXPECT_EQ(c[2], 0);
}

TEST(NumberCoreTest, NotEqualTo)
{
    NumberCore a{1, 2, 3};
    NumberCore b{0, 2, 4};
    NumberCore c = a != b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 0);
    EXPECT_EQ(c[2], 1);
}

TEST(NumberCoreTest, SetOtherAndGetOther)
{
    NumberCore nc{1.0, 2.0, 3.0};
    EXPECT_EQ(nc.getOther(0), "");
    EXPECT_EQ(nc.getOther(1), "");
    EXPECT_EQ(nc.getOther(2), "");

    const std::string other1 = "a";
    const std::string other2 = "b";
    const std::string other3 = "c";
    nc.setOther(0, other1);
    nc.setOther(1, other2);
    nc.setOther(2, other3);
    EXPECT_EQ(nc.getOther(0), other1);
    EXPECT_EQ(nc.getOther(1), other2);
    EXPECT_EQ(nc.getOther(2), other3);
}
