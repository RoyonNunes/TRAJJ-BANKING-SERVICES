#ifndef BANKING_H
#define BANKING_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <chrono>
#include <limits>

// Forward declarations
class Transaction;
class Account;
class SavingsAccount;
class ChequingAccount;

// Transaction Class
class Transaction {
private:
    double amount;
    std::string type; // "DEPOSIT", "WITHDRAWAL", "INTEREST", "FEE", "BALANCE_INQUIRY"
    std::string timestamp;
    std::string accountType; // "SAVINGS" or "CHEQUING"

public:
    // Parameterized constructor
    Transaction(double amt, const std::string& t, const std::string& accType);
    
    // Getters
    double getAmount() const;
    std::string getType() const;
    std::string getTimestamp() const;
    std::string getAccountType() const;

    // report() function as required
    std::string report() const;
};

// Account Base Class
class Account {
protected:
    double balance;
    std::vector<Transaction> log; // Transaction log as required
    std::string accountNumber;
    std::string accountTypeName; // Store account type as string

    // Helper method to add transaction to log
    void addToLog(const Transaction& transaction);
    
    // Helper method to generate account number
    std::string generateAccountNumber();
    
    // Helper method to get current timestamp
    std::string getCurrentTimestamp() const;

    // Helper method to get account type for logging (used in constructor)
    virtual std::string getAccountTypeForLog() const { return "UNKNOWN"; }

public:
    // Constructor with validation for initial balance
    Account(double initialBalance, const std::string& accNum = "", const std::string& accType = "UNKNOWN");
    
    // Virtual destructor for proper inheritance 
    virtual ~Account() = default;

    // Deposit money into account to increase balance 
    virtual void Deposit(double amount);
    
    // Withdraw money from account with balance check 
    virtual void Withdraw(double amount);
    
    // Get current balance of the account
    double GetBalance() const;
    
    // Virtual function to get account type (to be overridden by derived classes)
    virtual std::string getAccountType() const = 0;

    // report() function as required - formats transaction information
    virtual void report() const;
    
    // Method to save report to file
    bool saveReportToFile(const std::string& filename = "transactions.txt") const;
};

// Derived Class SavingsAccount
class SavingsAccount : public Account {
private:
    double interestRate; // as percentage (e.g., 2.5 for 2.5%)

    // Override helper method
    std::string getAccountTypeForLog() const override { return "SAVINGS"; }

public:
    // Constructor inheriting from Account
    SavingsAccount(double initialBalance, double rate, const std::string& accNum = "");
    
    // Calculate interest earned
    double CalculateInterest() const;
    
    // Add interest to the account
    void AddInterest();
    
    // Get interest rate
    double GetInterestRate() const;
    
    // Override getAccountType
    std::string getAccountType() const override;
    
    // Override report to include savings-specific info
    void report() const override;
};

// Derived Class ChequingAccount
class ChequingAccount : public Account {
private:
    double transactionFee;

    // Override helper method
    std::string getAccountTypeForLog() const override { return "CHEQUING"; }

public:
    // Constructor inheriting from Account
    ChequingAccount(double initialBalance, double fee, const std::string& accNum = "");
    
    // Override Withdraw to include transaction fee
    void Withdraw(double amount) override;
    
    // Override Deposit to include transaction fee
    void Deposit(double amount) override;
    
    // Get transaction fee
    double GetTransactionFee() const;
    
    // Override getAccountType
    std::string getAccountType() const override;
    
    // Override report to include chequing-specific info
    void report() const override;
};

// Menu display functions
void displayMainMenu();
void displayAccountMenu(const std::string& accountType);
void displayAccountInfo(Account* account, SavingsAccount* savings = nullptr, ChequingAccount* chequing = nullptr);

#endif