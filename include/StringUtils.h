// StringUtils.h
// Purpose: Shared string helpers (trim, case conversion, case-insensitive
//          comparison, field splitting, comment detection) used by every module.
// Author:  Anish Sandaka

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

namespace StringUtils {

// Removes leading and trailing whitespace (space, tab, \r, \n, \v, \f).
// Internal spaces are preserved.
std::string trim(const std::string& text);

// Returns a copy of text with ASCII letters converted to uppercase.
std::string toUpper(const std::string& text);

// Returns a copy of text with ASCII letters converted to lowercase.
std::string toLower(const std::string& text);

// Compares two strings ignoring ASCII letter case (no trimming is done).
bool equalsIgnoreCase(const std::string& a, const std::string& b);

// Splits line on delimiter. Empty fields are kept, so "A|" gives {"A", ""}.
std::vector<std::string> split(const std::string& line, char delimiter);

// True if the line is empty/whitespace only, or its first non-whitespace
// character is '#'.
bool isBlankOrComment(const std::string& line);

} // namespace StringUtils

#endif
