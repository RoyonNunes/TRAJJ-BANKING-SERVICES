# OOP - Bank Application TRAJJ BANKING SERVICES

A C++ object-oriented programming project demonstrating inheritance, polymorphism, and encapsulation through a simple banking system.

## Project Overview

This repository contains two bank application implementations that showcase OOP principles:

- **`bank_app.cpp`** — Basic bank application
- **`bank_app2.cpp`** — Enhanced version with `SavingsAccount` and `ChequingAccount` classes featuring interest rates and transaction fees

## Features

### Core Classes

- **Account** — Base class with deposit/withdrawal operations and balance management
  - Constructor validates minimum balance of $1,000.00
  - Virtual methods for polymorphic behavior
  
- **SavingsAccount** — Inherits from Account with interest rate functionality
  - Calculates interest based on current balance and rate
  - Add interest method to accrue earnings
  
- **ChequingAccount** — Inherits from Account with transaction fee management
  - Charges fees on both deposits and withdrawals
  - Overrides virtual methods to apply fee logic

### Account Operations

- ✓ Check account balance
- ✓ Deposit money
- ✓ Withdraw money
- ✓ View detailed account information
- ✓ Calculate and apply interest (Savings Account only)
- ✓ Track transaction fees (Chequing Account only)

## Prerequisites

- **C++ Compiler**: GCC/G++ (via MSYS2 recommended for Windows)
- **Platform**: Windows (PowerShell)
- **IDE**: Visual Studio Code (optional, with C/C++ extension)

## Build Instructions

### Option 1: Using VS Code Build Task (Recommended)

1. Open the desired `.cpp` file in VS Code
2. Press `Ctrl+Shift+B` or go to **Terminal → Run Build Task**
3. Select **C/C++: gcc.exe build active file**
4. Executable will be created in the same directory as the source file

## Usage Example

### Initial Setup
1. Start the program
2. Enter an initial balance (minimum $1,000.00)
3. Set Savings Account interest rate (e.g., 2.5%)
4. Set Chequing Account transaction fee (e.g., $2.50)

### Using Savings Account
- Check balance
- Deposit money (no fees)
- Withdraw money (no fees)
- Add interest to earn passive income
- View account details including interest earnable

### Using Chequing Account
- Check balance
- Deposit money (transaction fee applied)
- Withdraw money (transaction fee applied)
- View account details including transaction fee per operation

### Example Session
```
Welcome to Trajj Banking Services
Please Enter Initial Account Balance (minimum $1000.00): $5000
Enter Interest Rate for Savings Account (%): 2.5
Enter Transaction Fee for Chequing Account: $2.50

=== TRAJJ BANKING SERVICES INTERFACE ===
1. Use Savings Account
2. Use Chequing Account
3. Exit


**Happy Banking!** 🏦

For questions or issues, feel free to open an issue in the repository.
