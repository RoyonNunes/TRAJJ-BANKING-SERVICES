#include <iostream> 
#include <string> 
using std::cout; 
using std::cin; 
using std::endl; 
using std::string; 
 
int main() { 
    // Variable declarations
    int empID; 
    double salary = 0.0; 
    bool isActive = true; 
    string employeeName;
    int performanceScore;  // Stores performance rating (1-5)
    string performanceComment;  // Stores textual performance description
 
    // Get employee information from user
    cout << "Enter Employee Name: "; 
    cin >> employeeName; 
 
    cout << "Enter Employee ID: "; 
    cin >> empID; 
 
    cout << "Enter Employee Salary: $"; 
    cin >> salary; 
 
    // Get performance score with input validation
    cout << "Enter Performance Score (1-5): "; 
    cin >> performanceScore;
    
    // Validate performance score input
    while (performanceScore < 1 || performanceScore > 5) {
        cout << "Invalid score! Please enter a score between 1-5: ";
        cin >> performanceScore;
    }
 
    cout << "\n--- Employee Appraisal ---" << endl; 
 
    // Process appraisal only for active employees
    if (isActive) { 
        double salaryIncrease = 0.0;  // Stores the percentage increase
        double newSalary = salary;     // Stores the updated salary
        
        // Determine salary increase and performance comment based on score
        switch (performanceScore) {
            case 5:
                salaryIncrease = 0.20;  // 20% increase
                performanceComment = "Excellent";
                break;
            case 4:
                salaryIncrease = 0.10;  // 10% increase
                performanceComment = "Good";
                break;
            case 3:
                salaryIncrease = 0.05;  // 5% increase
                performanceComment = "Average";
                break;
            case 2:
                salaryIncrease = 0.0;   // No increase
                performanceComment = "Needs Improvement";
                break;
            case 1:
                salaryIncrease = 0.0;   // No increase
                performanceComment = "Poor";
                break;
        }
        
        // Calculate new salary
        newSalary = salary + (salary * salaryIncrease);
        
        // Display appraisal results
        cout << "Generating appraisal for " << employeeName << " (ID: " << empID << ")" << endl; 
        cout << "Current Salary: $" << salary << endl; 
        cout << "Performance Score: " << performanceScore << " - " << performanceComment << endl;
        
        // Display appropriate message based on salary increase
        if (salaryIncrease > 0) {
            cout << "Salary Increase: " << (salaryIncrease * 100) << "%" << endl;
            cout << "Updated Salary: $" << newSalary << endl; 
        } else {
            cout << "No salary increase this year." << endl;
            cout << "Salary remains: $" << salary << endl;
        }
        
    } else { 
        // Display message for inactive employees
        cout << "Employee " << empID << " is inactive. No appraisal generated." << endl; 
    } 
 
    return 0; 
}
