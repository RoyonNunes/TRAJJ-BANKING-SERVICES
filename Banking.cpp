#include "Banking.h"

using namespace std;

// ============================
// Transaction Class Implementation
// ============================

// Parameterized constructor
Transaction::Transaction(double amt, const std::string& t, const std::string& accType) 
    : amount(amt), type(t), accountType(accType) {
    // Generate timestamp
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    
    // Cross-platform timestamp generation
    char timeStr[100];
    #ifdef _WIN32
        ctime_s(timeStr, sizeof(timeStr), &now_time);
    #else
        ctime_r(&now_time, timeStr);
    #endif
    
    timestamp = string(timeStr);
    timestamp = timestamp.substr(0, timestamp.length() - 1); // Remove newline
}

// Getters
double Transaction::getAmount() const { return amount; }
string Transaction::getType() const { return type; }
string Transaction::getTimestamp() const { return timestamp; }
string Transaction::getAccountType() const { return accountType; }

// report() function as required
string Transaction::report() const {
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

// ============================
// Account Class Implementation
// ============================

// Constructor with validation for initial balance
Account::Account(double initialBalance, const string& accNum, const string& accType) 
    : accountNumber(accNum.empty() ? generateAccountNumber() : accNum), 
      accountTypeName(accType) {
    try {
        if (initialBalance >= 1000.00) {
            balance = initialBalance;
            // Log initial deposit - use accountTypeName instead of pure virtual function
            addToLog(Transaction(initialBalance, "INITIAL_DEPOSIT", accountTypeName));
        } else {
            throw invalid_argument("The Initial Balance Must Be At Least $1000.00");
        }
    } catch (const invalid_argument& e) {
        balance = 0.0;
        cout << "Warning: " << e.what() << ". Account Balance Set To $0.00" << endl;
        // Log failed initial deposit - use accountTypeName instead of pure virtual function
        addToLog(Transaction(0.0, "FAILED_INITIAL_DEPOSIT", accountTypeName));
    }
}

// Deposit money into account to increase balance 
void Account::Deposit(double amount) {
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
void Account::Withdraw(double amount) {
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
double Account::GetBalance() const {
    // Log balance inquiry
    const_cast<Account*>(this)->addToLog(Transaction(balance, "BALANCE_INQUIRY", getAccountType()));
    return balance;
}

// Helper method to add transaction to log
void Account::addToLog(const Transaction& transaction) {
    log.push_back(transaction);
}

// report() function as required - formats transaction information
void Account::report() const {
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
bool Account::saveReportToFile(const string& filename) const {
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

// Helper method to generate account number
string Account::generateAccountNumber() {
    static int accountCounter = 1000;
    return "ACC" + to_string(accountCounter++);
}

// Helper method to get current timestamp
string Account::getCurrentTimestamp() const {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    char timeStr[100];
    
    #ifdef _WIN32
        ctime_s(timeStr, sizeof(timeStr), &now_time);
    #else
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now_time));
    #endif
    
    return string(timeStr);
}

// ============================
// SavingsAccount Class Implementation
// ============================

// Constructor inheriting from Account
SavingsAccount::SavingsAccount(double initialBalance, double rate, const string& accNum) 
    : Account(initialBalance, accNum, "SAVINGS"), interestRate(rate) {
}

// Calculate interest earned
double SavingsAccount::CalculateInterest() const {
    return balance * (interestRate / 100.0);
}

// Add interest to the account
void SavingsAccount::AddInterest() {
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
double SavingsAccount::GetInterestRate() const {
    return interestRate;
}

// Override getAccountType
string SavingsAccount::getAccountType() const {
    return "SAVINGS";
}

// Override report to include savings-specific info
void SavingsAccount::report() const {
    Account::report();
    cout << "Interest Rate: " << fixed << setprecision(2) << interestRate << "%" << endl;
    cout << "Available Interest: $" << fixed << setprecision(2) << CalculateInterest() << endl;
}

// ============================
// ChequingAccount Class Implementation
// ============================

// Constructor inheriting from Account
ChequingAccount::ChequingAccount(double initialBalance, double fee, const string& accNum) 
    : Account(initialBalance, accNum, "CHEQUING"), transactionFee(fee) {
}

// Override Withdraw to include transaction fee
void ChequingAccount::Withdraw(double amount) {
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
void ChequingAccount::Deposit(double amount) {
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
double ChequingAccount::GetTransactionFee() const {
    return transactionFee;
}

// Override getAccountType
string ChequingAccount::getAccountType() const {
    return "CHEQUING";
}

// Override report to include chequing-specific info
void ChequingAccount::report() const {
    Account::report();
    cout << "Transaction Fee: $" << fixed << setprecision(2) << transactionFee << " per transaction" << endl;
}

// ============================
// Menu Functions Implementation
// ============================

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
void displayAccountInfo(Account* account, SavingsAccount* savings, ChequingAccount* chequing) {
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