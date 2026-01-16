#include <gtest/gtest.h>
#include "src/modules/transaction.h"

class TransactionTest : public ::testing::Test {
protected:
    Transaction defaultTransaction;
    Transaction paramTransaction{100.0, TransactionTargetType::Category, "target1", QDate(2025, 12, 14)};
};

TEST_F(TransactionTest, DefaultConstructor) {
    EXPECT_EQ(defaultTransaction.getId(), "");
    EXPECT_EQ(defaultTransaction.getAmount(), 0.0);
    EXPECT_EQ(defaultTransaction.getTargetType(), TransactionTargetType::None);
    EXPECT_EQ(defaultTransaction.getTargetId(), "");
    EXPECT_EQ(defaultTransaction.getInputDate(), QDate::currentDate());
}

TEST_F(TransactionTest, ParameterizedConstructor) {
    EXPECT_EQ(paramTransaction.getAmount(), 100.0);
    EXPECT_EQ(paramTransaction.getTargetType(), TransactionTargetType::Category);
    EXPECT_EQ(paramTransaction.getTargetId(), "target1");
    EXPECT_EQ(paramTransaction.getInputDate(), QDate(2025, 12, 14));
}

TEST_F(TransactionTest, SettersAndGetters) {
    Transaction t;
    t.setId("tx123");
    t.setAmount(50.5);
    t.setTargetType(TransactionTargetType::Goal);
    t.setTargetId("goal42");
    t.setInputDate(QDate(2025, 1, 1));

    EXPECT_EQ(t.getId(), "tx123");
    EXPECT_EQ(t.getAmount(), 50.5);
    EXPECT_EQ(t.getTargetType(), TransactionTargetType::Goal);
    EXPECT_EQ(t.getTargetId(), "goal42");
    EXPECT_EQ(t.getInputDate(), QDate(2025, 1, 1));
}
