/**
 * @file include/carinfo-manager/accountpool.hpp
 * @brief Declaration of class Account and class AccountPool
 * 
 * @details
 * This file contains the declaration of the Account class and the AccountPool class.
 * The Account class represents a user account with a username, password hash, and account type.
 * The AccountPool class manages a collection of user accounts and provides operations to add, remove, update, and verify accounts.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#pragma execution_character_set("utf-8")
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include "carinfo-manager/basicpool.hpp"

class Account {
  public:
	enum class AccountType { NONETYPE = 0, ADMIN = 1, USER = 2 };

  private:
	std::string username;
	std::string passwd_hash;
	AccountType account_type;

  public:
	Account();
	Account(const std::string &username,
			const std::string &passwd_hash,
			const AccountType &account_type);
	Account(const Account &acc);
	~Account();
	std::string getUsername() const;
	std::string getPasswdHash() const;
	AccountType getAccountType() const;
	bool isAdmin() const;
	bool isUser() const;
	void setUsername(const std::string &username);
	void setPasswdHash(const std::string &passwd_hash);
	void setAccountType(const AccountType &account_type);

	bool operator==(const Account &acc) const;
	bool operator!=(const Account &acc) const;
	bool operator<(const Account &acc) const;
	bool operator>(const Account &acc) const;
	bool operator<=(const Account &acc) const;
	bool operator>=(const Account &acc) const;
	Account &operator=(const Account &acc);
	const static Account NULL_ACCOUNT;
};

class AccountPool : public BasicPool {
  public:
	enum class AccountVerifyResult { SUCCESS = 0, ACCOUNT_NOT_FOUND = 1, WRONG_PASSWORD = 2 };

  private:
	std::map<std::string, Account> accountpool;
	std::map<std::string, Account> adminpool;
	std::map<std::string, Account> userpool;

  public:
	AccountPool();
	AccountPool(Account *begin, Account *end);
	AccountPool(const std::vector<Account> &accounts);
	AccountPool(const AccountPool &ap);
	~AccountPool();
	int addAccount(const Account &acc);
	int removeAccount(const std::string &username);
	int removeAccount(const Account &acc);
	int updateAccount(const Account &original_acc, const Account &new_acc);
	Account getAccount(const std::string &username) const;
	// 模糊查找
	AccountPool getAccountLike(const std::string &username) const;
	AccountVerifyResult verifyAccount(const std::string &username,
									  const std::string &passwd_hash) const;
	Account::AccountType getAccountType(const std::string &username) const;
	size_t size() const;
	bool empty() const;
	int clear();
	int load(std::istream &is);
	int save(std::ostream &os) const;
	std::vector<Account> list() const;

	bool operator==(const AccountPool &ap) const;
	bool operator!=(const AccountPool &ap) const;
	AccountPool &operator=(const AccountPool &ap);
};
