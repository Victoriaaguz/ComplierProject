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
    int line;
};

unordered_set<string> keywords = {
    "function", "if", "else", "while", "return", "print", "i", "B", "r"};

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

void update_identifier_type(Token &token)
{
    if (token.type == IDENTIFIER)
    {
        if (keywords.find(token.lexeme) != keywords.end())
        {
            token.type = KEYWORD;
        }
        else if (!contains_invalid_characters(token.lexeme))
        {
            token.type = IDENTIFIER;
        }
        else
        {
            if (token.lexeme.size() > 1 && token.lexeme.front() == '_' && token.lexeme.back() == '_')
            {
                token.type = ILLEGAL;
            }
            else
            {
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

vector<Token> lexer(const string &line, int lineNum)
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
                tokens.push_back({currentTokenType, currentLexeme, lineNum});
                currentLexeme.clear();
            }
        }
        else if (isSeparator(ch))
        {
            if (!currentLexeme.empty())
            {
                tokens.push_back({currentTokenType, currentLexeme, lineNum});
                currentLexeme.clear();
            }
            tokens.push_back({SEPARATOR, string(1, ch), lineNum});
        }
        else if (isOperatorChar(ch))
        {
            if (!currentLexeme.empty())
            {
                tokens.push_back({currentTokenType, currentLexeme, lineNum});
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
        }
        else if (ch == '"' || ch == '\'')
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
        tokens.push_back({currentTokenType, currentLexeme, lineNum});
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

class Parser
{
public:
    Parser(const vector<Token> &tokens) : tokenStream(tokens), pos(0) {}

    void parse()
    {
        while (pos < tokenStream.size())
        {
            parseStatement();
        }
        cout << "Parsing successful!" << endl;
    }

private:
    vector<Token> tokenStream;
    size_t pos;

    Token peek()
    {
        return pos < tokenStream.size() ? tokenStream[pos] : Token{SEPARATOR, "", -1};
    }

    void consume()
    {
        if (pos < tokenStream.size())
        {
            pos++;
        }
    }

    void match(TokenType expected, const string &productionRule)
    {
        if (peek().type == expected)
        {
            cout << "Token: " << tokenTypeToString(peek().type) << "\tLexeme: " << peek().lexeme << endl;
            cout << "\t" << productionRule << endl;
            consume();
        }
        else
        {
            throw runtime_error("Unexpected token.");
        }
    }

    string tokenTypeToString(TokenType type)
    {
        switch (type)
        {
        case KEYWORD:
            return "Keyword";
        case IDENTIFIER:
            return "Identifier";
        case INTEGER:
            return "Integer";
        case REAL:
            return "Real";
        case OPERATOR:
            return "Operator";
        case SEPARATOR:
            return "Separator";
        case ILLEGAL:
            return "Illegal";
        case STRING:
            return "String";
        default:
            return "Unknown";
        }
    }
    void parseIdentifier() {
    if (peek().type == IDENTIFIER) {
        // Consume the IDENTIFIER token
        consume();
    } else {
        // If the next token is not an identifier, raise an error
        error("Expected an identifier.");
    }
}

    void parseStatement()
    {
        switch (peek().type)
        {
        case IDENTIFIER:
            parseAssign();
            break;
        default:
            throw runtime_error("Unexpected token.");
        }
    }

    void parseStatement() {
        switch (peek().type) {
        case IDENTIFIER:
            parseAssign();
            break;
        default:
            throw runtime_error("Unexpected token.");
        }
    }

    void parseAssign()
    {
        match(IDENTIFIER, "<Statement> -> <Assign>");
        match(OPERATOR, "<Assign> -> <Identifier>  = <Expression> ;");
        parseExpression();
        match(SEPARATOR, "<Assign> -> <Identifier>  = <Expression> ;");
    }

    void parseExpression()
    {
        parseTerm();
        parseExpressionPrime();
    }

    void parseExpressionPrime()
    {
        switch (peek().type)
        {
        case OPERATOR:
            match(OPERATOR, "<Expression Prime> -> + <Term> <Expression Prime>");
            parseTerm();
            parseExpressionPrime();
            break;
        default:
            break; // ε (empty)
        }
    }

    void parseTerm()
    {
        parseFactor();
        parseTermPrime();
    }

    void parseTermPrime()
    {
        switch (peek().type)
        {
        case OPERATOR:
            match(OPERATOR, "<Term Prime> -> ε");
            parseFactor();
            break;
        default:
            break; // ε (empty)
        }
    }

    void parseFactor()
    {
        match(IDENTIFIER, "<Factor> -> <Identifier>");
    }
    void parseDeclaration()
    {
        parseQualifier();
        parseIDs();
        if (!(peek().type == SEPARATOR && peek().lexeme == ";"))
        {
            throw runtime_error("Expected ';' after declaration.");
        }
        consume(); // Consume the ';'
    }

    void parseQualifier()
    {
        if (!(peek().type == KEYWORD && (peek().lexeme == "integer" || peek().lexeme == "boolean" || peek().lexeme == "real")))
        {
            throw runtime_error("Expected a qualifier keyword (integer, boolean, real).");
        }
        consume(); // Consume the qualifier keyword
    }

    void parseIDs()
    {
        do
        {
            if (peek().type != IDENTIFIER)
            {
                throw runtime_error("Expected an identifier.");
            }
            consume(); // Consume the identifier
        } while (peek().type == SEPARATOR && peek().lexeme == ",");
    }
    
    
     void parseIf() {
    match(KEYWORD, "<if> -> if");
    if (peek().lexeme == "(") {
        consume(); // Consume the '(' token
        parseCondition();
        if (peek().lexeme == ")") {
            consume(); // Consume the ')' token
            parseStatement();
            if (peek().lexeme == "else") {
                consume(); // Consume the 'else' token
                parseStatement();
            }
        } else {
            throw runtime_error("Expected ')' after condition in if statement.");
        }
    } else {
        throw runtime_error("Expected '(' after 'if' keyword.");
    }
    match(KEYWORD, "<Prime if> -> endif | else");
    if (peek().lexeme == "endif") {
        consume(); // Consume the 'endif' token
    }
}


    void parseReturn() {
    match(KEYWORD, "<Return> -> return");
    if (peek().lexeme != ";") {
        parseExpression();
    }
    match(SEPARATOR, "<Return> -> return;");
}


    void parsePrint() {
    match(KEYWORD, "<Print> -> print");
    parseExpression();
    match(SEPARATOR, "<Print> -> print <Expression> ;");
}


    void parseScan() {
    match(KEYWORD, "<Scan> -> scan");
    parseIdentifier();
    match(SEPARATOR, "<Scan> -> scan <Identifier> ;");
}


    void parseWhile() {
    match(KEYWORD, "<While> -> while");
    match(SEPARATOR, "<While> -> while ( <Condition> )");
    parseCondition();
    match(SEPARATOR, "<While> -> while ( <Condition> ) <Statement>");
    parseStatement();
}


    void error(const string& message) {
        throw runtime_error(message);
    }
//R11 (1)
    void parseDL() {
    parseDeclaration();
    parseDLPrime();
}
// R11 (2)
void parseDLPrime() {
    if (peek().type != SEPARATOR || peek().lexeme != ";") {
        parseDL();
    }
    else{
        return; // empty
    }
}
// R13 (1)
void parseIDS() {
    parseIdentifier();
    parseIDSPrime();
}
//R13 (2)
void parseIDSPrime() {
    if (peek().type == IDENTIFIER) {
        parseIDS();
    }
    else{
        return; // empty
    }
}
// R14 (1)
void parseStatementList() {
    parseStatement();
    parseStatementListPrime();
}
// R14 (2)
void parseStatementListPrime() {
    // Check if there are more statements to parse
    if (peek().type != SEPARATOR || peek().lexeme != "}") {
        parseStatement(); // Parse the next statement
        parseStatementListPrime(); // Continue parsing statements recursively
    }
    // If the next token is a '}', then the rule is satisfied and we stop parsing
    else{
        return; // Empty
    }
    // This corresponds to the empty case (epsilon) in the grammar
}
// helper for R18
void parseRelationalOperator() {
    TokenType type = peek().type;
    if (type == OPERATOR && (peek().lexeme == "<" || peek().lexeme == ">" || peek().lexeme == "<=" || peek().lexeme == ">=" || peek().lexeme == "==" || peek().lexeme == "!=")) {
        consume();
    } else {
        error("Expected a relational operator.");
    }
}
//helper for R18
void parseCondition() {
    parseExpression();
    parseRelationalOperator();
    parseExpression();
}
//R18 (1)
void parseIf() {
    match(KEYWORD, "<if> -> if");
    match(SEPARATOR, "<if> -> (");
    parseCondition();
    match(SEPARATOR, "<if> -> )");
    parseStatement();
    parsePrimeIf();
}
//R18 (2)
void parsePrimeIf() {
    if (peek().lexeme == "endif") {
        match(KEYWORD, "<Prime if> -> endif");
    } else if (peek().lexeme == "else") {
        match(KEYWORD, "<Prime if> -> else");
        parseStatement();
        match(KEYWORD, "<Prime if> -> endif");
    } else {
        throw runtime_error("Expected 'endif' or 'else' after if statement.");
    }
}
//R19 (1)
void parseReturn() {
    if (peek().lexeme == "return") {
        consume(); // Consume "return"
        parseReturnPrime();
    } else {
        error("Expected 'return' keyword.");
    }
}
// R19 (2)
void parseReturnPrime() {
    if (peek().type == SEPARATOR && peek().lexeme == ";") {
        consume(); // Consume ';'
    } else {
        parseExpression();
        if (peek().type == SEPARATOR && peek().lexeme == ";") {
            consume(); // Consume ';'
        } else {
            error("Expected ';' after return statement or an expression.");
        }
    }
}
//R24
void parseCondition() {
    parseExpression();
    parseRelationalOperator();
    parseExpression();
}
//R25
void parseRelationalOperator() {
    if (peek().lexeme == "==" || peek().lexeme == "!=" || peek().lexeme == ">" ||
        peek().lexeme == "<" || peek().lexeme == "<=" || peek().lexeme == ">=") {
        consume();
    } else {
        error("Expected a relational operator.");
    }
}

//R26 (1)
void parseExpression() {
    parseTerm();
    parseExpressionPrime();
}
//R26 (2)
void parseExpressionPrime() {
    if (peek().lexeme == "+" || peek().lexeme == "-") {
        consume();
        parseTerm();
        parseExpressionPrime();
    } else {
        return; // ε (empty) case
    }
}
//R27 (1)
void parseTerm() {
    parseFactor();
    parseTermPrime();
}
//R27(2)
void parseTermPrime() {
    if (peek().lexeme == "*" || peek().lexeme == "/") {
        consume();
        parseFactor();
        parseTermPrime();
    } else {
        return;// ε (empty) case
    }
}
//helper for R28
void parsePrimary() {
    if (peek().type == IDENTIFIER) {
        parseIdentifier();
    } else if (peek().type == INTEGER || peek().type == REAL) {
        consume(); // Consume the INTEGER or REAL token
    } else if (peek().lexeme == "(") {
        consume(); // Consume the '(' token
        parseExpression();
        if (peek().lexeme == ")") {
            consume(); // Consume the ')' token
        } else {
            throw runtime_error("Expected ')' after expression.");
        }
    } else {
        throw runtime_error("Expected identifier, integer, real, or '('.");
    }
}

//R28
void parseFactor() {
    if (peek().lexeme == "-") {
        consume(); // Consume the '-'
        parsePrimary();
    } else {
        parsePrimary();
    }
}
//R29
void parsePrimary() {
    if (peek().type == IDENTIFIER) {
        parseIdentifier();
        if (peek().lexeme == "(") {
            consume(); // Consume the '(' token
            parseIDs();
            if (peek().lexeme == ")") {
                consume(); // Consume the ')' token
            } else {
                throw runtime_error("Expected ')' after identifier list.");
            }
        }
    } else if (peek().type == INTEGER || peek().type == REAL) {
        consume(); // Consume the INTEGER or REAL token
    } else if (peek().lexeme == "(") {
        consume(); // Consume the '(' token
        parseExpression();
        if (peek().lexeme == ")") {
            consume(); // Consume the ')' token
        } else {
            throw runtime_error("Expected ')' after expression.");
        }
    } else if (peek().lexeme == "true" || peek().lexeme == "false") {
        consume(); // Consume the 'true' or 'false' token
    } else {
        throw runtime_error("Expected identifier, integer, real, '(', 'true', or 'false'.");
    }
}


};

int main()
{
    string FileName;
    cout << "Enter the input file name: ";
    cin >> FileName;
    ifstream file(FileName);
    if (!file.is_open())
    {
        cerr << "Error opening file." << endl;
        return 1;
    }

    vector<string> lines;
    string line;
    int lineNum = 1;
    while (getline(file, line))
    {
        lines.push_back(line);
        lineNum++;
    }
    file.close();

    vector<Token> tokens;
    for (int i = 0; i < lines.size(); ++i)
    {
        auto lineTokens = lexer(lines[i], i + 1);
        tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());
    }
    update_token_types(tokens);

    try
    {
        Parser parser(tokens);
        parser.parse();
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
