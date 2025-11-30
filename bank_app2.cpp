#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class Account {
protected:
    double balance;

public:
    // Constructor with validation for initial balance
    Account(double initialBalance) {
        if (initialBalance >= 1000.00) {
            balance = initialBalance;
        } else {
            balance = 0.0;
            cout << "Warning: The Initial Balance Must Be At Least $1000.00. Account Balance Set To $0.00" << endl;
        }
    }

    // Virtual destructor for proper inheritance 
    virtual ~Account() = default;

    // Deposit money into account to increase balance 
    virtual void Deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Successfully Deposited: $" << fixed << setprecision(2) << amount << endl;
        } else {
            cout << "Invalid Deposit Amount!" << endl;
        }
    }

    // Withdraw money from account with balance check 
    virtual void Withdraw(double amount) {
        if (amount > 0) {
            if (amount <= balance) {
                balance -= amount;
                cout << "Successfully Withdrew: $" << fixed << setprecision(2) << amount << endl;
            } else {
                cout << "Debit Amount Exceeded Account Balance. Withdrawal Failed." << endl;
            }
        } else {
            cout << "Invalid Withdrawal Amount!" << endl;
        }
    }

    // Get current balance of the account
    double GetBalance() const {
        return balance;
    }
};

// Derived Class SavingsAccount
class SavingsAccount : public Account {
private:
    double interestRate; // as percentage (e.g., 2.5 for 2.5%)

public:
    // Constructor inheriting from Account
    SavingsAccount(double initialBalance, double rate) 
        : Account(initialBalance), interestRate(rate) {
    }

    // Calculate interest earned
    double CalculateInterest() const {
        return balance * (interestRate / 100.0);
    }

    // Add interest to the account
    void AddInterest() {
        double interest = CalculateInterest();
        balance += interest;
        cout << "Interest of $" << fixed << setprecision(2) << interest 
             << " added to savings account. New balance: $" << balance << endl;
    }

    // Get interest rate
    double GetInterestRate() const {
        return interestRate;
    }
};

// Derived Class ChequingAccount
class ChequingAccount : public Account {
private:
    double transactionFee;

public:
    // Constructor inheriting from Account
    ChequingAccount(double initialBalance, double fee) 
        : Account(initialBalance), transactionFee(fee) {
    }

    // Override Withdraw to include transaction fee
    void Withdraw(double amount) override {
        if (amount > 0) {
            double totalAmount = amount + transactionFee;
            if (totalAmount <= balance) {
                balance -= totalAmount;
                cout << "Successfully Withdrew: $" << fixed << setprecision(2) << amount << endl;
                cout << "Transaction fee: $" << fixed << setprecision(2) << transactionFee << endl;
                cout << "Total deducted: $" << fixed << setprecision(2) << totalAmount << endl;
            } else {
                cout << "Debit Amount + Fee Exceeded Account Balance. Withdrawal Failed." << endl;
            }
        } else {
            cout << "Invalid Withdrawal Amount!" << endl;
        }
    }

    // Override Deposit to include transaction fee
    void Deposit(double amount) override {
        if (amount > 0) {
            // For chequing account, fee is charged on deposits too
            if (transactionFee <= balance) {
                balance -= transactionFee; // Charge fee
                balance += amount; // Add deposit
                cout << "Successfully Deposited: $" << fixed << setprecision(2) << amount << endl;
                cout << "Transaction fee charged: $" << fixed << setprecision(2) << transactionFee << endl;
                cout << "Net balance change: +$" << fixed << setprecision(2) << (amount - transactionFee) << endl;
            } else {
                cout << "Insufficient balance to pay transaction fee. Deposit failed." << endl;
            }
        } else {
            cout << "Invalid Deposit Amount!" << endl;
        }
    }

    // Get transaction fee
    double GetTransactionFee() const {
        return transactionFee;
    }
};

// Function to display main menu options
void displayMainMenu() {
    cout << "\n=== TRAJJ BANKING SERVICES INTERFACE ===" << endl;
    cout << "1. Use Savings Account" << endl;
    cout << "2. Use Chequing Account" << endl;
    cout << "3. Exit" << endl;
    cout << "========================================" << endl;
    cout << "Please Select An Option (1-3): ";
}

// Function to display account operations menu
void displayAccountMenu(const string& accountType) {
    cout << "\n=== " << accountType << " ACCOUNT OPERATIONS ===" << endl;
    cout << "1. Check Account Balance" << endl;
    cout << "2. Deposit Money" << endl;
    cout << "3. Withdraw Money" << endl;
    if (accountType == "SAVINGS") {
        cout << "4. Add Interest" << endl;
    }
    cout << "5. Display Account Information" << endl;
    cout << "6. Back to Main Menu" << endl;
    cout << "========================================" << endl;
    cout << "Please Select An Option (1-6): ";
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
    // Create accounts with initial balances and specific parameters
    double initialBalance;
    double interestRate, transactionFee;
    
    cout << "Welcome to Trajj Banking Services" << endl;
    
    // Get initial balance
    cout << "Please Enter Initial Account Balance (minimum $1000.00): $";
    cin >> initialBalance;
    
    // Create savings account
    cout << "Enter Interest Rate for Savings Account (%): ";
    cin >> interestRate;
    SavingsAccount savingsAccount(initialBalance, interestRate);
    
    // Create chequing account
    cout << "Enter Transaction Fee for Chequing Account: $";
    cin >> transactionFee;
    ChequingAccount chequingAccount(initialBalance, transactionFee);
    
    int mainChoice, accountChoice;
    double amount;
    bool usingSavings = true; // Track which account we're using
    Account* currentAccount = &savingsAccount;
    
    // Main program loop 
    while (true) {
        displayMainMenu();
        cin >> mainChoice;
        
        switch (mainChoice) {
            case 1: // Use Savings Account
                usingSavings = true;
                currentAccount = &savingsAccount;
                cout << "\nSwitched to Savings Account" << endl;
                
                // Savings Account operations loop
                while (true) {
                    displayAccountMenu("SAVINGS");
                    cin >> accountChoice;
                    
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
                            savingsAccount.AddInterest();
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
                            
                        case 5: // Display Account Information
                            displayAccountInfo(currentAccount, nullptr, &chequingAccount);
                            break;
                            
                        case 6: // Back to Main Menu
                            cout << "Returning to Main Menu..." << endl;
                            break;
                            
                        default:
                            cout << "Invalid Option! Please try again." << endl;
                            break;
                    }
                    
                    if (accountChoice == 6) break;
                    
                    // Pause to let user see the result
                    cout << "\nPress Enter To Continue...";
                    cin.ignore();
                    cin.get();
                }
                break;
                
            case 3: // Exit Program
                cout << "\nThank You For Banking With Trajj Banking Services. Goodbye!" << endl;
                return 0;
                
            default: // Invalid Option 
                cout << "\nInvalid Option! Please Select a Valid Option (1-3)." << endl;
                break;
        }
        
        // Pause to let user see the result
        cout << "\nPress Enter To Continue...";
        cin.ignore();
        cin.get();
    }
    
    return 0;
}
