#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "parser.h"
#include "symbolTable.h"
#include <memory>

class Evaluator
{
public:
    explicit Evaluator(SymbolTable &st);

    // Evaluate any AST node
    double evaluate(const std::unique_ptr<ASTNode> &node);

    // Convert raw number text (binary, hex, decimal)
    double convertNumber(const std::string &raw) const;

private:
    SymbolTable &symbols;

    double evalNumber(const NumberNode *n) const;
    double evalVariable(const VariableNode *v) const;
    double evalBinary(const BinaryOpNode *b);
    double evalFunction(const UnaryFunctionNode *f);
};

#endif // EVALUATOR_H