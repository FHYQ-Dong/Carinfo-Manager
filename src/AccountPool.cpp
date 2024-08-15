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
#include <assert.h>
#include <fstream>
#include "carinfo-manager/log.hpp"
#include "json/json.hpp"
using json = nlohmann::json;

Account::Account() {
	username = "";
	passwd_hash = "";
	account_type = AccountType::NONETYPE;
}

Account::Account(const std::string &username,
				 const std::string &passwd_hash,
				 const AccountType &account_type) {
	assert(passwd_hash.length() == 64);
	this->username = username;
	this->passwd_hash = passwd_hash;
	this->account_type = account_type;
}

Account::Account(const Account &acc) {
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

bool Account::isAdmin() const {
	return account_type == AccountType::ADMIN;
}

bool Account::isUser() const {
	return account_type == AccountType::USER;
}

void Account::setUsername(const std::string &username) {
	this->username = username;
}

void Account::setPasswdHash(const std::string &passwd_hash) {
	assert(passwd_hash.length() == 64);
	this->passwd_hash = passwd_hash;
}

void Account::setAccountType(const AccountType &account_type) {
	this->account_type = account_type;
}

bool Account::operator==(const Account &acc) const {
	return username == acc.username;
}

bool Account::operator!=(const Account &acc) const {
	return username != acc.username;
}

bool Account::operator<(const Account &acc) const {
	return username < acc.username;
}

bool Account::operator>(const Account &acc) const {
	return username > acc.username;
}

bool Account::operator<=(const Account &acc) const {
	return username <= acc.username;
}

bool Account::operator>=(const Account &acc) const {
	return username >= acc.username;
}

Account &Account::operator=(const Account &acc) {
	username = acc.username;
	passwd_hash = acc.passwd_hash;
	account_type = acc.account_type;
	return *this;
}

const Account Account::NULL_ACCOUNT = Account();

/**
 * @brief Default constructor for the AccountPool class.
 * 
 * This constructor initializes the account pool with an empty map of accounts and sets the size of the account pool to 0.
 */
AccountPool::AccountPool() {
	accountpool = std::map<std::string, Account>();
	adminpool = std::map<std::string, Account>();
	userpool = std::map<std::string, Account>();
	sz = 0;
}

/**
 * @brief Constructor for the AccountPool class that initializes the account pool with a range of accounts.
 * 
 * This constructor initializes the account pool with the accounts in the range [begin, end). It adds each account in the range to the account pool.
 * 
 * @param begin An iterator pointing to the beginning of the range of accounts.
 * @param end An iterator pointing to the end of the range of accounts.
 */
AccountPool::AccountPool(Account *begin, Account *end) {
	accountpool = std::map<std::string, Account>();
	adminpool = std::map<std::string, Account>();
	userpool = std::map<std::string, Account>();
	sz = 0;
	for (Account *it = begin; it != end; it++) {
		if (addAccount(*it) != 0)
			throw "Error: AccountPool::AccountPool(Account* begin, Account* end)";
	}
}

/**
 * @brief Constructor for the AccountPool class that initializes the account pool with a vector of accounts.
 * 
 * This constructor initializes the account pool with the accounts in the specified vector. It adds each account in the vector to the account pool.
 * 
 * @param accounts The vector of accounts to initialize the account pool with.
 */
AccountPool::AccountPool(const std::vector<Account> &accounts) {
	accountpool = std::map<std::string, Account>();
	adminpool = std::map<std::string, Account>();
	userpool = std::map<std::string, Account>();
	sz = 0;
	for (Account acc : accounts) {
		if (addAccount(acc) != 0)
			throw "Error: AccountPool::AccountPool(std::vector<Account> accounts)";
	}
}

/**
 * @brief Copy constructor for the AccountPool class.
 * 
 * This constructor initializes the account pool with the accounts in the specified AccountPool object.
 * 
 * @param ap The AccountPool object to copy.
 */
AccountPool::AccountPool(const AccountPool &ap) {
	accountpool = ap.accountpool;
	adminpool = ap.adminpool;
	userpool = ap.userpool;
	sz = ap.sz;
}

/**
 * @brief Destructor for the AccountPool class.
 * 
 * This destructor clears the account pool and frees the memory used by the account pool.
 */
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
int AccountPool::addAccount(const Account &acc) {
	try {
		if (accountpool.find(acc.getUsername()) != accountpool.end()) {
			MyLogger::log("carinfo-manager-logger",
						  MyLogger::LOG_LEVEL::DEBUG,
						  "[AccountPool Add Account] \n- Username: " + acc.getUsername() +
							  "\n- PasswdHash: " + acc.getPasswdHash() + "\n- AccountType: " +
							  std::to_string((int)acc.getAccountType()) + "\n- Stuatus: 0x10");
			return 0x10;
		}
		accountpool[acc.getUsername()] = acc;
		if (acc.getAccountType() == Account::AccountType::ADMIN) {
			adminpool[acc.getUsername()] = acc;
		}
		else if (acc.getAccountType() == Account::AccountType::USER) {
			userpool[acc.getUsername()] = acc;
		}
		sz++;
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[AccountPool Add Account] \n- Username: " + acc.getUsername() +
						  "\n- PasswdHash: " + acc.getPasswdHash() + "\n- AccountType: " +
						  std::to_string((int)acc.getAccountType()) + "\n- Stuatus: 0");
		return 0;
	}
	catch (...) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[AccountPool Add Account] \n- Username: " + acc.getUsername() +
						  "\n- PasswdHash: " + acc.getPasswdHash() + "\n- AccountType: " +
						  std::to_string((int)acc.getAccountType()) + "\n- Stuatus: 0x1F");
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
 *         - 0x20: The account does not exist in the pool.
 *         - 0x2F: An unknown error occurred.
 */
