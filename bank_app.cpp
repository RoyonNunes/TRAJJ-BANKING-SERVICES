#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class Account {
private:
    double balance;

public:
    // Constructor with validation for initial balance for account creation at $1000.00 minimum
    Account(double initialBalance) {
        if (initialBalance >= 1000.00) {
            balance = initialBalance;
        } else {
            balance = 0.0;
            cout << "Warning: The Initial Balance Must Be At Least $1000.00. Account Balance Set To $0.00" << endl;
        }
    }

    // Deposit money into account to increase balance
    void Deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Successfully Deposited: $" << fixed << setprecision(2) << amount << endl;
        } else {
            cout << "Invalid Deposit Amount!" << endl;
        }
    }

    // Withdraw money from account with balance check 
    void Withdraw(double amount) {
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
    double GetBalance() {
        return balance;
    }
};

// Function to display menu options for banking services to user
void displayMenu() {
    cout << "\n=== TRAJJ BANKING SERVICES INTERFACE ===" << endl;
    cout << "1. Check Account Balance" << endl;
    cout << "2. Deposit Money" << endl;
    cout << "3. Withdraw Money" << endl;
    cout << "4. Exit" << endl;
    cout << "===============================" << endl;
    cout << "Please Select An Option (1-4): ";
}

int main() {
    // Initialize account with initial balance 
    double initialBalance;
    cout << "Welcome to Trajj Banking Services!" << endl;
    cout << "Please Enter Initial Account Balance (minimum $1000.00): $";
    cin >> initialBalance;
    
    Account customerAccount(initialBalance);
    
    int choice;
    double amount;
    
    // Main program loop 
    while (true) {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: // Check Balance In Account
                cout << "\nCurrent Account Balance: $" 
                     << fixed << setprecision(2) 
                     << customerAccount.GetBalance() << endl;
                break;
                
            case 2: // Deposit Money To Account 
                cout << "\nEnter Deposit Amount: $";
                cin >> amount;
                customerAccount.Deposit(amount);
                break;
                
            case 3: // Withdraw Money From Account
                cout << "\nEnter Withdrawal Amount: $";
                cin >> amount;
                customerAccount.Withdraw(amount);
                break;
                
            case 4: // Exit Program
                cout << "\nThank You For Banking With Trajj Banking Services. Goodbye!" << endl;
                return 0;
                
            default: // Invalid Option 
                cout << "\nInvalid Option! Please Select a Valid Option (1-4)." << endl;
                break;
        }
        
        // Pause to let user see the result before showing menu again
        cout << "\nPress Enter To Continue...";
        cin.ignore(); // Clear input buffer
        cin.get();    // Wait for Enter key 
    }
    
    return 0;
}