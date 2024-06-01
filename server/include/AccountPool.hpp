#pragma once
#pragma execution_character_set("utf-8")
#include "BasicPool.hpp"
#include <string>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

class Account {
    public:
        enum class AccountType {
            NONETYPE = 0,
            ADMIN = 1,
            USER = 2
        };

    private:
        uint32_t account_id;
        std::string username;
        std::string passwd_hash;
        AccountType account_type;

    public:
        Account();
        Account(uint32_t account_id, std::string username, std::string passwd_hash, AccountType account_type);
        Account(const Account& acc);
        ~Account();
        uint32_t getAccountId() const;
        std::string getUsername() const;
        std::string getPasswdHash() const;
        AccountType getAccountType() const;
        bool is_admin() const;
        bool is_user() const;
        void setAccountId(uint32_t account_id);
        void setUsername(std::string username); 
        void setPasswdHash(std::string passwd_hash);
        void setAccountType(AccountType account_type);

        bool operator == (const Account& acc) const;
        bool operator != (const Account& acc) const;
        bool operator < (const Account& acc) const;
        bool operator > (const Account& acc) const;
        bool operator <= (const Account& acc) const;
        bool operator >= (const Account& acc) const;
        Account& operator = (const Account& acc);
        const static Account NULL_ACCOUNT;
};

class AccountGenerator {
    private:
        uint32_t next_admin_id, next_user_id;
    public:
        AccountGenerator();
        AccountGenerator(uint32_t next_admin_id, uint32_t next_user_id);
        ~AccountGenerator();
        Account newAccount(Account::AccountType account_type);
};

class AccountPool : public BasicPool{
    public:
        enum class AccountVerifyResult {
            SUCCESS = 0,
            ACCOUNT_NOT_FOUND = 1,
            WRONG_PASSWORD = 2
        };

    private:
        std::map<uint32_t, Account> accountpool;
        std::map<uint32_t, Account> adminpool;
        std::map<uint32_t, Account> userpool;

    public:
        AccountPool();
        AccountPool(Account* begin, Account* end);
        AccountPool(std::vector<Account> accounts);
        AccountPool(const AccountPool& ap);
        ~AccountPool();
        int addAccount(Account acc);
        int removeAccount(uint32_t account_id);
        int removeAccount(Account acc);
        int updateAccount(uint32_t account_id, Account acc);
        Account getAccount(uint32_t account_id) const;
        AccountVerifyResult verifyAccount(uint32_t account_id, std::string passwd_hash) const;
        size_t size() const;
        bool empty() const;
        int clear();
        int load(std::string filename);
        int save(std::string filename) const;
        std::vector<Account> list() const;

        bool operator == (const AccountPool& ap) const;
        bool operator != (const AccountPool& ap) const;
        AccountPool& operator = (const AccountPool& ap);
};
