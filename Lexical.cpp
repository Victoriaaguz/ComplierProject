#include <iostream>
#include <unordered_map>
#include <cctype>

using namespace std;

enum State {
    START,
    POSITIVE,
    NEGATIVE,
    END
};

// Define the transitions for each state and input symbol
unordered_map<State, unordered_map<char, State>> fsmTable = {
    {START,     {{'0', POSITIVE}, {'1', POSITIVE}, {'2', POSITIVE}, {'3', POSITIVE},
                 {'4', POSITIVE}, {'5', POSITIVE}, {'6', POSITIVE}, {'7', POSITIVE},
                 {'8', POSITIVE}, {'9', POSITIVE}, {'+', NEGATIVE}, {'-', NEGATIVE}}},
    {POSITIVE,  {{'0', POSITIVE}, {'1', POSITIVE}, {'2', POSITIVE}, {'3', POSITIVE},
                 {'4', POSITIVE}, {'5', POSITIVE}, {'6', POSITIVE}, {'7', POSITIVE},
                 {'8', POSITIVE}, {'9', POSITIVE}}},
    {NEGATIVE,  {{'0', POSITIVE}, {'1', POSITIVE}, {'2', POSITIVE}, {'3', POSITIVE},
                 {'4', POSITIVE}, {'5', POSITIVE}, {'6', POSITIVE}, {'7', POSITIVE},
                 {'8', POSITIVE}, {'9', POSITIVE}}}
};

State transition(State currentState, char input) {
    if (fsmTable[currentState].find(input) != fsmTable[currentState].end()) {
        return fsmTable[currentState][input];
    } else {
        return END; // Invalid input
    }
}

int main() {
    string input;
    cout << "Enter a sequence of characters: ";
    cin >> input;

    State currentState = START;

    for (char c : input) {
        currentState = transition(currentState, c);
        if (currentState == END) {
            break; // Invalid input detected
        }
    }

    // Check final state
    if (currentState == POSITIVE || currentState == NEGATIVE) {
        cout << "Valid integer" << endl;
    } else {
        cout << "Invalid integer" << endl;
    }

    return 0;
}