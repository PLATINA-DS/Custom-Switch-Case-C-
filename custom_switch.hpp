#ifndef CUSTOM_SWITCH_HPP
#define CUSTOM_SWITCH_HPP

#include <functional>
#include <vector>
#include <optional>   // requires C++ 17
#include <utility>

// Represents a single 'case' branch within the custom switch.
// Holds a `predicate` (condition) and an action to execute if the predicate is true.
template <typename T>
class Case {
public:
    // Constructor: Takes the predicate function and the action function.
    Case(std::function<bool(const T&)> predicate, std::function<void()> action)
        : predicate_(std::move(predicate)), action_(std::move(action)) {} // Use std::move

    // Evaluates the predicate against the given value.
    // If the predicate returns true, executes the action and returns true.
    // Else, returns false.
    bool evaluate(const T& value) {
        if (predicate_(value)) {
            action_();
            return true;
        }
        return false;
    }

private:
    std::function<bool(const T&)> predicate_; // The condition function (lambda).
    std::function<void()> action_;             // The action function (lambda).
};

// Represents the main 'switch' construct.
// Holds the value being switched on and manages a collection of Case objects.
template <typename T>
class Switch {
public:
    // Constructor: Takes the value to be switched on (moved or copied).
    Switch(T value) : value_(std::move(value)) {} // Use std::move

    // Adds a case branch to this switch instance.
    Switch& add_case(std::function<bool(const T&)> predicate, std::function<void()> action) {
        cases_.emplace_back(std::move(predicate), std::move(action)); // Use std::move
        return *this; // Allows chaining, though not used directly with macros.
    }

    // Sets the default action to be executed if no cases match.
    void add_default(std::function<void()> action) {
        default_action_ = std::move(action); // Use std::move
    }

    // Evaluates the switch logic:
    // Iterates through all added cases, executes the action of the first matching case,
    // and then stops (mimicking 'break'). If no cases match, executes the default action, if set.
    void evaluate() {
        bool executed = false;
        for (auto& c : cases_) {
            if (c.evaluate(value_)) {
                executed = true;
                break; // Stop after the first matching case.
            }
        }
        if (!executed && default_action_) {
             (*default_action_)(); // Execute default action if present and no case matched.
        }
    }

private:
    T value_; // The value being switched on.
    std::vector<Case<T>> cases_; // Stores all the defined case branches.
    std::optional<std::function<void()>> default_action_; // Stores the optional default action.
};

// --- Helper Macros for unique variable name generation ---
#define SWITCH_CONCAT_IMPL(a, b) a##b
#define SWITCH_CONCAT(a, b) SWITCH_CONCAT_IMPL(a, b)
#define SWITCH_VAR(prefix) SWITCH_CONCAT(prefix, __LINE__)

// --- Main Macros for the custom switch syntax ---

// Begins a custom switch block for the given value 'x'.
// Opens a scope and sets up internal variables and type aliases.
// Usage: SWITCH(my_variable) { ... } END_SWITCH
#define SWITCH(x) \
    { /* Open scope for the switch block */ \
        using SWITCH_VAR(_sw_value_type_) = std::decay_t<decltype(x)>; /* Deduce and clean the type of x */ \
        auto SWITCH_VAR(_sw_obj_) = Switch<SWITCH_VAR(_sw_value_type_)>(x); /* Create the Switch object */ \
        auto& _sw_obj_ = SWITCH_VAR(_sw_obj_); /* Create a convenient alias for the Switch object */ \
        using _sw_value_type_ = SWITCH_VAR(_sw_value_type_); /* Create a convenient alias for the value type */ \
        ; /* Semicolon to terminate declarations */ \
        /* User code block {...} follows here */

// Defines a case branch within the SWITCH block.
// 'condition' is a boolean expression, typically using 'val' which represents the switched value.
// Must be followed by the action code block and terminated by BREAK.
// Usage: CASE(val > 10 && val < 20) /* action code */ BREAK
#define CASE(condition) \
    _sw_obj_.add_case( \
        /* Predicate lambda: evaluates the condition. 'val' is the parameter name. */ \
        [&](const _sw_value_type_& val) -> bool { return (condition); }, \
        /* Action lambda: contains the user's code for this case. Captures by reference. */ \
        [&]() -> void { \
            /* User's action code starts here... */

// Terminates a CASE block definition. Must follow the action code inside a CASE.
#define BREAK \
        } /* End of action lambda */ \
    ); /* End of add_case call */

// Defines the default branch within the SWITCH block. Optional.
// Must be followed by the action code block and terminated by END_DEFAULT.
// Usage: DEFAULT /* action code */ END_DEFAULT
#define DEFAULT \
    _sw_obj_.add_default( \
        /* Action lambda for the default case. Captures by reference. */ \
        [&]() -> void { \
            /* User's default action code starts here... */

// Terminates a DEFAULT block definition. Must follow the action code inside DEFAULT.
#define END_DEFAULT \
        } /* End of default action lambda */ \
    ); /* End of add_default call */

// Evaluates the custom switch logic and closes the scope opened by SWITCH.
// Must be placed after the user's closing brace '}' for the switch block.
#define END_SWITCH \
    _sw_obj_.evaluate(); /* Trigger the evaluation of all cases/default */ \
} /* Close the scope opened by the SWITCH macro */

#endif // CUSTOM_SWITCH_HPP
