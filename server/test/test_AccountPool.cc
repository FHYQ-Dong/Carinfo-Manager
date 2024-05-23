#include "AccountPool.hpp"
#include "Hash.hpp"
#include <gtest/gtest.h>

TEST(Test_AccountGenerator, test_AccountGenerator_default_constructor) {
    AccountGenerator accountgenerator;
    EXPECT_EQ(accountgenerator.newAccount(Account::AccountType::ADMIN).getAccountId(), (uint32_t)(1e10));
    EXPECT_EQ(accountgenerator.newAccount(Account::AccountType::USER).getAccountId(), (uint32_t)(2e10));
    EXPECT_EQ(accountgenerator.newAccount(Account::AccountType::ADMIN).getAccountId(), (uint32_t)(1e10) + 1);
    EXPECT_EQ(accountgenerator.newAccount(Account::AccountType::USER).getAccountId(), (uint32_t)(2e10) + 1);
}

TEST(Test_AccountGenerator, test_AccountGenerator_parameterized_constructor) {
    AccountGenerator accountgenerator(10, 20);
    EXPECT_EQ(accountgenerator.newAccount(Account::AccountType::ADMIN).getAccountId(), 10);
    EXPECT_EQ(accountgenerator.newAccount(Account::AccountType::USER).getAccountId(), 20);
    EXPECT_EQ(accountgenerator.newAccount(Account::AccountType::ADMIN).getAccountId(), 11);
    EXPECT_EQ(accountgenerator.newAccount(Account::AccountType::USER).getAccountId(), 21);
}

TEST(Test_AccountGenerator, test_AccountGenerator_newAccount) {
    AccountGenerator accountgenerator;
    Account acc1 = accountgenerator.newAccount(Account::AccountType::ADMIN);
    Account acc2 = accountgenerator.newAccount(Account::AccountType::USER);
    Account acc3 = accountgenerator.newAccount(Account::AccountType::USER);
    EXPECT_EQ(acc1.getAccountId(), (uint32_t)(1e10));
    EXPECT_EQ(acc2.getAccountId(), (uint32_t)(2e10));
    EXPECT_EQ(acc3.getAccountId(), (uint32_t)(2e10) + 1);
}

TEST(Test_AccountPool, test_AccountPool_default_constructor) {
    AccountPool accountpool;
    EXPECT_EQ(accountpool.size(), 0);
    EXPECT_TRUE(accountpool.empty());
}

