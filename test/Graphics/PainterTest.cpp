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

#include "../lib-chart/Graphics/Painter.h"
using namespace StockCharts;

TEST(ColorTest, Constructor)
{
    Color c1(255, 0, 0, 255);
    ASSERT_EQ(c1.r, 255);
    ASSERT_EQ(c1.g, 0);
    ASSERT_EQ(c1.b, 0);
    ASSERT_EQ(c1.a, 255);

    Color c2("#00FF00");
    ASSERT_EQ(c2.r, 0);
    ASSERT_EQ(c2.g, 255);
    ASSERT_EQ(c2.b, 0);
    ASSERT_EQ(c2.a, 255);

    Color c3("#FF00FFFF");
    ASSERT_EQ(c3.r, 255);
    ASSERT_EQ(c3.g, 0);
    ASSERT_EQ(c3.b, 255);
    ASSERT_EQ(c3.a, 255);

    Color c4("ABCDEF");
    ASSERT_EQ(c4.r, 171);
    ASSERT_EQ(c4.g, 205);
    ASSERT_EQ(c4.b, 239);
    ASSERT_EQ(c4.a, 255);

    Color c5("");
    ASSERT_EQ(c5.r, 0);
    ASSERT_EQ(c5.g, 255);
    ASSERT_EQ(c5.b, 255);
    ASSERT_EQ(c5.a, 255);
}

TEST(ColorTest, Equals)
{
    Color c1(255, 0, 0, 255);
    Color c2(255, 0, 0, 255);
    Color c3(0, 255, 0, 255);

    ASSERT_EQ(c1, c2);
    ASSERT_NE(c1, c3);
}
