#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

std::string trim(const std::string &s);

// Split file content into sessions using "----
std::vector<std::string> splitSessions(const std::string &fileContent);

// Read entire file content into a string
std::string readFile(const std::string &filename);

std::string formatDouble(double value);

#endif // UTILS_H