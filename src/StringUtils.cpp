// StringUtils.cpp
// Purpose: Implementation of shared string helpers.
// Author:  Anish Sandaka

#include "StringUtils.h"

#include <cctype>

namespace StringUtils {

// Helper: true for the whitespace characters we trim (includes '\r').
static bool isSpaceChar(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f';
}

// Removes whitespace from both ends of text.
std::string trim(const std::string& text) {
    std::string::size_type start = 0;
    while (start < text.size() && isSpaceChar(text[start])) {
        ++start;
    }
    std::string::size_type end = text.size();
    while (end > start && isSpaceChar(text[end - 1])) {
        --end;
    }
    return text.substr(start, end - start);
}

// Converts ASCII letters to uppercase.
std::string toUpper(const std::string& text) {
    std::string result = text;
    for (std::string::size_type i = 0; i < result.size(); ++i) {
        result[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(result[i])));
    }
    return result;
}

// Converts ASCII letters to lowercase.
std::string toLower(const std::string& text) {
    std::string result = text;
    for (std::string::size_type i = 0; i < result.size(); ++i) {
        result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}

// Character-by-character comparison ignoring case.
bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (std::string::size_type i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return true;
}

// Splits on every delimiter, keeping empty fields.
std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> fields;
    std::string current;
    for (std::string::size_type i = 0; i < line.size(); ++i) {
        if (line[i] == delimiter) {
            fields.push_back(current);
            current.clear();
        } else {
            current += line[i];
        }
    }
    fields.push_back(current);
    return fields;
}

// Blank lines and lines whose first non-whitespace character is '#'.
bool isBlankOrComment(const std::string& line) {
    std::string trimmed = trim(line);
    return trimmed.empty() || trimmed[0] == '#';
}

} // namespace StringUtils
