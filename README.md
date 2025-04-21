# C++ Custom Switch Implementation

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)

A header-only C++ library providing a flexible `switch`-like construct that allows complex boolean conditions in `CASE` statements, overcoming the limitations of the standard C++ `switch`.

## Motivation

This project began as a practical exploration following a discussion with Mr. Gerard regarding the constraints of the built-in C++ `switch` statement, which only works with integral types and constant expressions.

The primary goal was to demonstrate that a more versatile, condition-based switch mechanism is not only possible but also achievable using modern C++ features like templates, lambdas, and `std::function`. This repository stands as proof of that concept. Take that, Mr. Gerard! 😉

## Features

* **Complex Conditions:** Use any boolean expression in `CASE` statements (e.g., `val > 10 && val < 50`, `val.find("substring") != std::string::npos`, comparisons involving external variables).
* **Multiple Data Types:** Works with various data types (integers, strings, vectors, custom classes, etc.) thanks to templates.
* **Familiar Syntax:** Uses macros (`SWITCH`, `CASE`, `BREAK`, `DEFAULT`, `END_DEFAULT`, `END_SWITCH`) to mimic the structure of a standard `switch`.
* **Variable Capture:** `CASE` conditions can capture and use variables from the surrounding scope.
* **`DEFAULT` Branch:** Supports an optional `DEFAULT` case.
* **Header-Only:** Easy integration – just include `custom_switch.hpp`.

## Requirements

* A C++ compiler supporting **C++17** or later (due to usage of `std::optional`, `std::decay_t`).

## Installation & Usage

1.  **Download:** Place the `custom_switch.hpp` file in your project's include path.
2.  **Include:** Add `#include "custom_switch.hpp"` to your source file.
3.  **Use Macros:** Structure your switch logic using the provided macros.

**Basic Structure:**

```cpp
#include "custom_switch.hpp"
#include <string>
#include <iostream>

// ... other necessary includes ...

int main() {
    SomeType variable = /* ... some value ... */;
    ExternalVarType externalVar = /* ... used in a condition ... */;

    // Start the switch for 'variable'
    SWITCH(variable) { // User provides this block scope {}

        // Define a case. 'val' represents the value of 'variable' here.
        // The condition lambda captures external variables by reference ([&]).
        CASE(val > 0 && val < 10 || val == externalVar)
            // Action code for this case...
            std::cout << "Condition 1 met!" << std::endl;
        BREAK // Required to end the case definition

        // Another case
        CASE(some_complex_check(val))
            // Action code...
            std::cout << "Condition 2 met!" << std::endl;
        BREAK

        // Optional default case
        DEFAULT
            // Action code if no other case matches...
            std::cout << "Default case executed." << std::endl;
        END_DEFAULT // Required if DEFAULT is present

    } END_SWITCH // IMPORTANT: END_SWITCH goes *after* the closing brace '}'

    return 0;
}
```

# Key Usage Points

* Inside `CASE(...)`, the condition uses the predefined name `val` to refer to the value passed to `SWITCH`.
* The condition lambda automatically captures external variables by reference (`[&]`), so you can use variables from the surrounding scope directly in the condition.
* The code block following `CASE` or `DEFAULT` is the action to be performed.
* `BREAK` is mandatory after the action code in a `CASE`.
* `END_DEFAULT` is mandatory after the action code in `DEFAULT`.
* `END_SWITCH` must be placed immediately after the closing brace `}` of the block you provide for the `SWITCH`.

# Examples

*(Taken from `main.cpp`)*

**1. Switching on an `int`:**

```cpp
#include "custom_switch.hpp"
#include <iostream>

int main() {
    int value = 50;
    std::cout << "Testing int value = " << value << std::endl;

    SWITCH(value) {
        CASE(val <= 100 && val >= 0)
            std::cout << "Value is in range [0, 100]" << std::endl;
        BREAK

        CASE(val > 100)
            std::cout << "Value is greater than 100" << std::endl;
        BREAK

        CASE(val < 0)
            std::cout << "Value is less than 0" << std::endl;
        BREAK

        DEFAULT
            std::cout << "Unexpected value (default)" << std::endl;
        END_DEFAULT

    } END_SWITCH
    return 0;
}
```

**2. Switching on a `std::string` and using external variables:**

```cpp
#include "custom_switch.hpp"
#include <iostream>
#include <string>

int main() {
    std::string str = "Some text including Gerard";
    std::string name_to_find = "Gerard"; // External variable

    std::cout << "Testing string: \"" << str << "\" with name: \"" << name_to_find << "\"" << std::endl;

    SWITCH(str) {
        CASE(val == "hello")
            std::cout << "String is 'hello'" << std::endl;
        BREAK

        // Condition uses external 'name_to_find' captured by the lambda
        CASE(val.find(name_to_find) != std::string::npos)
            std::cout << "Hi Gerard! Found the name." << std::endl;
        BREAK

        CASE(val.length() > 30)
            std::cout << "Long string" << std::endl;
        BREAK

        DEFAULT
            std::cout << "Other string: " << str << std::endl;
        END_DEFAULT

    } END_SWITCH
    return 0;
}
```
