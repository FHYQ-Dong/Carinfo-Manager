#include "CarPool.hpp"
#include "Hash.hpp"
#include "AccountPool.hpp"
#include <iostream>
using namespace std;

int main() {
    AccountPool accountpool;
    Hash hash1("name1", "salt1"), hash2("name2", "salt2"), hash3("name3", "salt3");
    Account acc1(1, "name1", hash1.getHash(), Account::AccountType::ADMIN);
    Account acc2(2, "name2", hash2.getHash(), Account::AccountType::USER);
    Account acc3(3, "name3", hash3.getHash(), Account::AccountType::USER);
    accountpool.addAccount(acc1);
    accountpool.addAccount(acc2);
    accountpool.addAccount(acc3);
    cout << accountpool.size() << endl;
    vector<Account> accs1 = accountpool.list();
    for (auto acc : accs1) {
        cout << acc.getAccountId() << " " << acc.getUsername() << " " << acc.getPasswdHash() << " " << (int)acc.getAccountType() << endl;
    }
    cout << "save: " << accountpool.save("accountpool.dat") << endl;

    AccountPool accountpool2;
    cout << "load: " << accountpool2.load("accountpool.dat") << endl;
    cout << accountpool2.size() << endl;
    auto accs2 = accountpool2.list();
    for (auto acc : accs2) {
        cout << acc.getAccountId() << " " << acc.getUsername() << " " << acc.getPasswdHash() << " " << (int)acc.getAccountType() << endl;
    }
    return 0;
}
