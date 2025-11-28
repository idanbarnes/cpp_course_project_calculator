#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include "symbolTable.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: calc inputFileName\n";
        return 1;
    }

    std::string filename = argv[1];
    std::string fileContent = readFile(filename);

    if (fileContent.empty())
    {
        std::cout << "Error: Could not read file or file is empty.\n";
        return 1;
    }

    // Split file content into sessions
    auto sessions = splitSessions(fileContent);

    int sessionIndex = 1;
    const std::string separator(50, '-');

    for (const auto &session : sessions)
    {

        // Skip empty sessions
        if (trim(session).empty())
            continue;

        SymbolTable Symbols; // reset per session
        std::stringstream ss(session);
        std::string line;

        std::vector<std::string> rawLines;
        std::vector<std::string> answers;

        // First, process variable definitions
        while (std::getline(ss, line))
        {

            std::string cleaned = trim(line);

            // skip if empty OR too short to be a real expression
            if (cleaned == "" || cleaned == "\r" || cleaned == "\n")
                continue;

            rawLines.push_back(cleaned);

            // If the line contains "=", it's a variable definition.
            if (cleaned.find('=') != std::string::npos)
            {
                size_t pos = cleaned.find('=');
                std::string varName = trim(cleaned.substr(0, pos));
                std::string varValue = trim(cleaned.substr(pos + 1));

                // Lex and parse the value expression
                try
                {
                    Lexer lex(varValue);
                    Parser parser(lex);
                    auto ast = parser.parseExpression();

                    Evaluator eval(Symbols);
                    double result = eval.evaluate(ast);

                    Symbols.set(varName, result);
                }
                catch (const std::exception &ex)
                {
                    answers.push_back(std::string("Error: ") + ex.what());
                }
            }
            else
            {
                // then, evaluate the expression
                std::string expr = cleaned;

                if (trim(expr).empty())
                    continue;

                try
                {
                    Lexer lex(expr);
                    Parser parser(lex);
                    auto ast = parser.parseExpression();

                    Evaluator eval(Symbols);
                    double result = eval.evaluate(ast);

                    answers.push_back("Answer: " + formatDouble(result));
                }
                catch (const std::exception &ex)
                {
                    answers.push_back(std::string("Error: ") + ex.what());
                }
            }
        }

        // Output results for the input session defined by the user
        std::cout << "Session " << sessionIndex << ":" << std::endl
                  << std::endl;
        for (const auto &l : rawLines)
        {
            std::cout << l << std::endl;
        }
        std::cout << std::endl;

        if (answers.empty())
        {
            std::cout << "Answer: (no expression)" << std::endl;
        }
        else
        {
            for (const auto &ans : answers)
            {
                std::cout << ans << std::endl;
            }
        }

        std::cout << separator << std::endl
                  << std::endl;
        sessionIndex++;
    }

    return 0;
}