int AccountPool::removeAccount(const std::string &username) {
	try {
		if (accountpool.find(username) == accountpool.end()) {
			MyLogger::log("carinfo-manager-logger",
						  MyLogger::LOG_LEVEL::DEBUG,
						  "[AccountPool Remove Account] " + username + ". Stuatus: 0x20");
			return 0x20;
		}

		Account acc = accountpool[username];
		accountpool.erase(username);
		if (acc.getAccountType() == Account::AccountType::ADMIN) {
			adminpool.erase(username);
		}
		else if (acc.getAccountType() == Account::AccountType::USER) {
			userpool.erase(username);
		}
		sz--;
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[AccountPool Remove Account] " + username + ". Stuatus: 0");
		return 0;
	}
	catch (...) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[AccountPool Remove Account] " + username + ". Stuatus: 0x2F");
		return 0x2F;
	}
}

/**
 * @brief Removes an account from the account pool.
 * 
 * This function removes the account with the specified username from the account pool.
 * If the account is successfully removed, it returns 0. If the account does not exist in the account pool, it returns 0x30.
 * If an exception occurs during the removal process, it returns 0x3F.
 * 
 * @param acc The account to be removed.
 * @return Returns 0 if the account is successfully removed, else an error code:
 *         - 0x30: The account does not exist in the pool.
 *         - 0x3F: An unknown error occurred.
 */
