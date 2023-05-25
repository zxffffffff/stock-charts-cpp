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

#include "../lib-chart/Core/Utils.h"
using namespace StockCharts;

TEST(UtilsTest, toWStr)
{
    std::string input = "Hello, world!";
    std::wstring expected = L"Hello, world!";
    std::wstring actual = Utils::toWStr(input);
    EXPECT_EQ(expected, actual);
}

TEST(UtilsTest, toStr)
{
    std::wstring input = L"Hello, world!";
    std::string expected = "Hello, world!";
    std::string actual = Utils::toStr(input);
    EXPECT_EQ(expected, actual);
}

TEST(UtilsTest, to8bitStr)
{
    std::string input = "我是中国人";
    std::string expected = "%u6211%u662F%u4E2D%u56FD%u4EBA";
    std::string actual = Utils::to8bitStr(input);
    EXPECT_EQ(expected, actual);
}

TEST(UtilsTest, splitStr)
{
    std::string input = "Hello,world";
    char separator = ',';
    std::vector<std::string> expected = {"Hello", "world"};
    std::vector<std::string> actual = Utils::splitStr(input, separator);
    EXPECT_EQ(expected, actual);
}

TEST(UtilsTest, checkEmpty)
{
    std::string input1 = "";
    bool expected1 = true;
    bool actual1 = Utils::checkEmpty(input1);
    EXPECT_EQ(expected1, actual1);

    std::string input2 = "  \r\n\t\v; ";
    bool expected2 = true;
    bool actual2 = Utils::checkEmpty(input2);
    EXPECT_EQ(expected2, actual2);

    std::string input3 = "Hello, world!";
    bool expected3 = false;
    bool actual3 = Utils::checkEmpty(input3);
    EXPECT_EQ(expected3, actual3);
}

TEST(NumberUtilsTest, ToStringTest)
{
    double price1 = 123.449999;
    Number price2 = 123.450001;
    int precision = 2;
    std::string sNull = "--";
    std::string expected_str = "123.45";

    std::string str1 = NumberUtils::toString(price1, precision, sNull);
    std::string str2 = NumberUtils::toString(price2, precision, sNull);

    EXPECT_EQ(expected_str, str1);
    EXPECT_EQ(expected_str, str2);
}

TEST(NumberUtilsTest, ToTimestampTest)
{
    Number n = 1683603574;
    const char *format = "%Y-%m-%d %H:%M:%S";
    std::string expected_str = "2023-05-09 11:39:34"; // GMT+0800 (中国标准时间)

    std::string str = NumberUtils::toTimestamp(n, format);

    EXPECT_EQ(expected_str, str);
}

TEST(NumberUtilsTest, IsDoubleValidTest)
{
    double value1 = 123.45;
    double value2 = NAN;
    double value3 = INFINITY;
    double value4 = -INFINITY;

    bool valid1 = NumberUtils::isDoubleValid(value1);
    bool valid2 = NumberUtils::isDoubleValid(value1, value2);
    bool valid3 = NumberUtils::isDoubleValid(value1, value2, value3);
    bool valid4 = NumberUtils::isDoubleValid(value1, value2, value3, value4);

    EXPECT_TRUE(valid1);
    EXPECT_FALSE(valid2);
    EXPECT_FALSE(valid3);
    EXPECT_FALSE(valid4);
}
