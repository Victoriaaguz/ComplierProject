#include <iostream>
#include <fstream>
#include <vector>
#include <map>

// Define token types
enum TokenType {
    KEYWORD, IDENTIFIER, INTEGER, REAL, OPERATOR, ILLEGAL
};

// Structure to represent a token
struct Token {
    TokenType type;
    std::string value;
};

// Function to check if a character is whitespace
bool isWhitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

// Function to check if a character is a digit
bool isDigit(char c) {
    return (c >= '0' && c <= '9');
}

// Function to check if a character is a letter
bool isLetter(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// Function to perform lexical analysis
std::vector<Token> lexer(const std::string& input_code) {
    std::vector<Token> tokens;
    std::string remaining_code = input_code;

    // Loop through the input code
    while (!remaining_code.empty()) {
        // Step 1: Skip Whitespaces
        while (!remaining_code.empty() && isWhitespace(remaining_code[0])) {
            remaining_code = remaining_code.substr(1);
        }

        // Step 2: Try matching each token type
        if (isLetter(remaining_code[0])) {
            // Match Identifier or Keyword
            std::string identifier;
            while (!remaining_code.empty() && (isLetter(remaining_code[0]) || isDigit(remaining_code[0]))) {
                identifier += remaining_code[0];
                remaining_code = remaining_code.substr(1);
            }

            // Check if the identifier is a keyword
            std::map<std::string, TokenType> keywordMap;
            keywordMap["function"] = KEYWORD;
            keywordMap["int"] = KEYWORD;
            keywordMap["boolean"] = KEYWORD;
            keywordMap["real"] = KEYWORD;
            keywordMap["if"] = KEYWORD;
            keywordMap["else"] = KEYWORD;
            keywordMap["endif"] = KEYWORD;
            keywordMap["while"] = KEYWORD;
            keywordMap["return"] = KEYWORD;
            keywordMap["scan"] = KEYWORD;
            keywordMap["print"] = KEYWORD;
            keywordMap["true"] = KEYWORD;
            keywordMap["false"] = KEYWORD;

            std::map<std::string, TokenType>::iterator keywordType = keywordMap.find(identifier);
            if (keywordType != keywordMap.end()) {
                tokens.push_back(Token{keywordType->second, identifier});
            } else {
                tokens.push_back(Token{IDENTIFIER, identifier});
            }
        } else if (isDigit(remaining_code[0])) {
            // Match Integer or Real
            std::string number;
            while (!remaining_code.empty() && (isDigit(remaining_code[0]) || remaining_code[0] == '.')) {
                number += remaining_code[0];
                remaining_code = remaining_code.substr(1);
            }

            if (number.find('.') != std::string::npos) {
                tokens.push_back(Token{REAL, number});
            } else {
                tokens.push_back(Token{INTEGER, number});
            }
        } else {
            // Match Operator or Illegal
            std::string symbol(1, remaining_code[0]);
            remaining_code = remaining_code.substr(1);

            std::map<std::string, TokenType> symbolMap;
            symbolMap["=="] = OPERATOR;
            symbolMap["!="] = OPERATOR;
            symbolMap[">"] = OPERATOR;
            symbolMap["<"] = OPERATOR;
            symbolMap["<="] = OPERATOR;
            symbolMap[">="] = OPERATOR;
            symbolMap["+"] = OPERATOR;
            symbolMap["-"] = OPERATOR;
            symbolMap["*"] = OPERATOR;
            symbolMap["/"] = OPERATOR;

            std::map<std::string, TokenType>::iterator symbolType = symbolMap.find(symbol);
            if (symbolType != symbolMap.end()) {
                tokens.push_back(Token{symbolType->second, symbol});
            } else {
                tokens.push_back(Token{ILLEGAL, symbol});
            }
        }
    }

    return tokens;
}

// Function to display tokens
void displayTokens(const std::vector<Token>& tokens) {
    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];
        std::cout << "Token Type: " << token.type << ", Value: " << token.value << std::endl;
    }
}

// Function to write tokens to an output file
void writeTokensToFile(const std::vector<Token>& tokens, const std::string& output_file) {
    std::ofstream output(output_file);
    if (!output.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    for (const Token& token : tokens) {
        output << "Token Type: " << token.type << ", Value: " << token.value << std::endl;
    }

    output.close();
}

int main() {
    // Read code from file
    std::ifstream input_file("input_code.txt");
    if (!input_file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    // Read the entire file content into a string
    std::string source_code((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

    // Close the file
    input_file.close();

    // Perform lexical analysis
    std::vector<Token> tokens = lexer(source_code);

    // Display the tokens
    displayTokens(tokens);

    // Write tokens to an output file
    writeTokensToFile(tokens, "output_tokens.txt");

    return 0;
}
