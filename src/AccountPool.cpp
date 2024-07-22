/**
 * @file src/accountpool.cpp
 * @brief Implementation of class Account and class AccountPool, which are used to manage accounts in the system, including loading and saving accounts from and to files, adding, removing, updating and verifying accounts, etc. The class Account is used to represent an account, and the class AccountPool is used to manage a pool of accounts. The class AccountPool is derived from the class BasicPool, which is a base class for all pool classes in the system.
 * 
 * @details
 * The Account class represents an account in the system. It stores the username, password hash, and account type.
 * The AccountPool class manages a pool of accounts. It provides functions to add, remove, update, and verify accounts.
 * The AccountPool class is derived from the BasicPool class, which is a base class for all pool classes in the system.
 * The AccountPool class uses three internal maps to store the accounts: accountpool, adminpool, and userpool. The accountpool map stores all accounts, while the adminpool and userpool maps store only the admin and user accounts, respectively.
 * The AccountPool class provides functions to load and save accounts from and to files, as well as functions to retrieve the size of the account pool and check if it is empty.
 * The AccountPool class also provides functions to clear the account pool and get a list of all accounts.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/accountpool.hpp"
#include "json/json.hpp"
#include <assert.h>
#include <fstream>
using json = nlohmann::json;

Account::Account() {
    username = "";
    passwd_hash = "";
    account_type = AccountType::NONETYPE;
}

Account::Account(const std::string& username, const  std::string& passwd_hash, const AccountType& account_type) {
    assert(passwd_hash.length() == 64);
    this->username = username;
    this->passwd_hash = passwd_hash;
    this->account_type = account_type;
}

Account::Account(const Account& acc) {
    username = acc.username;
    passwd_hash = acc.passwd_hash;
    account_type = acc.account_type;
}

Account::~Account() {}

std::string Account::getUsername() const {
    return username;
}

std::string Account::getPasswdHash() const {
    return passwd_hash;
}

Account::AccountType Account::getAccountType() const {
    return account_type;
}

bool Account::is_admin() const {
    return account_type == AccountType::ADMIN;
}

bool Account::is_user() const {
    return account_type == AccountType::USER;
}

void Account::setUsername(const std::string& username) {
    this->username = username;
}

void Account::setPasswdHash(const std::string& passwd_hash) {
    assert(passwd_hash.length() == 64);
    this->passwd_hash = passwd_hash;
}

void Account::setAccountType(const AccountType& account_type) {
    this->account_type = account_type;
}

bool Account::operator == (const Account& acc) const {
    return username == acc.username;
}

bool Account::operator != (const Account& acc) const {
    return username != acc.username;
}

bool Account::operator < (const Account& acc) const {
    return username < acc.username;
}

bool Account::operator > (const Account& acc) const {
    return username > acc.username;
}

bool Account::operator <= (const Account& acc) const {
    return username <= acc.username;
}

bool Account::operator >= (const Account& acc) const {
    return username >= acc.username;
}

Account& Account::operator = (const Account& acc) {
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

/**
 * @brief Adds an account to the account pool.
 * 
 * This function adds the specified account to the account pool. It checks if the account already exists in the pool and returns an error code if it does. If the account type is ADMIN, it also adds the account to the admin pool. If the account type is USER, it adds the account to the user pool. Finally, it increments the size of the account pool.
 * 
 * @param acc The account to be added.
 * @return Returns 0 if the account was successfully added, else an error code:
 *         - 0x10: The account already exists in the pool.
 *         - 0x1F: An unknown error occurred.
 */
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

/**
 * @brief Removes an account from the account pool.
 * 
 * This function removes the account with the specified username from the account pool.
 * If the account is successfully removed, it returns 0. If the account does not exist in the account pool, it returns 0x20.
 * If an exception occurs during the removal process, it returns 0x2F.
 * 
 * @param username The username of the account to be removed.
 * @return Returns 0 if the account is successfully removed, else an error code:
 *             - 0x20: The account does not exist in the pool.
 *             - 0x2F: An unknown error occurred.
 */
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

/**
 * @brief Updates an account in the account pool.
 * 
 * This function removes the original account from the account pool and adds the new account to the pool.
 * 
 * @param original_acc The original account to be updated.
 * @param new_acc The new account to replace the original account.
 * @return Returns 0 if the update is successful, else an error code:
 *         - 0x30: The original account does not exist in the pool.
 *         - 0x31: The new account could not be added to the pool.
 *         - 0x3F: An unknown error occurred.
 */
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

/**
 * Retrieves the account associated with the given username from the account pool.
 * If the account is not found, returns a null account.
 *
 * @param username The username of the account to retrieve.
 * @return The account associated with the given username, or a null account if not found.
 */
Account AccountPool::getAccount(const std::string& username) const {
    if (accountpool.find(username) == accountpool.end()) return Account::NULL_ACCOUNT;
    return accountpool.at(username);
}

/**
 * Verifies the account with the given username and password hash.
 * 
 * @param username The username of the account to verify.
 * @param passwd_hash The password hash of the account to verify.
 * @return The result of the account verification.
 *         - ACCOUNT_NOT_FOUND if the account with the given username is not found.
 *         - WRONG_PASSWORD if the password hash does not match the account's password hash.
 *         - SUCCESS if the account is successfully verified.
 */
AccountPool::AccountVerifyResult AccountPool::verifyAccount(const std::string& username, const std::string& passwd_hash) const {
    if (accountpool.find(username) == accountpool.end()) return AccountVerifyResult::ACCOUNT_NOT_FOUND;
    if (accountpool.at(username).getPasswdHash() != passwd_hash) return AccountVerifyResult::WRONG_PASSWORD;
    return AccountVerifyResult::SUCCESS;
}

/**
 * Retrieves the account type associated with the given username.
 *
 * @param username The username of the account.
 * @return The account type associated with the username. If the username is not found in the account pool, returns Account::AccountType::NONETYPE.
 */
Account::AccountType AccountPool::getAccountType(const std::string& username) const {
    if (accountpool.find(username) == accountpool.end()) return Account::AccountType::NONETYPE;
    return accountpool.at(username).getAccountType();
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

/**
 * @brief Loads account data from an input stream.
 * 
 * This function reads account data from the specified input stream and populates the AccountPool object with the loaded accounts.
 * 
 * @param is The input stream to read from.
 * @return Returns 0 if the accounts are loaded successfully, else an error code:
 *         - 0x50: The input stream is invalid.
 *         - 0x51: An error occurred while clearing the account pool.
 *         - 0x52: An error occurred while adding an account to the pool.
 *         - 0x5F: An unknown error occurred.
 */
int AccountPool::load(std::istream& is) {
    if (!is) return 0x50;
    try {
        if (clear() != 0) return 0x51;
        // Read size
        size_t sz_tmp = 0;
        json load_json_obj = json::parse(is);
        
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
            if (addAccount(acc)) return 0x52;
        }
        return 0;
    }
    catch (...) {
        return 0x5F;
    }
}

/**
 * Saves the AccountPool to an output stream.
 * 
 * @param os The output stream to save the AccountPool to.
 * @return Returns 0 if the AccountPool is successfully saved, else an error code:
 *         - 0x60: The output stream is invalid.
 *         - 0x6F: An unknown error occurred.
 */
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

/**
 * Retrieves a list of all accounts in the account pool.
 *
 * @return A vector containing all the accounts in the account pool.
 */
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
