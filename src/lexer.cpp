#include "lexer.h"
#include <iostream>
#include <cctype>

// Constructor
Lexer::Lexer(const std::string &input)
    : text(input), pos(0) {}

// Reset lexer to start scanning from beginning
void Lexer::reset()
{
    pos = 0;
}

// Look at current character without consuming it
char Lexer::peek() const
{
    if (pos >= text.length())
        return '\0';
    return text[pos];
}

// Get current character and move forward
char Lexer::get()
{
    if (pos >= text.length())
        return '\0';
    return text[pos++];
}

// --------------------------------------
// Main tokenizing function
Token Lexer::getNextToken()
{
    // Skip whitespace
    while (isspace(peek()))
        get();

    char c = peek();
    if (c == '\0')
        return Token{TokenType::EndOfFile, ""};

    // Parentheses
    if (c == '(')
    {
        get();
        return Token{TokenType::LParen, "("};
    }
    if (c == ')')
    {
        get();
        return Token{TokenType::RParen, ")"};
    }

    // Assignment
    if (c == '=')
    {
        get();
        return Token{TokenType::Assign, "="};
    }

    // Operators
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
    {
        get();
        std::string s(1, c);
        return Token{TokenType::Operator, s};
    }

    // Numbers literal
    //(decimal, hex 0x, binary ending in b)

    if (isdigit(c) || c == '0')
        return numberToken();

    // Identifiers or Functions
    //  (sin and cos only)
    if (isalpha(c))
        return identifierOrFunction();

    // Unknown character
    get();
    return Token{TokenType::Unknown, std::string(1, c)};
}
// Parse number token:
// - decimal: 123, 45.67
// - hex: 0x1F, 0X2A
// - binary ending in b: 1101b, 101b
Token Lexer::numberToken()
{
    std::string value;

    // Hexadecimal?
    if (peek() == '0' && (pos + 1 < text.size()) && (text[pos + 1] == 'x' || text[pos + 1] == 'X'))
    {
        value += get(); // '0'
        value += get(); // 'x'
        while (isxdigit(peek()))
        {
            value += get();
        }
        return Token{TokenType::Number, value};
    }

    // Binary with 0b/0B prefix (tolerated for robustness)
    if (peek() == '0' && (pos + 1 < text.size()) && (text[pos + 1] == 'b' || text[pos + 1] == 'B'))
    {
        value += get(); // '0'
        value += get(); // 'b'
        while (peek() == '0' || peek() == '1')
        {
            value += get();
        }
        return Token{TokenType::Number, value};
    }

    // Read digits (decimal or binary before checking binary suffix)
    while (isdigit(peek()))
    {
        value += get();
    }

    // Optional decimal part
    bool hasDot = false;
    if (peek() == '.')
    {
        hasDot = true;
        value += get(); // '.'
        while (isdigit(peek()))
        {
            value += get();
        }
    }

    // Binary ends with 'b'
    if (!hasDot && (peek() == 'b' || peek() == 'B') && !value.empty())
    {
        value += get(); // append 'b'
        return Token{TokenType::Number, value};
    }

    // Decimal number
    return Token{TokenType::Number, value};
}

// Parse identifier or function token
// - functions: sin, cos
Token Lexer::identifierOrFunction()
{
    std::string name;

    while (isalpha(peek()))
    {
        name += get();
    }

    // Check if it's a function
    if (name == "sin" || name == "cos")
    {
        return Token{TokenType::Function, name};
    }

    return Token{TokenType::Identifier, name};
}
