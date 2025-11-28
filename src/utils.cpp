#include "utils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

// Trim whitespace from both ends of a string
std::string trim(const std::string &s)
{
    size_t left = s.find_first_not_of(" \t\n\r");
    if (left == std::string::npos)
        return "";
    size_t right = s.find_last_not_of(" \t\n\r");
    return s.substr(left, right - left + 1);
}

// Split file content into sessions using "----"
std::vector<std::string> splitSessions(const std::string &fileContent)
{
    std::vector<std::string> sessions;
    std::stringstream ss(fileContent);
    std::string line;
    std::string current;

    while (std::getline(ss, line))
    {
        if (trim(line) == "----")
        {
            if (!current.empty())
            {
                sessions.push_back(trim(current));
                current.clear();
            }
        }
        else
        {
            current += line + "\n";
        }
    }
    if (!current.empty())
    {
        sessions.push_back(current);
    }
    return sessions;
}

// Read entire file content into a string
std::string readFile(const std::string &filename)
{
    std::ifstream f(filename);
    if (!f.is_open())
        return "";

    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

// Format double cleanly (removing trailing zeros after decimal point)
std::string formatDouble(double value)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(10) << value;
    std::string s = out.str();

    // Remove trailing zeros
    while (s.find('.') != std::string::npos && s.back() == '0')
    {
        s.pop_back();
    }

    // Remove trailing decimal point
    if (!s.empty() && s.back() == '.')
    {
        s.pop_back();
    }
    return s;
}