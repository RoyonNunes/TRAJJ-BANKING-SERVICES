#include <iostream> 
#include <string> 
using std::cout; 
using std::cin; 
using std::endl; 
using std::string; 
 
int main() { 
    int empID; 
    double salary = 0.0; 
    bool isActive = true; 
    string employeeName; 
 
    cout << "Enter Employee Name: "; 
    cin >> employeeName; 
 
    cout << "Enter Employee ID: "; 
    cin >> empID; 
 
    cout << "Enter Employee Salary: "; 
    cin >> salary; 
 
    cout << "\n--- Employee Appraisal ---" << endl; 
 
    if (isActive) { 
        cout << "Generating appraisal for " << employeeName << " (ID: " << empID << ")" << endl; 
        cout << "Current Salary: $" << salary << endl; 
        cout << "Updated Salary after 10% increase: $" << salary * 1.10 << endl; 
    } else { 
        cout << "Employee " << empID << " is inactive." << endl; 
    } 
 
    return 0; 
} 
