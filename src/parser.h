#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string>
#include "lexer.h"

// AST Node Base Class
class ASTNode
{
public:
    virtual ~ASTNode() = default;
};

// Number Node
class NumberNode : public ASTNode
{
public:
    std::string raw; // raw number lexeme (e.g., "10", "0x1F", "1100b" )
    explicit NumberNode(const std::string &r) : raw(r) {}
};

// Variable Node
class VariableNode : public ASTNode
{
public:
    std::string name;
    explicit VariableNode(const std::string &n) : name(n) {}
};

// Binary Operator Node
class BinaryOpNode : public ASTNode
{
public:
    char op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(char oper,
                 std::unique_ptr<ASTNode> l,
                 std::unique_ptr<ASTNode> r)
        : op(oper), left(std::move(l)), right(std::move(r)) {}
};

// Unary Function Node (sin, cos only)
class UnaryFunctionNode : public ASTNode
{
public:
    std::string func; // "sin" or "cos" only
    std::unique_ptr<ASTNode> argument;

    UnaryFunctionNode(const std::string &f,
                      std::unique_ptr<ASTNode> arg)
        : func(f), argument(std::move(arg)) {}
};

// Parser Class (recursive descent)
class Parser
{
public:
    explicit Parser(Lexer &lexer);

    std::unique_ptr<ASTNode> parseExpression();

private:
    Lexer &lex;
    Token currentToken;

    void advance();
    void expect(TokenType t);

    // Recursive descent methods
    std::unique_ptr<ASTNode> parseExpressionLevel();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parsePrimary();

    std::unique_ptr<ASTNode> parseFunction();
};

#endif // PARSER_H
