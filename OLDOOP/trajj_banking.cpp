#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <chrono>

using namespace std;

// Forward declaration
class Transaction;

// Transaction Class
class Transaction {
private:
    double amount;
    string type; // "DEPOSIT", "WITHDRAWAL", "INTEREST", "FEE", "BALANCE_INQUIRY"
    string timestamp;
    string accountType; // "SAVINGS" or "CHEQUING"

public:
    // Parameterized constructor
    Transaction(double amt, const string& t, const string& accType) 
        : amount(amt), type(t), accountType(accType) {
        // Generate timestamp
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        char timeStr[100];
        ctime_s(timeStr, sizeof(timeStr), &now_time);
        timestamp = string(timeStr);
        timestamp = timestamp.substr(0, timestamp.length() - 1); // Remove newline
    }

    // Getters
    double getAmount() const { return amount; }
    string getType() const { return type; }
    string getTimestamp() const { return timestamp; }
    string getAccountType() const { return accountType; }

    // report() function as required
    string report() const {
        stringstream ss;
        ss << "[" << timestamp << "] ";
        ss << accountType << " Account - ";
        ss << type << ": ";
        
        if (type == "BALANCE_INQUIRY") {
            ss << "Balance checked";
        } else {
            ss << "$" << fixed << setprecision(2) << amount;
            
            if (type == "FEE") {
                ss << " (Transaction Fee)";
            } else if (type == "INTEREST") {
                ss << " (Interest Added)";
            }
        }
        
        return ss.str();
    }
};

class Account {
protected:
    double balance;
    vector<Transaction> log; // Transaction log as required
    string accountNumber;

    // Helper method to add transaction to log
    void addToLog(const Transaction& transaction) {
        log.push_back(transaction);
    }

public:
    // Constructor with validation for initial balance
    Account(double initialBalance, const string& accNum = "") 
        : accountNumber(accNum.empty() ? generateAccountNumber() : accNum) {
        try {
            if (initialBalance >= 1000.00) {
                balance = initialBalance;
                // Log initial deposit
                addToLog(Transaction(initialBalance, "INITIAL_DEPOSIT", getAccountType()));
            } else {
                throw invalid_argument("The Initial Balance Must Be At Least $1000.00");
            }
        } catch (const invalid_argument& e) {
            balance = 0.0;
            cout << "Warning: " << e.what() << ". Account Balance Set To $0.00" << endl;
            // Log failed initial deposit
            addToLog(Transaction(0.0, "FAILED_INITIAL_DEPOSIT", getAccountType()));
        }
    }

    // Virtual destructor for proper inheritance 
    virtual ~Account() = default;

