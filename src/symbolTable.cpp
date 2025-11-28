#include "symbolTable.h"

void SymbolTable::set(const std::string &name, double value)
{
    table[name] = value;
}

bool SymbolTable::get(const std::string &name, double &outValue) const
{
    auto it = table.find(name);
    if (it == table.end())
        return false;
    outValue = it->second;
    return true;
}