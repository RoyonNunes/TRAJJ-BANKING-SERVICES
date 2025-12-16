/*
Name: Royon Nunes
Program Description: This program implements a Rectangle base class with constructors, getters/setters with validation, and area/perimeter calculations. 
It also demonstrates inheritance and polymorphism by creating a Triangle class that overrides the area calculation method.
Date: December 12, 2025
ID Number: 1400147929
Question: 1
Instructor: Doron Williams
*/

#include <iostream>
using namespace std;

// ======================BASE CLASS======================
class Rectangle {
private:
    float length;
    float height;

public:
    // ======================CONSTRUCTORS======================
    // Default constructor
    Rectangle() {
        length = 5.0;
        height = 5.0;
        cout << "Rectangle Created With Default Values (5.0, 5.0)" << endl;
    }

    // ======================SETTERS WITH VALIDATION======================
    void setLength(float l) {
        if (l > 0.0) {
            length = l;
            cout << "Length Is Set To: " << l << endl;
        } else {
            cout << "Error: The Length Must Be Greater Than 0.0" << endl;
        }
    }

    void setHeight(float h) {
        if (h > 0.0) {
            height = h;
            cout << "The Height Is Set To: " << h << endl;
        } else {
            cout << "Error: The Height Must Be Greater Than 0.0" << endl;
        }
    }

    // ======================GETTERS======================
    float getLength() {
        return length;
    }

    float getHeight() {
        return height;
    }

    // ======================MEMBER FUNCTIONS======================
    virtual float calculateArea() {
        float area = length * height;
        cout << "Area Of Rectangle: " << area << endl;
        return area;
    }

    float calculatePerimeter() {
        float perimeter = 2 * (length + height);
        cout << "Perimeter Of Rectangle: " << perimeter << endl;
        return perimeter;
    }
};

// ======================INHERITANCE AND POLYMORPHISM======================
class Triangle : public Rectangle {
public:
    // Override calculateArea() for triangle
    float calculateArea() override {
        float area = (getLength() * getHeight()) / 2;
        cout << "Area Of Triangle: " << area << endl;
        return area;
    }
};

// Main function for testing the classes and their methods
int main() {
    cout << "=====RECTANGLE TEST=====" << endl;
    Rectangle rect;
    rect.setLength(10.5);
    rect.setHeight(4.2);
    rect.calculateArea();
    rect.calculatePerimeter();
    
    cout << "\n===== TRIANGLE TEST =====" << endl;
    Triangle tri;
    tri.setLength(10.5);
    tri.setHeight(4.2);
    tri.calculateArea();  // This will use for the overridden triangle formula
    tri.calculatePerimeter();  // This will use for the inherited rectangle method
    
    return 0;
}