int AccountPool::removeAccount(const Account &acc) {
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
int AccountPool::updateAccount(const Account &original_acc, const Account &new_acc) {
	try {
		if (removeAccount(original_acc) != 0) {
			MyLogger::log(
				"carinfo-manager-logger",
				MyLogger::LOG_LEVEL::DEBUG,
				"[AccountPool Update Account] \n- Original Username: " +
					original_acc.getUsername() + "\n- New Username: " + new_acc.getUsername() +
					"\n- New PasswdHash: " + new_acc.getPasswdHash() + "\n- New AccountType: " +
					std::to_string((int)new_acc.getAccountType()) + "\n- Stuatus: 0x30");
			return 0x30;
		}
		if (addAccount(new_acc) != 0) {
			MyLogger::log(
				"carinfo-manager-logger",
				MyLogger::LOG_LEVEL::DEBUG,
				"[AccountPool Update Account] \n- Original Username: " +
					original_acc.getUsername() + "\n- New Username: " + new_acc.getUsername() +
					"\n- New PasswdHash: " + new_acc.getPasswdHash() + "\n- New AccountType: " +
					std::to_string((int)new_acc.getAccountType()) + "\n- Stuatus: 0x31");
			return 0x31;
		}
		MyLogger::log(
			"carinfo-manager-logger",
			MyLogger::LOG_LEVEL::DEBUG,
			"[AccountPool Update Account] \n- Original Username: " + original_acc.getUsername() +
				"\n- New Username: " + new_acc.getUsername() +
				"\n- New PasswdHash: " + new_acc.getPasswdHash() + "\n- New AccountType: " +
				std::to_string((int)new_acc.getAccountType()) + "\n- Stuatus: 0");
		return 0;
	}
	catch (...) {
		MyLogger::log(
			"carinfo-manager-logger",
			MyLogger::LOG_LEVEL::ERROR,
			"[AccountPool Update Account] \n- Original Username: " + original_acc.getUsername() +
				"\n- New Username: " + new_acc.getUsername() +
				"\n- New PasswdHash: " + new_acc.getPasswdHash() + "\n- New AccountType: " +
				std::to_string((int)new_acc.getAccountType()) + "\n- Stuatus: 0x3F");
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
Account AccountPool::getAccount(const std::string &username) const {
	if (accountpool.find(username) == accountpool.end())
		return Account::NULL_ACCOUNT;
	MyLogger::log(
		"carinfo-manager-logger",
		MyLogger::LOG_LEVEL::DEBUG,
		"[AccountPool Get Account] \n- Username: " + username +
			"\n- PasswdHash: " + accountpool.at(username).getPasswdHash() +
			"\n- AccountType: " + std::to_string((int)accountpool.at(username).getAccountType()));
	return accountpool.at(username);
}

/**
 * Retrieves a list of accounts that match the given username.
 * 
 * This function searches for accounts in the account pool whose usernames contain the given username as a substring.
 * 
 * @param username The username to search for.
 * @return A vector containing all accounts whose usernames contain the given username as a substring.
 * 	   If no accounts are found, returns an empty vector.
 */
AccountPool AccountPool::getAccountLike(const std::string &username) const {
	AccountPool ap;
	for (auto it = accountpool.begin(); it != accountpool.end(); it++) {
		if (it->first.find(username) != std::string::npos)
			ap.addAccount(it->second);
	}
	std::stringstream ss;
	ap.save(ss);
	MyLogger::log("carinfo-manager-logger",
				  MyLogger::LOG_LEVEL::DEBUG,
				  "[AccountPool Get Account Like] \n- Username: " + username +
					  "\n- AccountPool: " + ss.str());
	return ap;
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
AccountPool::AccountVerifyResult AccountPool::verifyAccount(const std::string &username,
															const std::string &passwd_hash) const {
	if (accountpool.find(username) == accountpool.end()) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[AccountPool Verify Account] \n- Username: " + username +
						  "\n- PasswdHash: " + passwd_hash + "\n- Stuatus: 1");
		return AccountVerifyResult::ACCOUNT_NOT_FOUND;
	}
	if (accountpool.at(username).getPasswdHash() != passwd_hash) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[AccountPool Verify Account] \n- Username: " + username +
						  "\n- PasswdHash: " + passwd_hash + "\n- Stuatus: 2");
		return AccountVerifyResult::WRONG_PASSWORD;
	}
	MyLogger::log("carinfo-manager-logger",
				  MyLogger::LOG_LEVEL::DEBUG,
				  "[AccountPool Verify Account] \n- Username: " + username +
					  "\n- PasswdHash: " + passwd_hash + "\n- Stuatus: 0");
	return AccountVerifyResult::SUCCESS;
}

/**
 * Retrieves the account type associated with the given username.
 *
 * @param username The username of the account.
 * @return The account type associated with the username. If the username is not found in the account pool, returns Account::AccountType::NONETYPE.
 */
Account::AccountType AccountPool::getAccountType(const std::string &username) const {
	if (accountpool.find(username) == accountpool.end()) {
		MyLogger::log(
			"carinfo-manager-logger",
			MyLogger::LOG_LEVEL::DEBUG,
			"[AccountPool Get Account Type] \n- Username: " + username + "\n- Stuatus: 1");
		return Account::AccountType::NONETYPE;
	}
	MyLogger::log("carinfo-manager-logger",
				  MyLogger::LOG_LEVEL::DEBUG,
				  "[AccountPool Get Account Type] \n- Username: " + username + "\n- AccountType: " +
					  std::to_string((int)accountpool.at(username).getAccountType()) +
					  "\n- Stuatus: 0");
	return accountpool.at(username).getAccountType();
}

/**
 * Retrieves the size of the account pool.
 *
 * @return The size of the account pool.
 */
size_t AccountPool::size() const {
	MyLogger::log("carinfo-manager-logger",
				  MyLogger::LOG_LEVEL::DEBUG,
				  "[AccountPool Size] \n- Size: " + std::to_string(sz));
	return sz;
}

/**
 * Checks if the account pool is empty.
 *
 * @return True if the account pool is empty, false otherwise.
 */
bool AccountPool::empty() const {
	MyLogger::log("carinfo-manager-logger",
				  MyLogger::LOG_LEVEL::DEBUG,
				  "[AccountPool Empty] \n- Stuatus: " + std::to_string(sz == 0));
	return sz == 0;
}

/**
 * @brief Clears the account pool.
 * 
 * This function clears the account pool by removing all accounts from the account pool and setting the size of the account pool to 0.
 * 
 * @return Returns 0 if the account pool is successfully cleared, else an error code:
 *         - 0x4F: An unknown error occurred.
 */
