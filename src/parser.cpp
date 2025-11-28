#include "parser.h"
#include <stdexcept>
#include <iostream>

// Constructor: Load first token
Parser::Parser(Lexer &lexer)
    : lex(lexer)
{
    currentToken = lex.getNextToken();
}

// Move to next token
void Parser::advance()
{
    currentToken = lex.getNextToken();
}

// Expect a token type and advance
void Parser::expect(TokenType t)
{
    if (currentToken.type != t)
    {
        throw std::runtime_error("Parser error: unexpected token '");
    }
    advance();
}

// Entry point for parsing an expression
std::unique_ptr<ASTNode> Parser::parseExpression()
{
    return parseExpressionLevel();
}

// expression := term ((+|-) term)*
std::unique_ptr<ASTNode> Parser::parseExpressionLevel()
{
    auto node = parseTerm();

    while (currentToken.type == TokenType::Operator &&
           (currentToken.lexeme == "+" || currentToken.lexeme == "-"))
    {
        char op = currentToken.lexeme[0];
        advance();
        auto right = parseTerm();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(right));
    }

    return node;
}

// term := factor ((*|/) factor)*
std::unique_ptr<ASTNode> Parser::parseTerm()
{

    auto node = parseFactor();

    while (currentToken.type == TokenType::Operator &&
           (currentToken.lexeme == "*" || currentToken.lexeme == "/"))
    {
        char op = currentToken.lexeme[0];
        advance();
        auto right = parseFactor();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(right));
    }

    return node;
}

// factor := primary (^ factor)?
// Handle right-associative exponentiation
std::unique_ptr<ASTNode> Parser::parseFactor()
{

    auto node = parsePrimary();

    if (currentToken.type == TokenType::Operator && currentToken.lexeme == "^")
    {
        // char op = currentToken.lexeme[0];
        advance();
        auto right = parseFactor(); // right-associative
        node = std::make_unique<BinaryOpNode>('^', std::move(node), std::move(right));
    }

    return node;
}

// primary := number | identifier | "(" expression ")" | function "(" expression ")"
std::unique_ptr<ASTNode> Parser::parsePrimary()
{

    // Skip empty tokens (should not happen in well-formed input)
    while (currentToken.type == TokenType::EndOfFile ||
           (currentToken.type == TokenType::Unknown && currentToken.lexeme == ""))
    {
        return nullptr;
    }

    // Number
    if (currentToken.type == TokenType::Number)
    {
        std::string text = currentToken.lexeme;
        advance();
        return std::make_unique<NumberNode>(text);
    }

    // variable
    if (currentToken.type == TokenType::Identifier)
    {
        std::string name = currentToken.lexeme;
        advance();
        return std::make_unique<VariableNode>(name);
    }

    // Function call (sin, cos)
    if (currentToken.type == TokenType::Function)
    {
        std::string func = currentToken.lexeme;
        advance(); // get '('

        if (currentToken.type != TokenType::LParen)
        {
            throw std::runtime_error("Parser error: expected '(' after function name");
        }

        advance(); // skip '('
        auto arg = parseExpression();

        if (currentToken.type != TokenType::RParen)
        {
            throw std::runtime_error("Parser error: expected ')' after function argument");
        }
        advance(); // skip ')'
        return std::make_unique<UnaryFunctionNode>(func, std::move(arg));
    }

    // Parentheses
    if (currentToken.type == TokenType::LParen)
    {
        advance(); // skip '('
        auto expr = parseExpression();

        if (currentToken.type != TokenType::RParen)
        {
            throw std::runtime_error("Parser error: expected ')'");
        }
        advance(); // skip ')'
        return expr;
    }
    // If we reach here, it's an error. Don't crush; just return nullptr.
    return nullptr;
}
