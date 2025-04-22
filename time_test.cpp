#include <iostream>
#include <vector>
#include <string>
#include <chrono>   // For time measurement
#include <random>   // For generating test data

#include "custom_switch.hpp" // My custom switch header

using namespace std; 


// Function simulating work, to prevent the compiler from optimizing the code away
volatile int sink = 0; // volatile prevents excessive optimization

void do_work(int id) {
    sink = id; // Simple assignment
}

int main() {
    long long N = 1000000; // Number of iterations

    // --- Test Data ---
    vector<int> test_values(N);
    // Fill with random values to test different branches
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(-50, 150); // Range for the int example
    for(long long i = 0; i < N; ++i) {
        test_values[i] = dist(rng);
    }

    // --- IF-ELSE Measurement ---
    auto start_if = chrono::high_resolution_clock::now();

    for (long long i = 0; i < N; ++i) {
        int value = test_values[i]; // Get the next value
        if (value <= 100 && value >= 0) {
            do_work(1); // Work for branch 1
        } else if (value > 100) {
            do_work(2); // Work for branch 2
        } else if (value < 0) {
            do_work(3); // Work for branch 3
        } else {
            // This 'else' block is technically unreachable for int if the above conditions cover all integers.
            // However, if the conditions were different, this would correspond to the DEFAULT case.
            do_work(4); // Work for else branch
        }
    }

    auto end_if = chrono::high_resolution_clock::now();
    auto duration_if = chrono::duration_cast<chrono::milliseconds>(end_if - start_if);

    cout << "If/Else If/Else time: " << duration_if.count() << " ms" << endl;


    // --- Custom SWITCH Measurement ---
    // Reset sink just in case (to ensure compiler doesn't optimize based on previous value)
    sink = 0;

    auto start_switch = chrono::high_resolution_clock::now();

    for (long long i = 0; i < N; ++i) {
        int value = test_values[i]; // Get the same value
        SWITCH(value) {
            CASE(val <= 100 && val >= 0)
                do_work(1); // Same work
            BREAK

            CASE(val > 100)
                do_work(2); // Same work
            BREAK

            CASE(val < 0)
                do_work(3); // Same work
            BREAK

            DEFAULT // Default corresponds to the final 'else' in the if-chain
                 do_work(4); // Same work
            END_DEFAULT

        } END_SWITCH
    }

    auto end_switch = chrono::high_resolution_clock::now();
    auto duration_switch = chrono::duration_cast<chrono::milliseconds>(end_switch - start_switch);

    cout << "Custom Switch time:   " << duration_switch.count() << " ms" << endl;

    return 0;
}
