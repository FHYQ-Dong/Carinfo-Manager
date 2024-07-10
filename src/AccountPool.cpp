#include "carinfo-manager/accountpool.hpp"
#include <assert.h>
#include <fstream>

Account::Account() {
    // Default constructor
    username = "";
    passwd_hash = "";
    account_type = AccountType::NONETYPE;
}

Account::Account(const std::string& username, const  std::string& passwd_hash, const AccountType& account_type) {
    // Constructor with parameters
    assert(passwd_hash.length() == 64);
    this->username = username;
    this->passwd_hash = passwd_hash;
    this->account_type = account_type;
}

Account::Account(const Account& acc) {
    // Copy constructor
    username = acc.username;
    passwd_hash = acc.passwd_hash;
    account_type = acc.account_type;
}

Account::~Account() {
    // Destructor
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

void Account::setUsername(const std::string& username) {
    // Setter for username
    this->username = username;
}

void Account::setPasswdHash(const std::string& passwd_hash) {
    // Setter for passwd_hash
    assert(passwd_hash.length() == 64);
    this->passwd_hash = passwd_hash;
}

void Account::setAccountType(const AccountType& account_type) {
    // Setter for account_type
    this->account_type = account_type;
}

bool Account::operator == (const Account& acc) const {
    // Equality operator
    return username == acc.username;
}

bool Account::operator != (const Account& acc) const {
    // Inequality operator
    return username != acc.username;
}

bool Account::operator < (const Account& acc) const {
    // Less than operator
    return username < acc.username;
}

bool Account::operator > (const Account& acc) const {
    // Greater than operator
    return username > acc.username;
}

bool Account::operator <= (const Account& acc) const {
    // Less than or equal to operator
    return username <= acc.username;
}

bool Account::operator >= (const Account& acc) const {
    // Greater than or equal to operator
    return username >= acc.username;
}

Account& Account::operator = (const Account& acc) {
    // Assignment operator
    username = acc.username;
    passwd_hash = acc.passwd_hash;
    account_type = acc.account_type;
    return *this;
}

const Account Account::NULL_ACCOUNT = Account();

AccountPool::AccountPool() {
    accountpool = std::map<std::string, Account>();
    adminpool = std::map<std::string, Account>();
    userpool = std::map<std::string, Account>();
    sz = 0;
}

AccountPool::AccountPool(Account* begin, Account* end) {
    accountpool = std::map<std::string, Account>();
    adminpool = std::map<std::string, Account>();
    userpool = std::map<std::string, Account>();
    sz = 0;
    for (Account* it = begin; it != end; it++) {
        if (addAccount(*it) != 0) throw "Error: AccountPool::AccountPool(Account* begin, Account* end)";
    }
}

AccountPool::AccountPool(const std::vector<Account>& accounts) {
    accountpool = std::map<std::string, Account>();
    adminpool = std::map<std::string, Account>();
    userpool = std::map<std::string, Account>();
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

int AccountPool::addAccount(const Account& acc) {
    try {
        if (accountpool.find(acc.getUsername()) != accountpool.end()) return 0x10;
        accountpool[acc.getUsername()] = acc;
        if (acc.getAccountType() == Account::AccountType::ADMIN) {
            adminpool[acc.getUsername()] = acc;
        }
        else if (acc.getAccountType() == Account::AccountType::USER) {
            userpool[acc.getUsername()] = acc;
        }
        sz++;
        return 0;
    }
    catch (...) {
        return 0x1F;
    }
}

int AccountPool::removeAccount(const std::string& username) {
    try {
        if (accountpool.find(username) == accountpool.end()) return 0x20;
        Account acc = accountpool[username];
        accountpool.erase(username);
        if (acc.getAccountType() == Account::AccountType::ADMIN) {
            adminpool.erase(username);
        }
        else if (acc.getAccountType() == Account::AccountType::USER) {
            userpool.erase(username);
        }
        sz--;
        return 0;
    }
    catch (...) {
        return 0x2F;
    }
}

int AccountPool::removeAccount(const Account& acc) {
    return removeAccount(acc.getUsername());
}

int AccountPool::updateAccount(const Account& original_acc, const Account& new_acc) {
    try {
        if (removeAccount(original_acc) != 0) return 0x30;
        if (addAccount(new_acc) != 0) return 0x31;
        return 0;
    }
    catch (...) {
        return 0x3F;
    }
}

Account AccountPool::getAccount(const std::string& username) const {
    if (accountpool.find(username) == accountpool.end()) return Account::NULL_ACCOUNT;
    return accountpool.at(username);
}

AccountPool::AccountVerifyResult AccountPool::verifyAccount(const std::string& username, const std::string& passwd_hash) const {
    if (accountpool.find(username) == accountpool.end()) return AccountVerifyResult::ACCOUNT_NOT_FOUND;
    if (accountpool.at(username).getPasswdHash() != passwd_hash) return AccountVerifyResult::WRONG_PASSWORD;
    return AccountVerifyResult::SUCCESS;
}

size_t AccountPool::size() const {
    return sz;
}

bool AccountPool::empty() const {
    return sz == 0;
}

int AccountPool::clear() {
    try {
        accountpool.clear();
        adminpool.clear();
        userpool.clear();
        sz = 0;
        return 0;
    }
    catch (...) {
        return 0x4F;
    }
}

int AccountPool::load(std::istream& is) {
    if (!is) return 0x50;
    try {
        if (clear() != 0) return 0x51;
        // Read size
        size_t sz_tmp = 0;
        is.read((char*)&sz_tmp, sizeof(size_t));
        for (size_t i = 0; i < sz_tmp; i++) {
            // Read one Account
            std::string username, passwd_hash;
            char acc_type;
            // Read username
            size_t username_len = 0;
            is.read((char*)&username_len, sizeof(size_t));
            char* username_buf = new char[username_len + 1];
            is.read(username_buf, username_len * sizeof(char));
            username_buf[username_len] = '\0';
            username = std::string(username_buf);
            delete[] username_buf;
            // Read passwd_hash
            size_t passwd_hash_len = 0;
            is.read((char*)&passwd_hash_len, sizeof(size_t));
            char* passwd_hash_buf = new char[passwd_hash_len + 1];
            is.read(passwd_hash_buf, passwd_hash_len * sizeof(char));
            passwd_hash_buf[passwd_hash_len] = '\0';
            passwd_hash = std::string(passwd_hash_buf);
            delete[] passwd_hash_buf;
            // Read account_type
            is.read(&acc_type, sizeof(char));
            Account acc(username, passwd_hash, (Account::AccountType)acc_type);
            if (addAccount(acc)) return 0x53;
        }
        return 0;
    }
    catch (...) {
        return 0x5F;
    }
}

int AccountPool::save(std::ostream& os) const {
    if (!os) return 0x60;
    try {
        // Write size
        os.write((char*)&sz, sizeof(size_t));
        for (auto it = accountpool.begin(); it != accountpool.end(); it++) {
            // Save one Account
            Account acc = it->second;
            // Write username
            size_t username_len = acc.getUsername().length();
            os.write((char*)&username_len, sizeof(size_t));
            os.write(acc.getUsername().c_str(), username_len * sizeof(char));
            // Write passwd_hash
            size_t passwd_hash_len = acc.getPasswdHash().length();
            os.write((char*)&passwd_hash_len, sizeof(size_t));
            os.write(acc.getPasswdHash().c_str(), acc.getPasswdHash().length() * sizeof(char));
            // Write account_type
            char acc_type = (char)acc.getAccountType();
            os.write(&acc_type, sizeof(char));
        }
        return 0;
    }
    catch (...) {
        return 0x6F;
    }
}

std::vector<Account> AccountPool::list() const {
    std::vector<Account> accounts;
    for (auto it = accountpool.begin(); it != accountpool.end(); it++) {
        accounts.push_back(it->second);
    }
    return accounts;
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
