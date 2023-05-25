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

#include "../lib-chart/Graphics/Graphics.h"
using namespace StockCharts;

TEST(PointTest, Constructor)
{
  Point p;
  EXPECT_EQ(p.x, RealNull);
  EXPECT_EQ(p.y, RealNull);
}

TEST(PointTest, Set)
{
  Point p;
  p.set(1.0f, 2.0f);
  EXPECT_EQ(p.x, 1.0f);
  EXPECT_EQ(p.y, 2.0f);
}

TEST(PointTest, Clear)
{
  Point p;
  p.set(1.0f, 2.0f);
  p.clear();
  EXPECT_EQ(p.x, RealNull);
  EXPECT_EQ(p.y, RealNull);
}

TEST(PointTest, Valid)
{
  Point p1;
  EXPECT_FALSE(p1.valid());
  Point p2(1.0f, RealNull);
  EXPECT_FALSE(p2.valid());
  Point p3(RealNull, 2.0f);
  EXPECT_FALSE(p3.valid());
  Point p4(1.0f, 2.0f);
  EXPECT_TRUE(p4.valid());
}

TEST(PointTest, Equality)
{
  Point p1;
  Point p2;
  EXPECT_EQ(p1, p2);
  Point p3(1.0f, 2.0f);
  Point p4(1.0f, 2.0f);
  EXPECT_EQ(p3, p4);
  Point p5(3.0f, 4.0f);
  EXPECT_NE(p4, p5);
}

TEST(SizeTest, Constructor)
{
  Size s;
  EXPECT_EQ(s.width, 0);
  EXPECT_EQ(s.height, 0);
}

TEST(SizeTest, Set)
{
  Size s;
  s.set(1.0f, 2.0f);
  EXPECT_EQ(s.width, 1.0f);
  EXPECT_EQ(s.height, 2.0f);
}

TEST(SizeTest, Clear)
{
  Size s;
  s.set(1.0f, 2.0f);
  s.clear();
  EXPECT_EQ(s.width, 0);
  EXPECT_EQ(s.height, 0);
}

TEST(SizeTest, Valid)
{
  Size s1;
  EXPECT_TRUE(s1.valid());
  Size s2(-1.0f, 2.0f);
  EXPECT_FALSE(s2.valid());
  Size s3(1.0f, -2.0f);
  EXPECT_FALSE(s3.valid());
  Size s4(1.0f, 2.0f);
  EXPECT_TRUE(s4.valid());
}

TEST(SizeTest, Equality)
{
  Size s1;
  Size s2;
  EXPECT_EQ(s1, s2);
  Size s3(1.0f, 2.0f);
  Size s4(1.0f, 2.0f);
  EXPECT_EQ(s3, s4);
  Size s5(3.0f, 4.0f);
  EXPECT_NE(s4, s5);
}

TEST(RectTest, SetAndGet)
{
  Rect rect;
  rect.set(10, 20, 30, 40);
  EXPECT_EQ(rect.left(), 10);
  EXPECT_EQ(rect.top(), 20);
  EXPECT_EQ(rect.width(), 30);
  EXPECT_EQ(rect.height(), 40);
}

TEST(RectTest, Clear)
{
  Rect rect;
  rect.set(10, 20, 30, 40);
  rect.clear();
  EXPECT_FALSE(rect.valid());
}

TEST(RectTest, MoveInside)
{
  Rect rect(0, 0, 20, 20);
  rect.moveInside(Rect(1, 1, 50, 50), {0, 0, 0, 0});
  EXPECT_EQ(rect.left(), 1);
  EXPECT_EQ(rect.top(), 1);
  EXPECT_EQ(rect.right(), 21);
  EXPECT_EQ(rect.bottom(), 21);
}

TEST(RectTest, ClipInside)
{
  Rect rect(10, 10, 30, 30);
  rect.clipInside(Rect(0, 0, 20, 20));
  EXPECT_EQ(rect.left(), 10);
  EXPECT_EQ(rect.top(), 10);
  EXPECT_EQ(rect.width(), 10);
  EXPECT_EQ(rect.height(), 10);
}

TEST(RectTest, ContainsPoint)
{
  Rect rect(10, 10, 20, 20);
  EXPECT_TRUE(rect.contains(Point(15, 15)));
  EXPECT_FALSE(rect.contains(Point(5, 5)));
}

TEST(RectTest, Operators)
{
  Rect rect1(10, 10, 20, 20);
  Rect rect2(10, 10, 20, 20);
  Rect rect3(5, 5, 10, 10);
  EXPECT_EQ(rect1, rect2);
  EXPECT_NE(rect1, rect3);
}

TEST(LineTest, Constructor)
{
  Line line(1, 2, 3, 4);
  ASSERT_EQ(line.first.x, 1);
  ASSERT_EQ(line.first.y, 2);
  ASSERT_EQ(line.second.x, 3);
  ASSERT_EQ(line.second.y, 4);
}

