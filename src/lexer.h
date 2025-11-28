#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <cctype>

// --------------------------------------
// Token Types
// --------------------------------------
enum class TokenType
{
    Number,     // 10, 0x1F, 1100b
    Identifier, // variable names: a, radius, pi
    Function,   // sin, cos
    Operator,   // + - * / ^
    LParen,     // (
    RParen,     // )
    Assign,     // =
    EndOfLine,  // end of session line
    EndOfFile,  // end of file or input
    Unknown
};

// --------------------------------------
// Token Structure
// --------------------------------------
struct Token
{
    TokenType type;
    std::string lexeme; // actual text
};

// --------------------------------------
// Lexer Class Declaration
// --------------------------------------
class Lexer
{
public:
    explicit Lexer(const std::string &input);

    Token getNextToken();
    void reset();

private:
    std::string text;
    size_t pos;

    char peek() const;
    char get();

    Token numberToken();
    Token identifierOrFunction();
};

#endif // LEXER_H
