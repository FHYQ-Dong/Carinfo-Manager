#include "AccountPool.hpp"
#include <assert.h>

Account::Account() {
    // Default constructor
    account_id = 0;
    username = "";
    passwd_hash = "";
    account_type = AccountType::NONETYPE;
}

Account::Account(uint32_t account_id, std::string username, std::string passwd_hash, AccountType account_type) {
    // Constructor with parameters
    assert(passwd_hash.length() == 64);
    this->account_id = account_id;
    this->username = username;
    this->passwd_hash = passwd_hash;
    this->account_type = account_type;
}

Account::Account(const Account& acc) {
    // Copy constructor
    account_id = acc.account_id;
    username = acc.username;
    passwd_hash = acc.passwd_hash;
    account_type = acc.account_type;
}

Account::~Account() {
    // Destructor
}

uint32_t Account::getAccountId() const {
    // Getter for account_id
    return account_id;
}

std::string Account::getUsername() const {
    // Getter for username
    return username;
}

std::string Account::getPasswdHash() const {
    // Getter for passwd_hash
    return passwd_hash;
}

Account::AccountType Account::getAccountType() const {
    // Getter for account_type
    return account_type;
}

bool Account::is_admin() const {
    // Check if account is admin
    return account_type == AccountType::ADMIN;
}

bool Account::is_user() const {
    // Check if account is user
    return account_type == AccountType::USER;
}

void Account::setAccountId(uint32_t account_id) {
    // Setter for account_id
    this->account_id = account_id;
}

void Account::setUsername(std::string username) {
    // Setter for username
    this->username = username;
}

void Account::setPasswdHash(std::string passwd_hash) {
    // Setter for passwd_hash
    assert(passwd_hash.length() == 64);
    this->passwd_hash = passwd_hash;
}

void Account::setAccountType(AccountType account_type) {
    // Setter for account_type
    this->account_type = account_type;
}

bool Account::operator == (const Account& acc) const {
    // Equality operator
    return account_id == acc.account_id;
}

bool Account::operator != (const Account& acc) const {
    // Inequality operator
    return account_id != acc.account_id;
}

bool Account::operator < (const Account& acc) const {
    // Less than operator
    return account_id < acc.account_id;
}

bool Account::operator > (const Account& acc) const {
    // Greater than operator
    return account_id > acc.account_id;
}

bool Account::operator <= (const Account& acc) const {
    // Less than or equal to operator
    return account_id <= acc.account_id;
}

bool Account::operator >= (const Account& acc) const {
    // Greater than or equal to operator
    return account_id >= acc.account_id;
}

Account& Account::operator = (const Account& acc) {
    // Assignment operator
    account_id = acc.account_id;
    username = acc.username;
    passwd_hash = acc.passwd_hash;
    account_type = acc.account_type;
    return *this;
}

const Account Account::NULL_ACCOUNT = Account();

AccountGenerator::AccountGenerator() {
    // admin_id: 1xxxxx, user_id: 2xxxxx
    next_admin_id = (uint32_t)(1e10);
    next_user_id = (uint32_t)(2e10);
}

AccountGenerator::AccountGenerator(uint32_t next_admin_id, uint32_t next_user_id) {
    this->next_admin_id = next_admin_id;
    this->next_user_id = next_user_id;
}

AccountGenerator::~AccountGenerator() {
    // Do nothing
}

Account AccountGenerator::newAccount(Account::AccountType account_type) {
    assert(account_type != Account::AccountType::NONETYPE);
    Account acc;
    if (account_type == Account::AccountType::ADMIN) {
        acc.setAccountId(next_admin_id);
        acc.setAccountType(Account::AccountType::ADMIN);
        next_admin_id++;
    }
    else if (account_type == Account::AccountType::USER) {
        acc.setAccountId(next_user_id);
        acc.setAccountType(Account::AccountType::USER);
        next_user_id++;
    }
    return acc;
}

AccountPool::AccountPool() {
    accountpool = std::map<uint32_t, Account>();
    adminpool = std::map<uint32_t, Account>();
    userpool = std::map<uint32_t, Account>();
    sz = 0;
}