    // Deposit money into account to increase balance 
    virtual void Deposit(double amount) {
        try {
            if (amount <= 0) {
                throw invalid_argument("Invalid Deposit Amount!");
            }
            
            balance += amount;
            addToLog(Transaction(amount, "DEPOSIT", getAccountType()));
            cout << "Successfully Deposited: $" << fixed << setprecision(2) << amount << endl;
            
        } catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;
            addToLog(Transaction(amount, "FAILED_DEPOSIT", getAccountType()));
        }
    }

    // Withdraw money from account with balance check 
    virtual void Withdraw(double amount) {
        try {
            if (amount <= 0) {
                throw invalid_argument("Invalid Withdrawal Amount!");
            }
            
            if (amount <= balance) {
                balance -= amount;
                addToLog(Transaction(amount, "WITHDRAWAL", getAccountType()));
                cout << "Successfully Withdrew: $" << fixed << setprecision(2) << amount << endl;
            } else {
                throw runtime_error("Debit Amount Exceeded Account Balance");
            }
            
        } catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;
            addToLog(Transaction(amount, "FAILED_WITHDRAWAL", getAccountType()));
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << ". Withdrawal Failed." << endl;
            addToLog(Transaction(amount, "FAILED_WITHDRAWAL", getAccountType()));
        }
    }

    // Get current balance of the account
    double GetBalance() const {
        // Log balance inquiry
        const_cast<Account*>(this)->addToLog(Transaction(balance, "BALANCE_INQUIRY", getAccountType()));
        return balance;
    }

    // Virtual function to get account type (to be overridden by derived classes)
    virtual string getAccountType() const = 0;

    // report() function as required - formats transaction information
    virtual void report() const {
        cout << "\n=== TRANSACTION REPORT ===" << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Type: " << getAccountType() << endl;
        cout << "Current Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "\nTransaction History:" << endl;
        cout << "--------------------" << endl;
        
        if (log.empty()) {
            cout << "No transactions recorded." << endl;
        } else {
            for (const auto& transaction : log) {
                cout << transaction.report() << endl;
            }
        }
        cout << "=====================" << endl;
    }

    // Method to save report to file
    bool saveReportToFile(const string& filename = "C:\\transactions.txt") const {
        try {
            ofstream outFile(filename);
            
            if (!outFile.is_open()) {
                throw runtime_error("Unable to open file for writing: " + filename);
            }
            
            outFile << "=== TRAJJ BANKING SERVICES - TRANSACTION REPORT ===" << endl;
            outFile << "Generated: " << getCurrentTimestamp() << endl;
            outFile << "Account Number: " << accountNumber << endl;
            outFile << "Account Type: " << getAccountType() << endl;
            outFile << "Current Balance: $" << fixed << setprecision(2) << balance << endl;
            outFile << "\nTransaction History:" << endl;
            outFile << "--------------------" << endl;
            
            if (log.empty()) {
                outFile << "No transactions recorded." << endl;
            } else {
                for (const auto& transaction : log) {
                    outFile << transaction.report() << endl;
                }
            }
            
            outFile << "===================================================" << endl;
            outFile.close();
            
            cout << "\nTransaction report successfully saved to: " << filename << endl;
            return true;
            
        } catch (const exception& e) {
            cout << "Error saving report to file: " << e.what() << endl;
            return false;
        }
    }

protected:
    // Helper method to generate account number
    string generateAccountNumber() {
        static int accountCounter = 1000;
        return "ACC" + to_string(accountCounter++);
    }

    // Helper method to get current timestamp
    string getCurrentTimestamp() const {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        char timeStr[100];
        ctime_s(timeStr, sizeof(timeStr), &now_time);
        string timestamp = string(timeStr);
        return timestamp.substr(0, timestamp.length() - 1);
    }
};

// Derived Class SavingsAccount
class SavingsAccount : public Account {
private:
    double interestRate; // as percentage (e.g., 2.5 for 2.5%)

public:
    // Constructor inheriting from Account
    SavingsAccount(double initialBalance, double rate, const string& accNum = "") 
        : Account(initialBalance, accNum), interestRate(rate) {
    }

    // Calculate interest earned
    double CalculateInterest() const {
        return balance * (interestRate / 100.0);
    }

    // Add interest to the account
    void AddInterest() {
        try {
            double interest = CalculateInterest();
            if (interest > 0) {
                balance += interest;
                addToLog(Transaction(interest, "INTEREST", getAccountType()));
                cout << "Interest of $" << fixed << setprecision(2) << interest 
                     << " added to savings account. New balance: $" << balance << endl;
            }
        } catch (const exception& e) {
            cout << "Error adding interest: " << e.what() << endl;
            addToLog(Transaction(0.0, "FAILED_INTEREST", getAccountType()));
        }
    }

    // Get interest rate
    double GetInterestRate() const {
        return interestRate;
    }

    // Override getAccountType
    string getAccountType() const override {
        return "SAVINGS";
    }

    // Override report to include savings-specific info
    void report() const override {
        Account::report();
        cout << "Interest Rate: " << fixed << setprecision(2) << interestRate << "%" << endl;
        cout << "Available Interest: $" << fixed << setprecision(2) << CalculateInterest() << endl;
    }
};

// Derived Class ChequingAccount
class ChequingAccount : public Account {
private:
    double transactionFee;

public:
    // Constructor inheriting from Account
    ChequingAccount(double initialBalance, double fee, const string& accNum = "") 
        : Account(initialBalance, accNum), transactionFee(fee) {
    }

