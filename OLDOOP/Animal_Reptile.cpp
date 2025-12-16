/*
Name: Royon Nunes
ID Number: 1400147929
Program Description: This program implements an Animal base class and a Reptile derived class to demonstrate inheritance and polymorphism. It includes 
base behaviors, attribute setters with validation, and display functions. The Reptile class overrides behaviors and prevents
incorrect mammal classification, showcasing proper OOP principles.
Date: December 12, 2025
Question: 3
Instructor: Doron Williams
*/

#include <iostream>
#include <string>
using namespace std;

// ======================BASE ANIMAL CLASS======================
class Animal {
private:
    int size;
    int height;
    string name;
    bool isMammal;

public:
    // ======================BASE CONSTRUCTOR======================
    Animal() {
        size = 1;
        isMammal = true;
    }
    
    // ======================BASE BEHAVIORS======================
    virtual void speak() {
        cout << "An Animal Makes A Sound." << endl;
    }
    
    virtual void move() {
        cout << "An Animal Moves." << endl;
    }
    
    // ======================UNIVERSAL SETTER======================
    virtual void setAttributes(int s, int h, string n, bool mammal) {
        size = s;
        height = h;
        name = n;
        isMammal = mammal;
    }
    
    // ======================DISPLAY INFORMATION ======================
    void displayInfo() {
        cout << "\n===== ANIMAL INFORMATION =====" << endl;
        cout << "Name: " << name << endl;
        cout << "Size: " << size << endl;
        cout << "Height: " << height << endl;
        
        if (isMammal) {
            cout << "Type: Mammal" << endl;
        } else {
            cout << "Type: Reptile" << endl;
        }
        cout << "==============================" << endl;
    }
    
    // Getters for derived class access
    bool getIsMammal() { return isMammal; }
    void setIsMammal(bool mammal) { isMammal = mammal; }
};

// ======================DERIVE REPTILE CLASS======================
class Reptile : public Animal {
public:
    // Default constructor for Reptile
    Reptile() {
        setIsMammal(false);

    }
    
    // ======================OVERRIDE BEHAVIORS ======================
    void speak() override {
        cout << "A Reptile Hisses/Slithers." << endl;
    }
    
    void move() override {
        cout << "A Reptile Hisses/Slithers." << endl; 
    }
    
    // ======================OVERRIDE SETTER LOGIC ======================
    void setAttributes(int s, int h, string n, bool mammal) override {
        // Check if trying to set reptile as mammal
        if (mammal) {
            cout << "Error: A reptile cannot Be A Mammal!" << endl;
            // Call parent setter with false for isMammal
            Animal::setAttributes(s, h, n, false);
        } else {
            Animal::setAttributes(s, h, n, false);
        }
    }
};

// Main function for testing
int main() {
    cout << "===== MAMMAL TEST=====" << endl;
    Animal dog;
    dog.setAttributes(3, 2, "Dog", true);
    dog.displayInfo();
    dog.speak();
    dog.move();
    
    cout << "\n===== REPTILE TEST =====" << endl;
    Reptile snake;
    snake.setAttributes(2, 1, "Snake", false);
    snake.displayInfo();
    snake.speak();
    snake.move();
    
    cout << "\n===== TEST REPTILE MAMMAL ERROR =====" << endl;
    Reptile lizard;
    // This should trigger the error message
    lizard.setAttributes(1, 1, "Lizard", true);
    lizard.displayInfo();
    
    // Additional test to show polymorphism
    Animal* animalPtr;
    
    Animal genericAnimal;
    animalPtr = &genericAnimal;
    animalPtr->speak(); // This will call the base Animal method
    
    Reptile specificReptile;
    animalPtr = &specificReptile;
    animalPtr->speak();  // This will call the overridden Reptile method
    
    return 0;
}