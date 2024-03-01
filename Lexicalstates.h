#pragma once
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <cassert>

class Lexicalanalysis
{
public:
    // constructor for textfile
    Lexicalanalysis(std::string textfile);

    bool token(std::string &token, std::string &lexme);

private:
    // checks for known  keywords, separator, and operators

#pragma region Boolean knownWords
    bool isAlpha(char ch) { return std::isalpha(ch); }
    bool isDigit(char ch) { return std::isdigit(ch); }
    bool isUnderscore(char ch) { return ch == '_'; }
    bool isPeriod(char ch) { return ch == '.'; }
    bool isSpace(char ch) { return ch == ' '; }
    bool isReturn(char ch) { return ch == '\n'; }
    bool isTab(char ch) { return ch == '\t'; }
    bool isOpenParen(char ch) { return ch == '('; }
    bool isCloseParen(char ch) { return ch == ')'; }
    bool isOpenBracket(char ch) { return ch == '{'; }
    bool iscloseBracket(char ch) { return ch == '}'; }
    bool isPlus(char ch) { return ch == '+'; }
    bool isMinus(char ch) { return ch == '-'; }
    bool isAsterisk(char ch) { return ch == '*'; }
    bool isDivide(char ch) { return ch == '/'; }
    bool isEqualSign(char ch) { return ch == '='; }
    bool isLeftAngle(char ch) { return ch == '<'; }
    bool isRightAngle(char ch) { return ch == '>'; }
    bool isSemiColon(char ch) { return ch == ';'; }
    bool isComma(char ch) { return ch == ','; }
#pragma endregion
    // skip white spaces
    // OpenCom = beginning of comment [*
    // EndCom = end of comment *]
    std::string skipcomments(std::string source, std::string OpenCom, std::string EndCom)
    {
        size_t Open = source.find(OpenCom);
        size_t close = source.find(EndCom);
        std::string returnString{source};
        std::string endingHalf, beginnigHalf;

        while (Open != std::string::npos && close != std::string::npos)
        {
            beginnigHalf = returnString.substr(0, Open);
            endingHalf = returnString.substr(close + 2, source.size());
            returnString = std::string(beginnigHalf + endingHalf);
            Open = returnString.find(OpenCom);
            close = returnString.find(EndCom);
        }
        return returnString;
    };
    enum InputType
    {
        LETTER = 1,
        DIGIT = 2,
        UNDERSCORE = 3,
    };
    InputType getinputtype(char c);

    enum TnputType
    {
        IDENTIFIER,
        INTEGER,
        REAL,
        SEPARATOR,
        OPERATOR,
        NONE };


    #pragma Id state table 
        // states for Id 
        int Idtable[27][4] = {
    // state, Letter, Digit, underscore
            1, 1, 4, 5,
            2, 7, 10, 11,
            3, 14, 15, 17,
            4, 17, 16, 18,
            5, 13, 15, 25, 
            6, 2, 12, 18, 
            7, 1, 7, 16,
            8, 4, 16, 17, 
            9, 5, 23, 12, 
            10, 4, 8, 16, 
            11, 4, 17, 17,
            12, 5, 4, 26, 
            13, 15, 5, 23,
            14, 6, 12, 24,
            15, 5, 25, 18,
            16, 4, 8, 10,
            17, 10, 19, 4,
            18, 15, 24, 10, 
            19, 11, 17, 8, 
            20, 2, 15, 21,
            21, 6, 21, 23, 
            22, 6, 26, 18, 
            23, 5, 9, 13,
            24, 6, 24, 12,
            25, 5, 6, 25,
            26, 0, 0, 0,

        };
    #pragma digit or real 
    // states for integer or/and real 
   /* int intReal[][] = {

    };
    */

};
