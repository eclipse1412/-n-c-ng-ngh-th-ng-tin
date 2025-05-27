#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <functional>

struct Transaction {
    std::string from_wallet_id;
    std::string to_wallet_id;
    int amount;
    std::string timestamp;
};

class Wallet {
public:
    std::string wallet_id;
    int balance;
    std::vector<Transaction> history;

    Wallet(const std::string& id) : wallet_id(id), balance(0) {}

    void printBalance() const {
        std::cout << "[Wallet: " << wallet_id << "] So du: " << balance << " diem\n";
    }

    void printHistory() const {
        std::cout << "--- Lich su giao dich ---\n";
        for (const auto& t : history) {
            std::cout << t.timestamp << " | Tu: " << t.from_wallet_id << " -> " << t.to_wallet_id << " | So diem: " << t.amount << "\n";
        }
    }
};

std::string hashPassword(const std::string& password) {
    std::hash<std::string> hasher;
    size_t hashed = hasher(password);
    std::stringstream ss;
    ss << hashed;
    return ss.str();
}

std::string generateOTP() {
    srand(time(0));
    std::string otp = std::to_string(100000 + rand() % 900000);
    return otp;
}

bool verifyOTP(const std::string& expected) {
    std::string input;
    std::cout << "Nhap OTP: ";
    std::cin >> input;
    return input == expected;
}

std::string getCurrentTime() {
    time_t now = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", localtime(&now));
    return std::string(buf);
}

class Account {
public:
    std::string username;
    std::string password_hash;
    std::string role;
    bool has_temporary_password = false;
    Wallet wallet;

    Account(const std::string& user, const std::string& pass_hash, const std::string& r)
        : username(user), password_hash(pass_hash), role(r), wallet(user + "_wallet") {}
};

void saveAccountsToFile(const std::vector<Account>& accounts, const std::string& filename = "data.txt") {
    std::ofstream fout(filename);
    for (const auto& acc : accounts) {
        fout << acc.username << "," << acc.password_hash << "," << acc.role << "," << acc.wallet.balance << "," << acc.has_temporary_password << "\n";
    }
    fout.close();

    std::ifstream src(filename, std::ios::binary);
    std::ofstream dst("backup_" + filename, std::ios::binary);
    dst << src.rdbuf();
}

std::vector<Account> loadAccountsFromFile(const std::string& filename = "data.txt") {
    std::vector<Account> accounts;
    std::ifstream fin(filename);
    std::string line;
    while (getline(fin, line)) {
        std::stringstream ss(line);
        std::string username, hash, role, balance_str, temp_flag;
        getline(ss, username, ',');
        getline(ss, hash, ',');
        getline(ss, role, ',');
        getline(ss, balance_str, ',');
        getline(ss, temp_flag);

        Account acc(username, hash, role);
        acc.wallet.balance = std::stoi(balance_str);
        acc.has_temporary_password = (temp_flag == "1" || temp_flag == "true");
        accounts.push_back(acc);
    }
    return accounts;
}

Account* login(std::vector<Account>& accounts) {
    std::string user, pass;
    std::cout << "Ten dang nhap: "; std::cin >> user;
    std::cout << "Mat khau: "; std::cin >> pass;
    std::string hash = hashPassword(pass);

    for (auto& acc : accounts) {
        if (acc.username == user && acc.password_hash == hash) {
            if (acc.has_temporary_password) {
                std::cout << "Ban dang dung mat khau tam. Hay doi mat khau moi!\nMoi: ";
                std::cin >> pass;
                acc.password_hash = hashPassword(pass);
                acc.has_temporary_password = false;
            }
            return &acc;
        }
    }
    std::cout << "Dang nhap that bai!\n";
    return nullptr;
}

void registerAccount(std::vector<Account>& accounts, const std::string& role = "user") {
    std::string user, pass;
    std::cout << "Ten tai khoan: "; std::cin >> user;
    for (const auto& acc : accounts) {
        if (acc.username == user) {
            std::cout << "Tai khoan da ton tai!\n";
            return;
        }
    }
    std::cout << "Mat khau (de trong de sinh tu dong): ";
    std::getline(std::cin >> std::ws, pass);

    if (pass.empty()) {
        pass = generateOTP();
        std::cout << "Mat khau tam thoi: " << pass << "\n";
    }

    std::string hash = hashPassword(pass);
    Account new_acc(user, hash, role);
    if (pass.length() == 6 && isdigit(pass[0])) new_acc.has_temporary_password = true;
    accounts.push_back(new_acc);
    std::cout << "Dang ky thanh cong!\n";
}

void transferPoints(Account& sender, std::vector<Account>& accounts) {
    std::string recipient;
    int amount;
    std::cout << "Nguoi nhan: "; std::cin >> recipient;
    std::cout << "So diem: "; std::cin >> amount;

    if (recipient == sender.username || amount <= 0 || sender.wallet.balance < amount) {
        std::cout << "Loi giao dich!\n";
        return;
    }

    Account* receiver = nullptr;
    for (auto& acc : accounts) {
        if (acc.username == recipient) {
            receiver = &acc;
            break;
        }
    }
    if (!receiver) {
        std::cout << "Nguoi nhan khong ton tai!\n";
        return;
    }

    std::string otp = generateOTP();
    std::cout << "Ma OTP: " << otp << "\n";
    if (!verifyOTP(otp)) {
        std::cout << "Sai OTP! Huy giao dich.\n";
        return;
    }

    sender.wallet.balance -= amount;
    receiver->wallet.balance += amount;
    std::string time = getCurrentTime();
    Transaction t = { sender.wallet.wallet_id, receiver->wallet.wallet_id, amount, time };
    sender.wallet.history.push_back(t);
    receiver->wallet.history.push_back(t);
    std::cout << "Chuyen thanh cong!\n";
}

void walletMenu(Account& user, std::vector<Account>& accounts) {
    int choice;
    do {
        std::cout << "\n--- MENU VI ---\n1. Xem so du\n2. Lich su giao dich\n3. Chuyen diem\n0. Quay lai\nChon: ";
        std::cin >> choice;
        switch (choice) {
            case 1: user.wallet.printBalance(); break;
            case 2: user.wallet.printHistory(); break;
            case 3: transferPoints(user, accounts); break;
        }
    } while (choice != 0);
}

int main() {
    std::vector<Account> accounts = loadAccountsFromFile();
    int choice;
    do {
        std::cout << "\n1. Dang nhap\n2. Dang ky\n0. Thoat\nChon: ";
        std::cin >> choice;
        if (choice == 1) {
            Account* user = login(accounts);
            if (user) walletMenu(*user, accounts);
        } else if (choice == 2) {
            registerAccount(accounts);
        }
    } while (choice != 0);

    saveAccountsToFile(accounts);
    std::cout << "Da luu va thoat.\n";
    return 0;
}
