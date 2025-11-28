#include "evaluator.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

// Constructor
Evaluator::Evaluator(SymbolTable &st)
    : symbols(st) {}

// Main evaluation function
double Evaluator::evaluate(const std::unique_ptr<ASTNode> &node)
{
    if (!node)
    {
        throw std::runtime_error("Null AST node");
    }

    if (auto n = dynamic_cast<NumberNode *>(node.get()))
    {
        return evalNumber(n);
    };

    if (auto v = dynamic_cast<VariableNode *>(node.get()))
    {
        return evalVariable(v);
    };

    if (auto b = dynamic_cast<BinaryOpNode *>(node.get()))
    {
        return evalBinary(b);
    };

    if (auto f = dynamic_cast<UnaryFunctionNode *>(node.get()))
    {
        return evalFunction(f);
    };

    throw std::runtime_error("Unknown AST node type");
}

// Convert raw number text (binary, hex, decimal)
double Evaluator::convertNumber(const std::string &raw) const
{

    // Hexadecimal: 0xFF
    if (raw.size() > 2 && raw[0] == '0' && (raw[1] == 'x' || raw[1] == 'X'))
    {
        return static_cast<double>(std::stoul(raw, nullptr, 16));
    }

    // Binary with 0b prefix
    if (raw.size() > 2 && raw[0] == '0' && (raw[1] == 'b' || raw[1] == 'B'))
    {
        int value = 0;
        for (char c : raw.substr(2))
        {
            value = value * 2 + (c - '0');
        }
        return static_cast<double>(value);
    }

    // Binary: ends with 'b'
    if (raw.size() > 1 && (raw.back() == 'b' || raw.back() == 'B'))
    {
        std::string digits = raw.substr(0, raw.size() - 1);
        int value = 0;
        for (char c : digits)
        {
            value = value * 2 + (c - '0');
        }
        return static_cast<double>(value);
    }

    // Decimal fallback
    return std::stod(raw);
}

// Evaluate number
double Evaluator::evalNumber(const NumberNode *n) const
{
    // Directly return the stored value
    return convertNumber(n->raw);
}

// Evaluate variable
double Evaluator::evalVariable(const VariableNode *v) const
{
    double value;
    if (!symbols.get(v->name, value))
    {
        throw std::runtime_error("Undefined variable: " + v->name);
    }
    return value;
}

// Evaluate binary operation
double Evaluator::evalBinary(const BinaryOpNode *b)
{
    double left = evaluate(b->left);
    double right = evaluate(b->right);

    switch (b->op)
    {
    case '+':
        return left + right;
    case '-':
        return left - right;
    case '*':
        return left * right;
    case '/':
        if (right == 0)
        {
            throw std::runtime_error("Division by zero");
        }
        return left / right;
    case '^':
        return std::pow(left, right);
    default:
        throw std::runtime_error(std::string("Unknown binary operator: ") + b->op);
    }
}

// Evaluate unary function (sin, cos)
double Evaluator::evalFunction(const UnaryFunctionNode *f)
{
    double arg = evaluate(f->argument);

    if (f->func == "sin")
    {
        return std::sin(arg);
    }
    else if (f->func == "cos")
    {
        return std::cos(arg);
    }
    else
    {
        throw std::runtime_error("Unknown function: " + f->func);
    }
}
