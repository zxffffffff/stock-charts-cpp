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

#include "../lib-chart/Core/DataBinding.h"
using namespace StockCharts;

class MockDataBinding : public DataBinding
{
public:
    MOCK_METHOD(void, on, (DataBinding * sender, const std::string &id), (override));
};

TEST(DataBindingTest, BindAndunlisten)
{
    MockDataBinding listener1;
    MockDataBinding listener2;
    MockDataBinding sender;

    // 检查on()函数是否按预期被调用
    EXPECT_CALL(listener1, on(&sender, "test"));
    EXPECT_CALL(listener2, on(&sender, "test"));

    listener1.listen(&sender);
    listener2.listen(&sender);
    sender.fire("test");

    // 确保on()函数不再被调用
    EXPECT_CALL(listener1, on(&sender, "test")).Times(0);
    EXPECT_CALL(listener2, on(&sender, "test")).Times(0);

    listener1.unlisten(&sender);
    listener2.unlisten(&sender);
    sender.fire("test1");
}

TEST(DataBindingTest, FireWithoutListeners)
{
    MockDataBinding sender;

    EXPECT_CALL(sender, on(_, _)).Times(0);

    sender.fire("test");
}

TEST(DataBindingTest, unlistenNotBoundListener)
{
    MockDataBinding listener;
    MockDataBinding sender;

    EXPECT_CALL(listener, on(_, _)).Times(0);

    sender.unlisten(&listener);
}
