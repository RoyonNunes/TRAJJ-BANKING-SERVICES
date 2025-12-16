#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdexcept>

using namespace std;

/* =========================
   TRANSACTION CLASS
   ========================= */
class Transaction {
private:
    double amount;
    string type;

public:
    // Parameterized constructor
    Transaction(double amt, const string& t) : amount(amt), type(t) {}

    // Build transaction report string
    string report() const {
        stringstream ss;
        ss << fixed << setprecision(2);
        ss << "Transaction Type: " << type << " | Amount: $" << amount;
        return ss.str();
    }
};

/* =========================
   ACCOUNT BASE CLASS
   ========================= */
class Account {
protected:
    double balance;
    vector<Transaction> log;

public:
    Account(double initialBalance) {
        if (initialBalance >= 1000.00)
            balance = initialBalance;
        else
            throw invalid_argument("Initial balance must be at least $1000.00");
    }

    virtual ~Account() = default;

    virtual void Deposit(double amount) {
        try {
            if (amount <= 0)
                throw invalid_argument("Invalid deposit amount");

            balance += amount;
            log.emplace_back(amount, "Deposit");

            cout << "Deposited: $" << fixed << setprecision(2) << amount << endl;
        }
        catch (exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    virtual void Withdraw(double amount) {
        try {
            if (amount <= 0)
                throw invalid_argument("Invalid withdrawal amount");

            if (amount > balance)
                throw runtime_error("Insufficient funds");

            balance -= amount;
            log.emplace_back(amount, "Withdrawal");

            cout << "Withdrew: $" << fixed << setprecision(2) << amount << endl;
        }
        catch (exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    double GetBalance() const {
        return balance;
    }

    // Account transaction report
    string report() const {
        stringstream ss;
        ss << "\n=== TRANSACTION HISTORY ===\n";

        if (log.empty()) {
            ss << "No transactions recorded.\n";
        } else {
            for (const auto& t : log)
                ss << t.report() << endl;
        }

        ss << "===========================\n";
        return ss.str();
    }
};

/* =========================
   SAVINGS ACCOUNT
   ========================= */
class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(double initialBalance, double rate)
        : Account(initialBalance) {
        interestRate = (rate >= 0) ? rate : 0;
    }

    double CalculateInterest() const {
        return balance * (interestRate / 100.0);
    }

    void AddInterest() {
        double interest = CalculateInterest();
        balance += interest;
        log.emplace_back(interest, "Interest Added");

        cout << "Interest added: $" << fixed << setprecision(2) << interest << endl;
    }

    double GetInterestRate() const {
        return interestRate;
    }
};

/* =========================
   CHEQUING ACCOUNT
   ========================= */
class ChequingAccount : public Account {
private:
    double transactionFee;

public:
    ChequingAccount(double initialBalance, double fee)
        : Account(initialBalance), transactionFee(fee) {}

    void Deposit(double amount) override {
        try {
            if (amount <= transactionFee)
                throw invalid_argument("Deposit must exceed transaction fee");

            balance += (amount - transactionFee);
            log.emplace_back(amount, "Chequing Deposit");
            log.emplace_back(transactionFee, "Transaction Fee");

            cout << "Deposited: $" << fixed << setprecision(2) << amount << endl;
            cout << "Fee Charged: $" << transactionFee << endl;
        }
        catch (exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void Withdraw(double amount) override {
        try {
            double total = amount + transactionFee;

            if (amount <= 0)
                throw invalid_argument("Invalid withdrawal amount");

            if (total > balance)
                throw runtime_error("Amount + fee exceeds balance");

            balance -= total;
            log.emplace_back(amount, "Chequing Withdrawal");
            log.emplace_back(transactionFee, "Transaction Fee");

            cout << "Withdrew: $" << fixed << setprecision(2) << amount << endl;
            cout << "Fee Charged: $" << transactionFee << endl;
        }
        catch (exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    double GetTransactionFee() const {
        return transactionFee;
    }
};

/* =========================
   MAIN PROGRAM
   ========================= */
int main() {
    try {
        double initialBalance, interestRate, transactionFee;

        cout << "Welcome to TRAJJ Banking Services\n";

        do {
            cout << "Enter Initial Balance (minimum $1000.00): $";
            cin >> initialBalance;
        } while (initialBalance < 1000.00);

        cout << "Enter Savings Interest Rate (%): ";
        cin >> interestRate;

        cout << "Enter Chequing Transaction Fee: $";
        cin >> transactionFee;

        SavingsAccount savings(initialBalance, interestRate);
        ChequingAccount chequing(initialBalance, transactionFee);

        int choice;
        double amount;

        while (true) {
            cout << "\n1. Savings Deposit\n2. Savings Withdraw\n3. Add Interest\n"
                 << "4. Chequing Deposit\n5. Chequing Withdraw\n"
                 << "6. View Reports\n7. Save & Exit\nChoice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Amount: $"; cin >> amount;
                    savings.Deposit(amount);
                    break;

                case 2:
                    cout << "Amount: $"; cin >> amount;
                    savings.Withdraw(amount);
                    break;

                case 3:
                    savings.AddInterest();
                    break;

                case 4:
                    cout << "Amount: $"; cin >> amount;
                    chequing.Deposit(amount);
                    break;

                case 5:
                    cout << "Amount: $"; cin >> amount;
                    chequing.Withdraw(amount);
                    break;

                case 6:
                    cout << savings.report();
                    cout << chequing.report();
                    break;

                case 7:
                    try {
                        ofstream file("C:\\transactions.txt");
                        if (!file)
                            throw runtime_error("Unable to open file");

                        file << savings.report();
                        file << chequing.report();
                        file.close();

                        cout << "Transactions saved to C:\\transactions.txt\n";
                    }
                    catch (exception& e) {
                        cout << "File Error: " << e.what() << endl;
                    }
                    return 0;

                default:
                    cout << "Invalid option.\n";
            }
        }
    }
    catch (exception& e) {
        cout << "Fatal Error: " << e.what() << endl;
    }

    return 0;
}
