#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

// Define token types
enum TokenType
{
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    REAL,
    OPERATOR,
    SEPARATOR,
    STRING,
    ILLEGAL
};

// Structure to represent a token
struct Token
{
    TokenType type;
    std::string lexeme;
};

// Function to check if a character is whitespace
bool isWhitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

// Function to check if a character is a digit
bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

// Function to check if a character is a letter
bool isLetter(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// Function to perform lexical analysis
std::vector<Token> lexer(const std::string &input_code)
{
    std::vector<Token> tokens;
    std::string remaining_code = input_code;

    // Loop through the input code
    while (!remaining_code.empty())
    {
        // Step 1: Skip Whitespaces

        remaining_code = remaining_code.substr(1);

        // Step 2: Try matching each token type
        if (!remaining_code.empty())
        {
            // Match Identifier or Keyword
            std::string identifier;
            if (!isLetter(remaining_code[0]))
            {
                identifier += remaining_code[0];
                remaining_code = remaining_code.substr(1);
            }else if (isDigit(remaining_code[0])){

            }
            else {}

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
            if (keywordType != keywordMap.end())
            {
                tokens.push_back(Token{keywordType->second, identifier});
            }
            else
            {
                tokens.push_back(Token{IDENTIFIER, identifier});
            }
        }
        else if (isDigit(remaining_code[0]))
        {
            // Match Integer or Real
            std::string number;
            while (!remaining_code.empty() && (isDigit(remaining_code[0]) || remaining_code[0] == '.'))
            {
                number += remaining_code[0];
                remaining_code = remaining_code.substr(1);
            }

            if (number.find('.') != std::string::npos)
            {
                tokens.push_back(Token{REAL, number});
            }
            else
            {
                tokens.push_back(Token{INTEGER, number});
            }
        }
        else
        {
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
            if (symbolType != symbolMap.end())
            {
                tokens.push_back(Token{symbolType->second, symbol});
            }
            else
            {
                tokens.push_back(Token{ILLEGAL, symbol});
            }
        }
    }

    return tokens;
}

// Function to display tokens
void displayTokens(const std::vector<Token> &tokens)
{
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        const Token &token = tokens[i];
        std::cout << "Token Type: " << token.type << ", Value: " << token.lexeme << std::endl;
    }
}

// Function to write tokens to an output file
void writeTokensToFile(const std::vector<Token> &tokens, const std::string &output_file)
{
    std::ofstream output(output_file);
    if (!output.is_open())
    {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    for (const Token &token : tokens)
    {
        output << "Token Type: " << token.type << ", Value: " << token.lexeme << std::endl;
    }

    output.close();
}

int main()
{
    std::ifstream file("T1.txt");
    std::ofstream outputFile("tokens.txt");

    if (!file.is_open())
    {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    if (!outputFile.is_open())
    {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    std::string line;
    while (getline(file, line))
    {
        std::vector<Token> tokens = lexer(line);
        writeTokensToFile(tokens, "output1.txt");
        for (const auto &token : tokens)
        {
            outputFile << "Token: ";
            switch (token.type)
            {
            case KEYWORD:
                outputFile << "KEYWORD";
                break;
            case IDENTIFIER:
                outputFile << "IDENTIFIER";
                break;
            case INTEGER:
                outputFile << "INTEGER";
                break;
            case REAL:
                outputFile << "REAL";
                break;
            case OPERATOR:
                outputFile << "OPERATOR";
                break;
            case SEPARATOR:
                outputFile << "SEPARATOR";
                break;
            case STRING:
                outputFile << "STRING";
                break;
            }
            outputFile << ", Lexeme: " << token.lexeme << std::endl;
        }
    }

    file.close();
    outputFile.close();
    return 0;
}