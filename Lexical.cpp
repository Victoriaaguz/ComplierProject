#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <map>

using namespace std;

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    REAL,
    OPERATOR,
    SEPARATOR,
    STRING
};

// Define a struct to represent tokens
struct Token {
    TokenType type;
    string lexeme;
};

unordered_set<string> keywords = {
    "False", "None", "True", "and", "as", "assert", "async", "await",
    "break", "class", "continue", "def", "del", "elif", "else", "except",
    "finally", "for", "from", "global", "if", "import", "in", "is",
    "lambda", "nonlocal", "not", "or", "pass", "raise", "return", "try",
    "while", "with", "yield"};

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
bool isSeparator(char ch) {
     return ch == '(' || ch == ')' || ch == ';' || ch == ':' || ch == ',' || ch == '{' || ch == '}';
}

/*bool isOperatorChar(char ch) {
     return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '<' || ch == '>' || ch == '='|| ch == '=='|| ch == '!' || ch == '=>' || ch == '=< '|| ch == '!=';
}
*/

vector<Token> lexer(const string& line) {
    vector<Token> tokens;
    string currentLexeme;
    TokenType currentTokenType = IDENTIFIER; // Initial type, will be updated as necessary
    bool inStringLiteral = false;
    bool inComment = false; // Flag to track comment status

    for (size_t i = 0; i < line.length(); ++i) {
        char ch = line[i];

        // Check for the start of a comment block
        if (!inStringLiteral && i + 1 < line.length() && ch == '[' && line[i + 1] == '*') {
            inComment = true;
            i++; // Skip the '*' to avoid parsing it again
            continue;
        }

        // Check for the end of a comment block
        if (inComment && i + 1 < line.length() && ch == '*' && line[i + 1] == ']') {
            inComment = false;
            i++; // Skip the ']' to avoid parsing it again
            continue;
        }

        // Skip all characters within a comment
        if (inComment) continue;

        if (isspace(ch)) {
            if (!currentLexeme.empty()) {
                tokens.push_back({currentTokenType, currentLexeme});
                currentLexeme.clear();
            }
        } else if (isSeparator(ch)) {
            if (!currentLexeme.empty()) {
                tokens.push_back({currentTokenType, currentLexeme});
                currentLexeme.clear();
            }
            tokens.push_back({SEPARATOR, string(1, ch)});
        } else if (isOperatorChar(ch)) {
            if (!currentLexeme.empty()) {
                tokens.push_back({currentTokenType, currentLexeme});
                currentLexeme.clear();
            }
            currentLexeme += ch;
            currentTokenType = OPERATOR;
        } else if (ch == '"' || ch == '\'') {
            inStringLiteral = true;
            currentLexeme += ch;
        } else {
            currentLexeme += ch;
            if (isdigit(ch) && currentLexeme.size() == 1) {
                currentTokenType = INTEGER;
            } else if (isalpha(ch) || ch == '_') {
                currentTokenType = IDENTIFIER;
            }
        }
    }

    // Handle the last lexeme if it exists and is not part of a comment
    if (!currentLexeme.empty() && !inComment) {
        tokens.push_back({currentTokenType, currentLexeme});
    }

    return tokens;
}


void updateTokenTypes(vector<Token>& tokens) {
    for (auto& token : tokens) {
        if (token.type == IDENTIFIER) {
            if (keywords.find(token.lexeme) != keywords.end()) {
                token.type = KEYWORD;
            }
        } else if (token.type == INTEGER) {
            if (token.lexeme.find('.') != string::npos) {
                token.type = REAL;
            }
        }
    }
}

int main() {
    ifstream file("T1.txt");
    ofstream outputFile("tokens.txt");

    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    if (!outputFile.is_open()) {
        cerr << "Error opening output file." << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        vector<Token> tokens = lexer(line);
        updateTokenTypes(tokens);
        for (const auto& token : tokens) {
            outputFile << "Token: ";
            switch (token.type) {
                case KEYWORD: outputFile << "KEYWORD"; break;
                case IDENTIFIER: outputFile << "IDENTIFIER"; break;
                case INTEGER: outputFile << "INTEGER"; break;
                case REAL: outputFile << "REAL"; break;
                case OPERATOR: outputFile << "OPERATOR"; break;
                case SEPARATOR: outputFile << "SEPARATOR"; break;
                case STRING: outputFile << "STRING"; break;
            }
            outputFile << ", Lexeme: " << token.lexeme << endl;
        }
    }

    file.close();
    outputFile.close();
    return 0;
}