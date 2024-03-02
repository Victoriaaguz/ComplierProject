#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

enum TokenType
{
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    REAL,
    OPERATOR,
    SEPARATOR,
    ILLEGAL,
    STRING
};

// Define a struct to represent tokens
struct Token
{
    TokenType type;
    string lexeme;
};

unordered_set<string> keywords = {
    "False", "None", "True", "and", "as", "assert", "async", "await",
    "break", "class", "continue", "def", "del", "elif", "else", "except",
    "finally", "for", "from", "global", "if", "import", "in", "is",
    "lambda", "nonlocal", "not", "or", "pass", "raise", "return", "try",
    "while", "with", "yield"};

bool isSeparator(char ch)
{
    return ch == '(' || ch == ')' || ch == ';' || ch == ':' || ch == ',' || ch == '{' || ch == '}' || ch == '$';
}

bool isOperatorChar(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '<' || ch == '>' || ch == '=' || ch == '!';
}

bool is_keyword(const string &lexeme)
{
    return keywords.find(lexeme) != keywords.end();
}

bool contains_invalid_characters(const string &lexeme)
{
    return lexeme.find('_') != string::npos || lexeme.find('"') != string::npos;
}

void update_identifier_type(Token &token) {
    if (token.type == IDENTIFIER) {
        if (keywords.find(token.lexeme) != keywords.end()) {
            token.type = KEYWORD;
        } else if (!contains_invalid_characters(token.lexeme)) {
            token.type = IDENTIFIER;
        } else {
            if (token.lexeme.size() > 1 && token.lexeme.front() == '_' && token.lexeme.back() == '_') {
                token.type = ILLEGAL;
            } else {
                token.type = IDENTIFIER;
            }
        }
    }
}

void update_integer_type(Token &token)
{
    if (token.type == INTEGER)
    {
        if (token.lexeme.find('.') != string::npos)
        {
            token.type = REAL;
        }
    }
}
vector<Token> lexer(const string &line)
{
    vector<Token> tokens;
    string currentLexeme;
    TokenType currentTokenType = ILLEGAL; // Initial type, will be updated as necessary
    bool inStringLiteral = false;
    bool inComment = false; // Flag to track comment status

    for (size_t i = 0; i < line.length(); ++i)
    {
        char ch = line[i];

        // Check for the start of a comment block
        if (!inStringLiteral && i + 1 < line.length() && ch == '[' && line[i + 1] == '*')
        {
            inComment = true;
            i++; // Skip the '*' to avoid parsing it again
            continue;
        }

        // Check for the end of a comment block
        if (inComment && i + 1 < line.length() && ch == '*' && line[i + 1] == ']')
        {
            inComment = false;
            i++; // Skip the ']' to avoid parsing it again
            continue;
        }

        // Skip all characters within a comment
        if (inComment)
            continue;

        if (isspace(ch))
        {
            if (!currentLexeme.empty())
            {
                tokens.push_back({currentTokenType, currentLexeme});
                currentLexeme.clear();
            }
        }
        else if (isSeparator(ch))
        {
            if (!currentLexeme.empty())
            {
                tokens.push_back({currentTokenType, currentLexeme});
                currentLexeme.clear();
            }
            tokens.push_back({SEPARATOR, string(1, ch)});
        }
        else if (isOperatorChar(ch))
        {
            if (!currentLexeme.empty())
            {
                tokens.push_back({currentTokenType, currentLexeme});
                currentLexeme.clear();
            }
            currentLexeme += ch;
             // Check if the next character is part of a compound operator
            if (i + 1 < line.length() && isOperatorChar(ch) && line[i + 1] == '=')
            {
                currentLexeme += '=';
                i++; // Skip the '=' character
            }

            currentTokenType = OPERATOR;
        } else if (ch == '"' || ch == '\'')
        {
            currentLexeme += ch;
            currentTokenType = ILLEGAL;
            inStringLiteral = true;
            
        }
        else
        {
            currentLexeme += ch;
            if (isalpha(ch))
            {
                if (isalpha(currentLexeme[0]) || currentLexeme[0] == '_' || isdigit(ch))
                {
                    currentTokenType = IDENTIFIER;
                }
                else
                {
                    currentTokenType = ILLEGAL;
                }
            }
            else if (isdigit(ch))
            {
                if (currentTokenType != REAL && currentLexeme.size() == 1)
                {
                    currentTokenType = INTEGER;
                }
                else if (currentTokenType == INTEGER && ch == '.')
                {
                    currentTokenType = REAL;
                }
                else if (currentTokenType == REAL && ch == '.')
                {
                    // If already a REAL token and another '.' appears, mark it as ILLEGAL
                    currentTokenType = ILLEGAL;
                }
            }
        }
    }

    // Handle the last lexeme if it exists and is not part of a comment
    if (!currentLexeme.empty() && !inComment)
    {
        tokens.push_back({currentTokenType, currentLexeme});
    }

    return tokens;
}

void update_token_types(vector<Token> &tokens)
{
    for (auto &token : tokens)
    {
        if (token.type == TokenType::IDENTIFIER)
        {
            update_identifier_type(token);
        }
        else if (token.type == TokenType::INTEGER)
        {
            update_integer_type(token);
        }
    }
}

int main()
{
    string FileName;
    std::cout << "Enter the input file name: ";
    cin >> FileName;
    ifstream file(FileName);

    if (!file.is_open())
    {
        cerr << "Error opening file." << endl;
        return 1;
    }

    size_t lastDotPos = FileName.find_last_of('.');

    // Create the output file name by extracting the substring before the last dot
    string outputFileName = FileName.substr(0, lastDotPos) + "_output.txt";

    ofstream outputFile(outputFileName);

    if (!outputFile.is_open())
    {
        cerr << "Error opening output file." << endl;
        return 1;
    }

    string line;
    while (getline(file, line))
    {
        vector<Token> tokens = lexer(line);
        update_token_types(tokens);
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
            case ILLEGAL:
                outputFile << "ILLEGAL";
                break;
            case STRING:
                outputFile << "STRING";
                break;
            }
            outputFile << ", Lexeme: " << token.lexeme << endl;
        }
    }

    file.close();
    outputFile.close();
    std::cout << "Output file " << outputFileName << " created successfully." << endl;
    return 0;
}