int AccountPool::clear() {
	try {
		accountpool.clear();
		adminpool.clear();
		userpool.clear();
		sz = 0;
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[AccountPool Clear] \n- Stuatus: 0");
		return 0;
	}
	catch (...) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[AccountPool Clear] \n- Stuatus: 0x4F");
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
 *         - 0x52: The loaded data is not in the correct format.
 *         - 0x53: The loaded data is missing required fields.
 *         - 0x54: The loaded data has incorrect field types.
 *         - 0x55: An error occurred while adding an account to the account pool.
 *         - 0x5F: An unknown error occurred.
 */
int AccountPool::load(std::istream &is) {
	if (!is) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[AccountPool Load] \n- Stuatus: 0x50");
		return 0x50;
	}
	try {
		if (clear() != 0) {
			MyLogger::log("carinfo-manager-logger",
						  MyLogger::LOG_LEVEL::ERROR,
						  "[AccountPool Load] \n- Stuatus: 0x51");
			return 0x51;
		}
		json load_json_obj = json::parse(is);
		for (json::iterator it = load_json_obj.begin(); it != load_json_obj.end(); it++) {
			if (!it.value().is_object()) {
				MyLogger::log("carinfo-manager-logger",
							  MyLogger::LOG_LEVEL::ERROR,
							  "[AccountPool Load] \n- Stuatus: 0x52");
				return 0x52;
			}
			if (!it.value().contains("username") || !it.value().contains("passwd_hash") ||
				!it.value().contains("account_type")) {
				MyLogger::log("carinfo-manager-logger",
							  MyLogger::LOG_LEVEL::ERROR,
							  "[AccountPool Load] \n- Stuatus: 0x53");
				return 0x53;
			}
			if (!it.value()["username"].is_string() || !it.value()["passwd_hash"].is_string() ||
				!it.value()["account_type"].is_number_integer()) {
				MyLogger::log("carinfo-manager-logger",
							  MyLogger::LOG_LEVEL::ERROR,
							  "[AccountPool Load] \n- Stuatus: 0x54");
				return 0x54;
			}
			Account acc(std::string(it.key()),
						std::string(it.value()["passwd_hash"]),
						(Account::AccountType)(int)(it.value()["account_type"]));
			if (addAccount(acc) != 0) {
				MyLogger::log("carinfo-manager-logger",
							  MyLogger::LOG_LEVEL::ERROR,
							  "[AccountPool Load] \n- Stuatus: 0x55");
				return 0x55;
			}
		}
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[AccountPool Load] \n- Stuatus: 0");
		return 0;
	}
	catch (...) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[AccountPool Load] \n- Stuatus: 0x5F");
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
int AccountPool::save(std::ostream &os) const {
	if (!os) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[AccountPool Save] \n- Stuatus: 0x60");
		return 0x60;
	}
	try {
		// Write size
		json save_json_obj;
		for (auto it = accountpool.begin(); it != accountpool.end(); it++) {
			json acc_json_obj;
			acc_json_obj["username"] = it->second.getUsername();
			acc_json_obj["passwd_hash"] = it->second.getPasswdHash();
			acc_json_obj["account_type"] = (int)it->second.getAccountType();
			save_json_obj[it->first] = acc_json_obj;
		}
		os << save_json_obj.dump(4);
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[AccountPool Save] \n- Stuatus: 0");
		return 0;
	}
	catch (...) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[AccountPool Save] \n- Stuatus: 0x6F");
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
	MyLogger::log(
		"carinfo-manager-logger", MyLogger::LOG_LEVEL::DEBUG, "[AccountPool List] \n- Stuatus: 0");
	return accounts;
}

/**
 * @brief Equality operator for the AccountPool class.
 * 
 * This operator checks if two AccountPool objects are equal by comparing their size, account pools, admin pools, and user pools.
 * 
 * @param ap The AccountPool object to compare with.
 * @return True if the AccountPool objects are equal, false otherwise.
 */
bool AccountPool::operator==(const AccountPool &ap) const {
	return sz == ap.sz && accountpool == ap.accountpool && adminpool == ap.adminpool &&
		   userpool == ap.userpool;
}

/**
 * @brief Inequality operator for the AccountPool class.
 * 
 * This operator checks if two AccountPool objects are not equal by comparing their size, account pools, admin pools, and user pools.
 * 
 * @param ap The AccountPool object to compare with.
 * @return True if the AccountPool objects are not equal, false otherwise.
 */
bool AccountPool::operator!=(const AccountPool &ap) const {
	return sz != ap.sz || accountpool != ap.accountpool || adminpool != ap.adminpool ||
		   userpool != ap.userpool;
}

/**
 * @brief Less than operator for the AccountPool class.
 * 
 * This operator compares two AccountPool objects by comparing their size.
 * 
 * @param ap The AccountPool object to compare with.
 * @return True if the size of the AccountPool object is less than the size of the other AccountPool object, false otherwise.
 */
AccountPool &AccountPool::operator=(const AccountPool &ap) {
	sz = ap.sz;
	accountpool = ap.accountpool;
	adminpool = ap.adminpool;
	userpool = ap.userpool;
	return *this;
}