TEST(LineTest, Set)
{
  Line line;
  line.set(1, 2, 3, 4);
  ASSERT_EQ(line.first.x, 1);
  ASSERT_EQ(line.first.y, 2);
  ASSERT_EQ(line.second.x, 3);
  ASSERT_EQ(line.second.y, 4);
}

TEST(LineTest, Valid)
{
  Line line(1, 2, 3, 4);
  ASSERT_TRUE(line.valid());

  Line invalidLine(1, 2, std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
  ASSERT_FALSE(invalidLine.valid());
}

TEST(LineTest, Clear)
{
  Line line(1, 2, 3, 4);
  line.clear();
  ASSERT_FALSE(line.valid());
}

TEST(LineTest, Equality)
{
  Line line1(1, 2, 3, 4);
  Line line2(1, 2, 3, 4);
  Line line3(3, 4, 1, 2);

  ASSERT_EQ(line1, line2);
  ASSERT_EQ(line1, line3);
}

TEST(LineTest, Inequality)
{
  Line line1(1, 2, 3, 4);
  Line line2(1, 2, 5, 6);

  ASSERT_NE(line1, line2);
}

TEST(StickTest, DefaultConstructor)
{
  Stick s;
  EXPECT_FALSE(s.valid());
  EXPECT_EQ(s.flag, 0);
}

TEST(StickTest, Constructor)
{
  Stick s(10, 20, 30, 40, 5, 60);
  EXPECT_EQ(s.left(), 10);
  EXPECT_EQ(s.right(), 40);
  EXPECT_EQ(s.top(), 20);
  EXPECT_EQ(s.bottom(), 60);
  EXPECT_EQ(s.width(), 30);
  EXPECT_EQ(s.height(), 40);
  EXPECT_EQ(s.high(), 5);
  EXPECT_EQ(s.low(), 60);
  EXPECT_EQ(s.centerX(), 25);
  EXPECT_EQ(s.centerY(), 32.5);
  EXPECT_EQ(s.center(), Point(25, 32.5));
}

TEST(StickTest, Set)
{
  Stick s;
  EXPECT_FALSE(s.valid());
  s.set(20, 30, 40, 50, 10, 70, 1);
  EXPECT_EQ(s.left(), 20);
  EXPECT_EQ(s.right(), 60);
  EXPECT_EQ(s.top(), 30);
  EXPECT_EQ(s.bottom(), 80);
  EXPECT_EQ(s.width(), 40);
  EXPECT_EQ(s.height(), 50);
  EXPECT_EQ(s.high(), 10);
  EXPECT_EQ(s.low(), 70);
  EXPECT_EQ(s.centerX(), 40);
  EXPECT_EQ(s.centerY(), 40);
  EXPECT_EQ(s.center(), Point(40, 40));
}

TEST(StickTest, Clear)
{
  Stick s(10, 20, 30, 40, 5, 60);
  EXPECT_TRUE(s.valid());
  s.clear();
  EXPECT_FALSE(s.valid());
  EXPECT_EQ(s.flag, 0);
}

TEST(StickTest, EqualityOperator)
{
  Stick s1(1, 2, 3, 4, 5, 6, 7);
  Stick s2(1, 2, 3, 4, 5, 6, 7);
  Stick s3(2, 2, 3, 4, 5, 6, 7);
  EXPECT_TRUE(s1 == s2);
  EXPECT_FALSE(s1 == s3);
}

TEST(StickTest, InequalityOperator)
{
  Stick s1(1, 2, 3, 4, 5, 6, 7);
  Stick s2(1, 2, 3, 4, 5, 6, 7);
  Stick s3(2, 2, 3, 4, 5, 6, 7);
  EXPECT_FALSE(s1 != s2);
  EXPECT_TRUE(s1 != s3);
}

TEST(StickTest, TopLessThanHigh)
{
  Stick stick(0, 10, 5, 10, 20, 5);
  // 不检查
  ASSERT_TRUE(stick.valid());
}

TEST(StickTest, BottomGreaterThanLow)
{
  Stick stick(0, 10, 5, 10, 5, 0);
  // 不检查
  ASSERT_TRUE(stick.valid());
}

TEST(StickTest, TopGreaterOrEqualThanHighAndBottomLessOrEqualThanLow)
{
  Stick stick(0, 5, 5, 5, 10, 0);
  ASSERT_TRUE(stick.valid());
}

TEST(StickTest, NullRectOrLine)
{
  Stick stick1;
  ASSERT_FALSE(stick1.valid());

  Rect rect;
  Line line;
  Stick stick2(rect, line);
  ASSERT_FALSE(stick2.valid());
}
