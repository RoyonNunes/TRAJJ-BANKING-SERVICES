#include "Banking.h"

using namespace std;

int main() {
    
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
                        savingsAccount.saveReportToFile("final_savings_report.txt");
                        chequingAccount.saveReportToFile("final_chequing_report.txt");
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