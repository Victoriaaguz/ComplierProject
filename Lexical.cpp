#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    REAL,
    OPERATOR,
    SEPARATOR
};