    // Override Withdraw to include transaction fee
    void Withdraw(double amount) override {
        try {
            if (amount <= 0) {
                throw invalid_argument("Invalid Withdrawal Amount!");
            }
            
            double totalAmount = amount + transactionFee;
            if (totalAmount <= balance) {
                balance -= totalAmount;
                // Log withdrawal
                addToLog(Transaction(amount, "WITHDRAWAL", getAccountType()));
                // Log fee
                addToLog(Transaction(transactionFee, "FEE", getAccountType()));
                
                cout << "Successfully Withdrew: $" << fixed << setprecision(2) << amount << endl;
                cout << "Transaction fee: $" << fixed << setprecision(2) << transactionFee << endl;
                cout << "Total deducted: $" << fixed << setprecision(2) << totalAmount << endl;
            } else {
                throw runtime_error("Debit Amount + Fee Exceeded Account Balance");
            }
            
        } catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;
            addToLog(Transaction(amount, "FAILED_WITHDRAWAL", getAccountType()));
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << ". Withdrawal Failed." << endl;
            addToLog(Transaction(amount, "FAILED_WITHDRAWAL", getAccountType()));
        }
    }

    // Override Deposit to include transaction fee
    void Deposit(double amount) override {
        try {
            if (amount <= 0) {
                throw invalid_argument("Invalid Deposit Amount!");
            }
            
            // Check if deposit covers the fee
            if (amount > transactionFee) {
                balance += (amount - transactionFee);
                // Log deposit
                addToLog(Transaction(amount, "DEPOSIT", getAccountType()));
                // Log fee
                addToLog(Transaction(transactionFee, "FEE", getAccountType()));
                
                cout << "Successfully Deposited: $" << fixed << setprecision(2) << amount << endl;
                cout << "Transaction fee charged: $" << fixed << setprecision(2) << transactionFee << endl;
                cout << "Net balance change: +$" << fixed << setprecision(2) << (amount - transactionFee) << endl;
            } else {
                throw runtime_error("Deposit amount must exceed transaction fee");
            }
            
        } catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;
            addToLog(Transaction(amount, "FAILED_DEPOSIT", getAccountType()));
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << ". Deposit failed." << endl;
            addToLog(Transaction(amount, "FAILED_DEPOSIT", getAccountType()));
        }
    }

    // Get transaction fee
    double GetTransactionFee() const {
        return transactionFee;
    }

    // Override getAccountType
    string getAccountType() const override {
        return "CHEQUING";
    }

    // Override report to include chequing-specific info
    void report() const override {
        Account::report();
        cout << "Transaction Fee: $" << fixed << setprecision(2) << transactionFee << " per transaction" << endl;
    }
};

// Function to display main menu options
void displayMainMenu() {
    cout << "\n=== TRAJJ BANKING SERVICES INTERFACE ===" << endl;
    cout << "1. Use Savings Account" << endl;
    cout << "2. Use Chequing Account" << endl;
    cout << "3. View Account Reports" << endl;
    cout << "4. Save Reports to File" << endl;
    cout << "5. Exit" << endl;
    cout << "========================================" << endl;
    cout << "Please Select An Option (1-5): ";
}

// Function to display account operations menu
void displayAccountMenu(const string& accountType) {
    cout << "\n=== " << accountType << " ACCOUNT OPERATIONS ===" << endl;
    cout << "1. Check Account Balance" << endl;
    cout << "2. Deposit Money" << endl;
    cout << "3. Withdraw Money" << endl;
    if (accountType == "SAVINGS") {
        cout << "4. Add Interest" << endl;
        cout << "5. Display Account Information" << endl;
        cout << "6. Back to Main Menu" << endl;
    } else {
        cout << "4. Display Account Information" << endl;
        cout << "5. Back to Main Menu" << endl;
    }
    cout << "========================================" << endl;
    cout << "Please Select An Option: ";
}

// Function to display account information
void displayAccountInfo(Account* account, SavingsAccount* savings = nullptr, ChequingAccount* chequing = nullptr) {
    cout << "\n=== ACCOUNT INFORMATION ===" << endl;
    cout << "Current Balance: $" << fixed << setprecision(2) << account->GetBalance() << endl;
    
    if (savings != nullptr) {
        cout << "Account Type: Savings Account" << endl;
        cout << "Interest Rate: " << fixed << setprecision(2) << savings->GetInterestRate() << "%" << endl;
        cout << "Interest Earnable: $" << fixed << setprecision(2) << savings->CalculateInterest() << endl;
    }
    
    if (chequing != nullptr) {
        cout << "Account Type: Chequing Account" << endl;
        cout << "Transaction Fee: $" << fixed << setprecision(2) << chequing->GetTransactionFee() << " per transaction" << endl;
    }
    cout << "===========================" << endl;
}