AccountPool::AccountPool(Account* begin, Account* end) {
    accountpool = std::map<uint32_t, Account>();
    adminpool = std::map<uint32_t, Account>();
    userpool = std::map<uint32_t, Account>();
    sz = 0;
    for (Account* it = begin; it != end; it++) {
        if (addAccount(*it) != 0) throw "Error: AccountPool::AccountPool(Account* begin, Account* end)";
    }
}

AccountPool::AccountPool(std::vector<Account> accounts) {
    accountpool = std::map<uint32_t, Account>();
    adminpool = std::map<uint32_t, Account>();
    userpool = std::map<uint32_t, Account>();
    sz = 0;
    for (Account acc : accounts) {
        if (addAccount(acc) != 0) throw "Error: AccountPool::AccountPool(std::vector<Account> accounts)";
    }
}

AccountPool::AccountPool(const AccountPool& ap) {
    accountpool = ap.accountpool;
    adminpool = ap.adminpool;
    userpool = ap.userpool;
    sz = ap.sz;
}

AccountPool::~AccountPool() {
    accountpool.clear();
    adminpool.clear();
    userpool.clear();
    sz = 0;
}

int AccountPool::addAccount(Account acc) {
    try {
        if (accountpool.find(acc.getAccountId()) != accountpool.end()) return 1;
        accountpool[acc.getAccountId()] = acc;
        if (acc.getAccountType() == Account::AccountType::ADMIN) {
            adminpool[acc.getAccountId()] = acc;
        }
        else if (acc.getAccountType() == Account::AccountType::USER) {
            userpool[acc.getAccountId()] = acc;
        }
        sz++;
        return 0;
    }
    catch (...) {
        return 1;
    }
}

int AccountPool::removeAccount(uint32_t account_id) {
    try {
        if (accountpool.find(account_id) == accountpool.end()) return 1;
        Account acc = accountpool[account_id];
        accountpool.erase(account_id);
        if (acc.getAccountType() == Account::AccountType::ADMIN) {
            adminpool.erase(account_id);
        }
        else if (acc.getAccountType() == Account::AccountType::USER) {
            userpool.erase(account_id);
        }
        sz--;
        return 0;
    }
    catch (...) {
        return 1;
    }
}

int AccountPool::removeAccount(Account acc) {
    return removeAccount(acc.getAccountId());
}

int AccountPool::updateAccount(uint32_t account_id, Account acc) {
    try {
        if (removeAccount(account_id) != 0) return 1;
        if (addAccount(acc) != 0) return 1;
        return 0;
    }
    catch (...) {
        return 1;
    }
}

Account AccountPool::getAccount(uint32_t account_id) const {
    if (accountpool.find(account_id) == accountpool.end()) return Account::NULL_ACCOUNT;
    return accountpool.at(account_id);
}

AccountPool::AccountVerifyResult AccountPool::verifyAccount(uint32_t account_id, std::string passwd_hash) const {
    if (accountpool.find(account_id) == accountpool.end()) return AccountVerifyResult::ACCOUNT_NOT_FOUND;
    if (accountpool.at(account_id).getPasswdHash() != passwd_hash) return AccountVerifyResult::WRONG_PASSWORD;
    return AccountVerifyResult::SUCCESS;
}

size_t AccountPool::size() const {
    return sz;
}

bool AccountPool::empty() const {
    return sz == 0;
}

bool AccountPool::operator == (const AccountPool& ap) const {
    return sz == ap.sz && accountpool == ap.accountpool && adminpool == ap.adminpool && userpool == ap.userpool;
}

bool AccountPool::operator != (const AccountPool& ap) const {
    return sz != ap.sz || accountpool != ap.accountpool || adminpool != ap.adminpool || userpool != ap.userpool;
}

AccountPool& AccountPool::operator = (const AccountPool& ap) {
    sz = ap.sz;
    accountpool = ap.accountpool;
    adminpool = ap.adminpool;
    userpool = ap.userpool;
    return *this;
}
