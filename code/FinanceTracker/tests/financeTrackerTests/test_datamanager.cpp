#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "src/modules/datamanager.h"
#include "src/modules/goal.h"
#include "src/modules/transaction.h"
#include "src/modules/category.h"

using ::testing::_;
using ::testing::Exactly;

class MockObserver {
public:
    MOCK_METHOD(void, onGoalsChanged, (), ());
    MOCK_METHOD(void, onTransactionsChanged, (), ());
};

class DataManagerTest : public ::testing::Test {
protected:
    DataManager& dm = DataManager::instance();

    void SetUp() override {
        dm.clear();
    }

    void TearDown() override {
        dm.clear();
    }
};

TEST_F(DataManagerTest, AddAndRetrieveGoal)
{
    Goal g;
    g.setName("Buy Car");

    dm.addGoal(g);

    Goal result = dm.getGoal("Buy Car");

    EXPECT_EQ(result.getName(), "Buy Car");
}

TEST_F(DataManagerTest, RemovingGoalAlsoRemovesLinkedTransactions)
{
    Goal g;
    g.setName("Vacation");
    dm.addGoal(g);

    Transaction t;
    t.setTargetId("Vacation");
    dm.addTransaction(t);

    EXPECT_EQ(dm.getTransactions().size(), 1);

    dm.removeGoal("Vacation");

    EXPECT_EQ(dm.getGoals().size(), 0);
    EXPECT_EQ(dm.getTransactions().size(), 0);
}


TEST_F(DataManagerTest, AddAndUpdateCategory)
{
    Category c;
    c.setCategoryName("Food");

    dm.addCategory(c);

    Category fetched = dm.getCategory("1");
    EXPECT_EQ(fetched.getCategoryName(), "Food");

    Category updated;
    updated.setCategoryName("Groceries");

    dm.updateCategory("1", updated);

    fetched = dm.getCategory("1");
    EXPECT_EQ(fetched.getCategoryName(), "Groceries");
}

TEST(DataManagerMockTest, ExpectCallsOnMockObserver)
{
    MockObserver mock;

    EXPECT_CALL(mock, onGoalsChanged()).Times(Exactly(1));
    EXPECT_CALL(mock, onTransactionsChanged()).Times(Exactly(1));

    mock.onGoalsChanged();
    mock.onTransactionsChanged();
}
