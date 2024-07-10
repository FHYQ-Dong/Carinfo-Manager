#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/Hash.hpp"
#include "gtest/gtest.h"
#include <fstream>

TEST(Test_AccountPool, test_AccountPool_default_constructor) {
    AccountPool accountpool;
    EXPECT_EQ(accountpool.size(), 0);
    EXPECT_TRUE(accountpool.empty());
}

TEST(Test_AccountPool, test_AccountPool_parameterized_constructor) {
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1("admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2("user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3("user2", hash3.getHash(), Account::AccountType::USER);
    Account accounts[] = {acc1, acc2, acc3};
    AccountPool accountpool(accounts, accounts + 3);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
}

TEST(Test_AccountPool, test_AccountPool_parameterized_constructor_vector) {
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1("admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2("user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3("user2", hash3.getHash(), Account::AccountType::USER);
    std::vector<Account> accounts = {acc1, acc2, acc3};
    AccountPool accountpool(accounts);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
}

TEST(Test_AccountPool, test_AccountPool_copy_constructor) {
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1("admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2("user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3("user2", hash3.getHash(), Account::AccountType::USER);
    Account accounts[] = {acc1, acc2, acc3};
    AccountPool accountpool1(accounts, accounts + 3);
    AccountPool accountpool2(accountpool1);
    EXPECT_EQ(accountpool2.size(), 3);
    EXPECT_FALSE(accountpool2.empty());
}

TEST(Test_AccountPool, test_AccountPool_addAccount) {
    AccountPool accountpool;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1("admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2("user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3("user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool.addAccount(acc1), 0);
    EXPECT_EQ(accountpool.addAccount(acc2), 0);
    EXPECT_EQ(accountpool.addAccount(acc3), 0);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    EXPECT_EQ(accountpool.addAccount(acc1), 0x10);
    EXPECT_EQ(accountpool.addAccount(acc2), 0x10);
    EXPECT_EQ(accountpool.addAccount(acc3), 0x10);
}

TEST(Test_AccountPool, test_AccountPool_removeAccount) {
    AccountPool accountpool;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1("admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2("user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3("user2", hash3.getHash(), Account::AccountType::USER);
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
    EXPECT_EQ(accountpool.removeAccount(acc1), 0x20);
    EXPECT_EQ(accountpool.removeAccount(acc2), 0x20);
    EXPECT_EQ(accountpool.removeAccount(acc3), 0x20);
}

TEST(Test_AccountPool, test_AccountPool_updateAccount) {
    AccountPool accountpool;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1("admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2("user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3("user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool.addAccount(acc1), 0);
    EXPECT_EQ(accountpool.addAccount(acc2), 0);
    EXPECT_EQ(accountpool.addAccount(acc3), 0);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    Hash hash4("password4", "salt4"), hash5("password5", "salt5"), hash6("password6", "salt6");
    Account acc4("admin2", hash4.getHash(), Account::AccountType::ADMIN);
    Account acc5("user3", hash5.getHash(), Account::AccountType::USER);
    Account acc6("user4", hash6.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool.updateAccount(acc1, acc4), 0);
    EXPECT_EQ(accountpool.updateAccount(acc2, acc5), 0);
    EXPECT_EQ(accountpool.updateAccount(acc3, acc6), 0);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    EXPECT_EQ(accountpool.updateAccount(acc1, acc4), 0x30);
    EXPECT_EQ(accountpool.updateAccount(acc2, acc5), 0x30);
    EXPECT_EQ(accountpool.updateAccount(acc3, acc6), 0x30);
}

TEST(Test_AccountPool, test_AccountPool_getAccount) {
    AccountPool accountpool;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1("admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2("user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3("user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool.addAccount(acc1), 0);
    EXPECT_EQ(accountpool.addAccount(acc2), 0);
    EXPECT_EQ(accountpool.addAccount(acc3), 0);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    EXPECT_EQ(accountpool.getAccount("admin1"), acc1);
    EXPECT_EQ(accountpool.getAccount("user1"), acc2);
    EXPECT_EQ(accountpool.getAccount("user2"), acc3);
    EXPECT_EQ(accountpool.getAccount("admin2"), Account::NULL_ACCOUNT);
    EXPECT_EQ(accountpool.getAccount("user3"), Account::NULL_ACCOUNT);
    EXPECT_EQ(accountpool.getAccount("user4"), Account::NULL_ACCOUNT);
}

TEST(Test_AccountPool, test_AccountPool_verifyAccount) {
    AccountPool accountpool;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1("admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2("user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3("user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool.addAccount(acc1), 0);
    EXPECT_EQ(accountpool.addAccount(acc2), 0);
    EXPECT_EQ(accountpool.addAccount(acc3), 0);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    EXPECT_EQ(accountpool.verifyAccount("admin1", hash1.getHash()), AccountPool::AccountVerifyResult::SUCCESS);
    EXPECT_EQ(accountpool.verifyAccount("user1", hash2.getHash()), AccountPool::AccountVerifyResult::SUCCESS);
    EXPECT_EQ(accountpool.verifyAccount("user2", hash3.getHash()), AccountPool::AccountVerifyResult::SUCCESS);
    EXPECT_EQ(accountpool.verifyAccount("admin1", hash2.getHash()), AccountPool::AccountVerifyResult::WRONG_PASSWORD);
    EXPECT_EQ(accountpool.verifyAccount("user1", hash3.getHash()), AccountPool::AccountVerifyResult::WRONG_PASSWORD);
    EXPECT_EQ(accountpool.verifyAccount("user2", hash1.getHash()), AccountPool::AccountVerifyResult::WRONG_PASSWORD);
    EXPECT_EQ(accountpool.verifyAccount("admin2", hash1.getHash()), AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND);
    EXPECT_EQ(accountpool.verifyAccount("user3", hash2.getHash()), AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND);
    EXPECT_EQ(accountpool.verifyAccount("user4", hash3.getHash()), AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND);
}

TEST(Test_AccountPool, test_AccountPool_save) {
    AccountPool accountpool;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1("admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2("user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3("user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool.addAccount(acc1), 0);
    EXPECT_EQ(accountpool.addAccount(acc2), 0);
    EXPECT_EQ(accountpool.addAccount(acc3), 0);
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    std::ofstream ofs("test_AccountPool_save.dat", std::ios::binary);
    ASSERT_EQ(accountpool.save(ofs), 0);
    ofs.close();
}

TEST(Test_AccountPool, test_AccountPool_load) {
    AccountPool accountpool;
    std::ifstream ifs("test_AccountPool_save.dat", std::ios::binary);
    ASSERT_EQ(accountpool.load(ifs), 0);
    ifs.close();
    EXPECT_EQ(accountpool.size(), 3);
    EXPECT_FALSE(accountpool.empty());
    EXPECT_EQ(accountpool.getAccount("admin1").getUsername(), "admin1");
    EXPECT_EQ(accountpool.getAccount("user1").getUsername(), "user1");
    EXPECT_EQ(accountpool.getAccount("user2").getUsername(), "user2");
}

TEST(Test_AccountPool, test_AccountPool_operator_eq) {
    AccountPool accountpool1;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1("admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2("user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3("user2", hash3.getHash(), Account::AccountType::USER);
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
    Account acc1("admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2("user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3("user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool1.addAccount(acc1), 0);
    EXPECT_EQ(accountpool1.addAccount(acc2), 0);
    EXPECT_EQ(accountpool1.addAccount(acc3), 0);
    EXPECT_EQ(accountpool1.size(), 3);
    EXPECT_FALSE(accountpool1.empty());
    AccountPool accountpool2;
    EXPECT_NE(accountpool1, accountpool2);
    accountpool2 = accountpool1;
    EXPECT_EQ(accountpool1, accountpool2);
    accountpool2.removeAccount("admin1");
    EXPECT_NE(accountpool1, accountpool2);
    accountpool2.addAccount(acc1);
    EXPECT_EQ(accountpool1, accountpool2);
}

TEST(Test_AccountPool, test_AccountPool_operator_assign) {
    AccountPool accountpool1;
    Hash hash1("password1", "salt1"), hash2("password2", "salt2"), hash3("password3", "salt3");
    Account acc1("admin1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2("user1", hash2.getHash(), Account::AccountType::USER);
    Account acc3("user2", hash3.getHash(), Account::AccountType::USER);
    EXPECT_EQ(accountpool1.addAccount(acc1), 0);
    EXPECT_EQ(accountpool1.addAccount(acc2), 0);
    EXPECT_EQ(accountpool1.addAccount(acc3), 0);
    EXPECT_EQ(accountpool1.size(), 3);
    EXPECT_FALSE(accountpool1.empty());
    AccountPool accountpool2;
    accountpool2 = accountpool1;
    EXPECT_EQ(accountpool2.size(), 3);
    EXPECT_FALSE(accountpool2.empty());
    accountpool2.removeAccount("admin1");
    EXPECT_EQ(accountpool2.size(), 2);
    EXPECT_FALSE(accountpool2.empty());
    accountpool2 = accountpool1;
    EXPECT_EQ(accountpool2.size(), 3);
    EXPECT_FALSE(accountpool2.empty());
}