int main() {
    cout << "=== TRAJJ BANKING SERVICES ===" << endl;
    cout << "Enhanced with Transaction Logging System" << endl;
    cout << "========================================\n" << endl;

    // Test the Transaction class
    cout << "Testing Transaction Class..." << endl;
    try {
        Transaction testTrans(100.50, "TEST_DEPOSIT", "SAVINGS");
        cout << "Transaction created successfully!" << endl;
        cout << "Transaction report: " << testTrans.report() << endl;
    } catch (const exception& e) {
        cout << "Error testing Transaction class: " << e.what() << endl;
    }
    cout << "\n";

    // Create accounts with exception handling
    double initialBalance;
    double interestRate, transactionFee;
    
    cout << "Welcome to Trajj Banking Services" << endl;
    
    // Get initial balance with error handling
    while (true) {
        try {
            cout << "Please Enter Initial Account Balance (minimum $1000.00): $";
            cin >> initialBalance;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid input. Please enter a numeric value.");
            }
            
            if (initialBalance < 1000.00) {
                throw invalid_argument("Initial balance must be at least $1000.00");
            }
            
            break;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << ". Please try again." << endl;
        }
    }
    
    // Get interest rate with error handling
    while (true) {
        try {
            cout << "Enter Interest Rate for Savings Account (%): ";
            cin >> interestRate;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid input. Please enter a numeric value.");
            }
            
            if (interestRate < 0) {
                throw invalid_argument("Interest rate cannot be negative");
            }
            
            break;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << ". Please try again." << endl;
        }
    }
    
    // Get transaction fee with error handling
    while (true) {
        try {
            cout << "Enter Transaction Fee for Chequing Account: $";
            cin >> transactionFee;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid input. Please enter a numeric value.");
            }
            
            if (transactionFee < 0) {
                throw invalid_argument("Transaction fee cannot be negative");
            }
            
            break;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << ". Please try again." << endl;
        }
    }
    
    // Create accounts
    SavingsAccount savingsAccount(initialBalance, interestRate);
    ChequingAccount chequingAccount(initialBalance, transactionFee);
    
    int mainChoice, accountChoice;
    double amount;
    bool usingSavings = true;
    Account* currentAccount = &savingsAccount;
    
    // Main program loop 
    while (true) {
        displayMainMenu();
        
        try {
            cin >> mainChoice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid input. Please enter a number.");
            }
            
            switch (mainChoice) {
                case 1: // Use Savings Account
                    usingSavings = true;
                    currentAccount = &savingsAccount;
                    cout << "\nSwitched to Savings Account" << endl;
                    
                    // Savings Account operations loop
                    while (true) {
                        displayAccountMenu("SAVINGS");
                        cin >> accountChoice;
                        
                        if (cin.fail()) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input. Please enter a number." << endl;
                            continue;
                        }
                        
                        try {
                            switch (accountChoice) {
                                case 1: // Check Balance
                                    cout << "\nSavings Account Balance: $" 
                                         << fixed << setprecision(2) 
                                         << currentAccount->GetBalance() << endl;
                                    break;
                                    
                                case 2: // Deposit Money
                                    cout << "\nEnter Deposit Amount: $";
                                    cin >> amount;
                                    currentAccount->Deposit(amount);
                                    break;
                                    
                                case 3: // Withdraw Money
                                    cout << "\nEnter Withdrawal Amount: $";
                                    cin >> amount;
                                    currentAccount->Withdraw(amount);
                                    break;
                                    
                                case 4: // Add Interest
                                    static_cast<SavingsAccount*>(currentAccount)->AddInterest();
                                    break;
                                    
                                case 5: // Display Account Information
                                    displayAccountInfo(currentAccount, &savingsAccount, nullptr);
                                    break;
                                    
                                case 6: // Back to Main Menu
                                    cout << "Returning to Main Menu..." << endl;
                                    break;
                                    
                                default:
                                    cout << "Invalid Option! Please try again." << endl;
                                    break;
                            }
                        } catch (const exception& e) {
                            cout << "Operation failed: " << e.what() << endl;
                        }
                        
                        if (accountChoice == 6) break;
                        
                        // Pause to let user see the result
                        cout << "\nPress Enter To Continue...";
                        cin.ignore();
                        cin.get();
                    }
                    break;
                    
                case 2: // Use Chequing Account
                    usingSavings = false;
                    currentAccount = &chequingAccount;
                    cout << "\nSwitched to Chequing Account" << endl;
                    
                    // Chequing Account operations loop
                    while (true) {
                        displayAccountMenu("CHEQUING");
                        cin >> accountChoice;
                        
                        if (cin.fail()) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input. Please enter a number." << endl;
                            continue;
                        }
                        
                        try {
                            switch (accountChoice) {
                                case 1: // Check Balance
                                    cout << "\nChequing Account Balance: $" 
                                         << fixed << setprecision(2) 
                                         << currentAccount->GetBalance() << endl;
                                    break;
                                    
                                case 2: // Deposit Money
                                    cout << "\nEnter Deposit Amount: $";
                                    cin >> amount;
                                    currentAccount->Deposit(amount);
                                    break;
                                    
                                case 3: // Withdraw Money
                                    cout << "\nEnter Withdrawal Amount: $";
                                    cin >> amount;
                                    currentAccount->Withdraw(amount);
                                    break;
                                    
                                case 4: // Display Account Information
                                    displayAccountInfo(currentAccount, nullptr, &chequingAccount);
                                    break;
                                    
                                case 5: // Back to Main Menu
                                    cout << "Returning to Main Menu..." << endl;
                                    break;
                                    
                                default:
                                    cout << "Invalid Option! Please try again." << endl;
                                    break;
                            }
                        } catch (const exception& e) {
                            cout << "Operation failed: " << e.what() << endl;
                        }
                        
                        if (accountChoice == 5) break;
                        
                        // Pause to let user see the result
                        cout << "\nPress Enter To Continue...";
                        cin.ignore();
                        cin.get();
                    }
                    break;
                    
                case 3: // View Account Reports
                    cout << "\n=== ACCOUNT REPORTS ===" << endl;
                    cout << "1. Savings Account Report" << endl;
                    cout << "2. Chequing Account Report" << endl;
                    cout << "3. Both Accounts Report" << endl;
                    cout << "Enter choice: ";
                    
                    int reportChoice;
                    cin >> reportChoice;
                    
                    try {
                        switch (reportChoice) {
                            case 1:
                                savingsAccount.report();
                                break;
                            case 2:
                                chequingAccount.report();
                                break;
                            case 3:
                                savingsAccount.report();
                                cout << endl;
                                chequingAccount.report();
                                break;
                            default:
                                cout << "Invalid choice!" << endl;
                        }
                    } catch (const exception& e) {
                        cout << "Error generating report: " << e.what() << endl;
                    }
                    break;
                    
                case 4: // Save Reports to File
                    cout << "\nSaving reports to file..." << endl;
                    try {
                        bool success1 = savingsAccount.saveReportToFile();
                        bool success2 = chequingAccount.saveReportToFile();
                        
                        if (success1 && success2) {
                            cout << "All reports saved successfully!" << endl;
                        }
                    } catch (const exception& e) {
                        cout << "Error saving reports: " << e.what() << endl;
                    }
                    break;
                    
                case 5: // Exit Program
                    cout << "\nThank You For Banking With Trajj Banking Services. Goodbye!" << endl;
                    
                    // Save final reports on exit
                    try {
                        savingsAccount.saveReportToFile("C:\\final_savings_report.txt");
                        chequingAccount.saveReportToFile("C:\\final_chequing_report.txt");
                    } catch (...) {
                        // Ignore file errors on exit
                    }
                    
                    return 0;
                    
                default: // Invalid Option 
                    cout << "\nInvalid Option! Please Select a Valid Option (1-5)." << endl;
                    break;
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << ". Please try again." << endl;
        }
        
        // Pause to let user see the result
        cout << "\nPress Enter To Continue...";
        cin.ignore();
        cin.get();
    }
    
    return 0;
}