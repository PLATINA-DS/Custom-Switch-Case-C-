// Demonstrates the usage of the custom switch defined in custom_switch.hpp

#include <iostream>  
#include <string>    
#include <vector>  

#include "custom_switch.hpp" // Include the custom switch header
using namespace std;

// ANSI escape codes for colors
#define COLOR_BLUE     "\033[1;36m"  // Cyan color
#define COLOR_RESET    "\033[0m"     // Reset color


int main() {
    // --- Example 1: Switching on an integer ---
    int value = 50;
    cout << "Testing int value = " << value << endl;

    // Start the switch block for 'value'
    SWITCH(value) { // User's block within the SWITCH scope
    
        // Define the first case
        CASE(val <= 100 && val >= 0) // Condition uses 'val'
            cout << "Value is in range [0, 100]" << endl; // Action
        BREAK // End case definition

        // Define the second case
        CASE(val > 100)
            cout << "Value is greater than 100" << endl;
        BREAK

        // Define the third case
        CASE(val < 0)
            cout << "Value is less than 0" << endl;
        BREAK

        // Define the optional default case
        DEFAULT
            cout << "Unexpected value (default)" << endl;
        END_DEFAULT // End default definition

    } END_SWITCH // Evaluate the switch and close the SWITCH macro's scope

    cout << "---" << endl;


    // --- Example 2: Switching on a string ---
    // Changing console color
    cout << COLOR_BLUE;
    
    string str = "Hello Gerard!";
    string name = "Gerard";
    cout << "Testing string value = \"" << str << "\"" << endl;

    SWITCH(str) {
        // Case comparing the string value
        CASE(val.find(name) != string::npos)
            cout << "Hi Gerard! This is my own implementation of switch-case. And this method supports any operators)" << endl;
        BREAK

        // Case checking string properties
        CASE(val.length() > 10)
            cout << "Long string" << endl;
        BREAK

        // Default case for the string switch
        DEFAULT
            // Note: 'str' (the original variable) is accessible here due to [&] capture
            cout << "Other string: " << str << endl;
        END_DEFAULT

    } END_SWITCH

    cout << "---" << endl;
    // Reseting console color
    cout << COLOR_RESET;

    // --- Example 3: Integer switch without a DEFAULT case ---
    int another_val = 10;
     cout << "Testing int value = " << another_val << " without DEFAULT" << endl;

     SWITCH(another_val) {
        // First case
        CASE(val == 10)
            cout << "Value is 10" << endl;
        BREAK

        // Second case
        CASE(val > 10)
            cout << "Value is greater than 10" << endl;
        BREAK
        // No DEFAULT case is provided here
     } END_SWITCH // Evaluation happens; if no case matches, nothing executes


    return 0;
}