TEST(Test_AccountPool, test_AccountPool_parameterized_constructor) {
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1(1, "admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2(2, "user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3(3, "user2", hash3.getHash(), Account::AccountType::USER);
    Account accounts[] = {acc1, acc2, acc3};
    AccountPool accountpool(accounts, accounts + 3);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
}

TEST(Test_AccountPool, test_AccountPool_parameterized_constructor_vector) {
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1(1, "admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2(2, "user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3(3, "user2", hash3.getHash(), Account::AccountType::USER);
    std::vector<Account> accounts = {acc1, acc2, acc3};
    AccountPool accountpool(accounts);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
}

TEST(Test_AccountPool, test_AccountPool_copy_constructor) {
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1(1, "admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2(2, "user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3(3, "user2", hash3.getHash(), Account::AccountType::USER);
    Account accounts[] = {acc1, acc2, acc3};
    AccountPool accountpool1(accounts, accounts + 3);
    AccountPool accountpool2(accountpool1);
    EXPECT_EQ(accountpool2.size(), 3);
    EXPECT_FALSE(accountpool2.empty());
}

TEST(Test_AccountPool, test_AccountPool_addAccount) {
    AccountPool accountpool;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1(1, "admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2(2, "user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3(3, "user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool.addAccount(acc1), 0);
    EXPECT_EQ(accountpool.addAccount(acc2), 0);
    EXPECT_EQ(accountpool.addAccount(acc3), 0);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    EXPECT_EQ(accountpool.addAccount(acc1), 1);
    EXPECT_EQ(accountpool.addAccount(acc2), 1);
    EXPECT_EQ(accountpool.addAccount(acc3), 1);
}

TEST(Test_AccountPool, test_AccountPool_removeAccount) {
    AccountPool accountpool;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1(1, "admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2(2, "user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3(3, "user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool.addAccount(acc1), 0);
    EXPECT_EQ(accountpool.addAccount(acc2), 0);
    EXPECT_EQ(accountpool.addAccount(acc3), 0);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    EXPECT_EQ(accountpool.removeAccount(acc1), 0);
    EXPECT_EQ(accountpool.removeAccount(acc2), 0);
    EXPECT_EQ(accountpool.removeAccount(acc3), 0);
    EXPECT_EQ(accountpool.size(), 0);
    EXPECT_TRUE(accountpool.empty());
    EXPECT_EQ(accountpool.removeAccount(acc1), 1);
    EXPECT_EQ(accountpool.removeAccount(acc2), 1);
    EXPECT_EQ(accountpool.removeAccount(acc3), 1);
}

TEST(Test_AccountPool, test_AccountPool_updateAccount) {
    AccountPool accountpool;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1(1, "admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2(2, "user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3(3, "user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool.addAccount(acc1), 0);
    EXPECT_EQ(accountpool.addAccount(acc2), 0);
    EXPECT_EQ(accountpool.addAccount(acc3), 0);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    Hash hash4("password4", "salt4"), hash5("password5", "salt5"), hash6("password6", "salt6");
    Account acc4(1, "admin2", hash4.getHash(), Account::AccountType::ADMIN);
    Account acc5(2, "user3", hash5.getHash(), Account::AccountType::USER);
    Account acc6(3, "user4", hash6.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool.updateAccount(1, acc4), 0);
    EXPECT_EQ(accountpool.updateAccount(2, acc5), 0);
    EXPECT_EQ(accountpool.updateAccount(3, acc6), 0);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    EXPECT_EQ(accountpool.updateAccount(4, acc4), 1);
    EXPECT_EQ(accountpool.updateAccount(5, acc5), 1);
    EXPECT_EQ(accountpool.updateAccount(6, acc6), 1);
}

TEST(Test_AccountPool, test_AccountPool_getAccount) {
    AccountPool accountpool;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1(1, "admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2(2, "user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3(3, "user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool.addAccount(acc1), 0);
    EXPECT_EQ(accountpool.addAccount(acc2), 0);
    EXPECT_EQ(accountpool.addAccount(acc3), 0);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    EXPECT_EQ(accountpool.getAccount(1), acc1);
    EXPECT_EQ(accountpool.getAccount(2), acc2);
    EXPECT_EQ(accountpool.getAccount(3), acc3);
    EXPECT_EQ(accountpool.getAccount(4), Account::NULL_ACCOUNT);
    EXPECT_EQ(accountpool.getAccount(5), Account::NULL_ACCOUNT);
    EXPECT_EQ(accountpool.getAccount(6), Account::NULL_ACCOUNT);
}

TEST(Test_AccountPool, test_AccountPool_verifyAccount) {
    AccountPool accountpool;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1(1, "admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2(2, "user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3(3, "user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool.addAccount(acc1), 0);
    EXPECT_EQ(accountpool.addAccount(acc2), 0);
    EXPECT_EQ(accountpool.addAccount(acc3), 0);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    EXPECT_EQ(accountpool.verifyAccount(1, hash1.getHash()), AccountPool::AccountVerifyResult::SUCCESS);
    EXPECT_EQ(accountpool.verifyAccount(2, hash2.getHash()), AccountPool::AccountVerifyResult::SUCCESS);
    EXPECT_EQ(accountpool.verifyAccount(3, hash3.getHash()), AccountPool::AccountVerifyResult::SUCCESS);
    EXPECT_EQ(accountpool.verifyAccount(1, hash2.getHash()), AccountPool::AccountVerifyResult::WRONG_PASSWORD);
    EXPECT_EQ(accountpool.verifyAccount(2, hash3.getHash()), AccountPool::AccountVerifyResult::WRONG_PASSWORD);
    EXPECT_EQ(accountpool.verifyAccount(3, hash1.getHash()), AccountPool::AccountVerifyResult::WRONG_PASSWORD);
    EXPECT_EQ(accountpool.verifyAccount(4, hash1.getHash()), AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND);
    EXPECT_EQ(accountpool.verifyAccount(5, hash2.getHash()), AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND);
    EXPECT_EQ(accountpool.verifyAccount(6, hash3.getHash()), AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND);
}

TEST(Test_AccountPool, test_AccountPool_operator_eq) {
    AccountPool accountpool1;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1(1, "admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2(2, "user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3(3, "user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool1.addAccount(acc1), 0);
    EXPECT_EQ(accountpool1.addAccount(acc2), 0);
    EXPECT_EQ(accountpool1.addAccount(acc3), 0);
    EXPECT_EQ(accountpool1.size(), 3);
    EXPECT_FALSE(accountpool1.empty());
    AccountPool accountpool2;
    accountpool2 = accountpool1;
    EXPECT_EQ(accountpool2.size(), 3);
    EXPECT_FALSE(accountpool2.empty());
}

TEST(Test_AccountPool, test_AccountPool_operator_ne) {
    AccountPool accountpool1;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1(1, "admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2(2, "user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3(3, "user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool1.addAccount(acc1), 0);
    EXPECT_EQ(accountpool1.addAccount(acc2), 0);
    EXPECT_EQ(accountpool1.addAccount(acc3), 0);
    EXPECT_EQ(accountpool1.size(), 3);
    EXPECT_FALSE(accountpool1.empty());
    AccountPool accountpool2;
    EXPECT_NE(accountpool1, accountpool2);
    accountpool2 = accountpool1;
    EXPECT_EQ(accountpool1, accountpool2);
    accountpool2.removeAccount(1);
    EXPECT_NE(accountpool1, accountpool2);
    accountpool2.addAccount(acc1);
    EXPECT_EQ(accountpool1, accountpool2);
}

TEST(Test_AccountPool, test_AccountPool_operator_assign) {
    AccountPool accountpool1;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1(1, "admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2(2, "user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3(3, "user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool1.addAccount(acc1), 0);
    EXPECT_EQ(accountpool1.addAccount(acc2), 0);
    EXPECT_EQ(accountpool1.addAccount(acc3), 0);
    EXPECT_EQ(accountpool1.size(), 3);
    EXPECT_FALSE(accountpool1.empty());
    AccountPool accountpool2;
    accountpool2 = accountpool1;
    EXPECT_EQ(accountpool2.size(), 3);
    EXPECT_FALSE(accountpool2.empty());
    accountpool2.removeAccount(1);
    EXPECT_EQ(accountpool2.size(), 2);
    EXPECT_FALSE(accountpool2.empty());
    accountpool2 = accountpool1;
    EXPECT_EQ(accountpool2.size(), 3);
    EXPECT_FALSE(accountpool2.empty());
}