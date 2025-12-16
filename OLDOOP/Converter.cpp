/*
Name: Royon Nunes
ID Number: 1400147929
Program Description: This program creates a Converter class with overloaded constructors and various unit conversion functions. It demonstrates constructor
overloading with validation and provides conversion methods for USD to JMD, Miles to Kilometers, Fahrenheit to Celsius,  Pounds to Kilograms, and Gallons to Litres.
Date: December 12, 2025
Question: 2
Instructor: Doron Williams
*/

#include <iostream>
using namespace std;

// ======================CONVERSION CLASS======================
class Converter {
private:
    float value;

public:
    // ======================OVERLOADED CONSTRUCTORS ======================
    // Default constructor
    Converter() {
        value = 1.0;
        cout << "Converter Created With Default Value: 1.0" << endl;
    }
    
    // Parameterized constructor with validation 
    Converter(float val) {
        if (val > 0.0) {
            value = val;
            cout << "Converter Created With Value: " << val << endl;
        } else {
            cout << "Error: Value Must Be Greater Than 0.0. Setting to default 1.0" << endl;
            value = 1.0;
        }
    }
    
    // ======================CONVERSION FUNCTIONS======================
    
    // a. USD to JMD
    void usdToJmd() {
        float jmd = value * 137.83;
        cout << value << " USD Is Approximately " << jmd << " JMD" << endl;
    }
    
    // b. Miles to Kilometers
    void milesToKm() {
        float km = value * 1.609;
        cout << value << " Miles Is Approximately " << km << " Kilometers" << endl;
    }
    
    // c. Fahrenheit to Celsius
    void fahrenheitToCelsius() {
        float celsius = (value - 32) * 5.0/9.0;
        cout << value << " Fahrenheit is Approximately " << celsius << " Celsius" << endl;
    }
    
    // d. Pounds to Kilograms
    void poundsToKg() {
        float kg = value / 2.205;
        cout << value << " Pounds Is Approximately " << kg << " Kilograms" << endl;
    }
    
    // e. Gallons to Litres
    void gallonsToLitres() {
        float litres = value * 3.785;
        cout << value << " Gallons Is Approximately " << litres << " Litres" << endl;
    }
};

// Main function for testing
int main() {
    cout << "===== CONVERTER TEST =====" << endl;
    
    // Test default constructor
    Converter conv1;
    conv1.usdToJmd();
    conv1.milesToKm();
    
    cout << "\n";
    
    // Test parameterized constructor with valid value
    Converter conv2(10.5);
    conv2.fahrenheitToCelsius();
    conv2.poundsToKg();
    conv2.gallonsToLitres();
    
    cout << "\n";
    
    // Test parameterized constructor with invalid value
    Converter conv3(-5.0);
    conv3.usdToJmd();
    
    return 0;
}