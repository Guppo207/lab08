#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Account.h>
#include <Transaction.h>

using ::testing::_;
using ::testing::Expectation;

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}
    
    MOCK_METHOD(void, Lock, ());
    MOCK_METHOD(void, Unlock, ());
    MOCK_METHOD(int, id, (), (const));
    MOCK_METHOD(void, ChangeBalance, (int diff));
    MOCK_METHOD(int, GetBalance, ());
};

class MockTransaction : public Transaction {
public:
    MOCK_METHOD(bool, Make, (Account& from, Account& to, int sum));
    MOCK_METHOD(void, set_fee, (int fee));
    MOCK_METHOD(int, fee, ());
};

namespace AccountTests {
    TEST(AccountTest, ShouldHandleBalanceAndIdOperations) {
        MockAccount account(1, 100);
        
        Expectation get_balance = EXPECT_CALL(account, GetBalance()).Times(3);
        Expectation lock = EXPECT_CALL(account, Lock()).Times(1).After(get_balance);
        Expectation unlock = EXPECT_CALL(account, Unlock()).Times(1);
        EXPECT_CALL(account, ChangeBalance(_)).Times(2);
        EXPECT_CALL(account, id()).Times(1);

        account.GetBalance();
        account.id();
        account.Unlock();
        account.ChangeBalance(1000);
        account.GetBalance();
        account.ChangeBalance(2);
        account.GetBalance();
        account.Lock();
    }

    TEST(AccountTest, ShouldThrowWhenNotLocked) {
        Account account(0, 100);
        
        EXPECT_THROW(account.ChangeBalance(50), std::runtime_error);
        
        account.Lock();
        account.ChangeBalance(50);
        EXPECT_EQ(account.GetBalance(), 150);
        
        EXPECT_THROW(account.Lock(), std::runtime_error);
        account.Unlock();
    }
}

namespace TransactionTests {
    TEST(TransactionTest, ShouldHandleMoneyTransfer) {
        MockTransaction transaction;
        MockAccount account1(1, 100);
        MockAccount account2(2, 250);
        MockAccount flat_org(3, 10000);
        MockAccount organization(4, 5000);

        EXPECT_CALL(transaction, set_fee(_)).Times(1);
        EXPECT_CALL(transaction, fee()).Times(1);
        EXPECT_CALL(transaction, Make(_, _, _)).Times(2);
        EXPECT_CALL(account1, GetBalance()).Times(1);
        EXPECT_CALL(account2, GetBalance()).Times(1);

        transaction.set_fee(300);
        transaction.Make(account1, account2, 2000);
        transaction.fee();
        account1.GetBalance();
        account2.GetBalance();
        transaction.Make(organization, account1, 1000);
    }
}
