#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <map>

class SymbolTable
{
public:
    // Store or update a variable
    void set(const std::string &name, double value);

    // Retrieve variable value, returns true if found
    bool get(const std::string &name, double &outValue) const;

private:
    std::map<std::string, double> table;
};

#endif // SYMBOL_TABLE_H
