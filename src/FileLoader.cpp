// FileLoader.cpp
// Purpose: Implementation of the shared file-loading helpers.
// Author:  Anish Sandaka

#include "FileLoader.h"
#include "StringUtils.h"

#include <iostream>
#include <sstream>

namespace FileLoader {

// Joins directory and file name, avoiding a doubled '/'.
std::string joinPath(const std::string& directory, const std::string& fileName) {
    if (directory.empty()) {
        return fileName;
    }
    char last = directory[directory.size() - 1];
    if (last == '/' || last == '\\') {
        return directory + fileName;
    }
    return directory + "/" + fileName;
}

// Opens the file, or prints an error if it is missing.
bool openFile(std::ifstream& input, const std::string& path, const std::string& fileName) {
    input.open(path.c_str());
    if (!input.is_open()) {
        std::cout << "Error: could not open " << fileName << " (" << path
                  << "); treating it as empty." << std::endl;
        return false;
    }
    return true;
}

// Reads complete lines with getline and skips blank and comment lines.
bool readRecord(std::ifstream& input, int& lineNumber, std::vector<std::string>& fields) {
    std::string line;
    while (std::getline(input, line)) {
        lineNumber++;
        if (StringUtils::isBlankOrComment(line)) {
            continue;
        }
        fields = StringUtils::split(line, '|');
        for (int i = 0; i < (int)fields.size(); i++) {
            fields[i] = StringUtils::trim(fields[i]);
        }
        return true;
    }
    return false;
}

// Checks the field count first, then looks for an empty field.
std::string checkFields(const std::vector<std::string>& fields, int expectedFields) {
    if ((int)fields.size() != expectedFields) {
        std::ostringstream reason;
        reason << "wrong field count (expected " << expectedFields
               << ", found " << fields.size() << ")";
        return reason.str();
    }
    for (int i = 0; i < (int)fields.size(); i++) {
        if (fields[i].empty()) {
            std::ostringstream reason;
            reason << "empty required field (field " << (i + 1) << ")";
            return reason.str();
        }
    }
    return "";
}

// Adds one to accepted or rejected and prints a warning for rejections.
void countRecord(LoadStats& stats, const std::string& fileName, int lineNumber,
                 const std::string& reason) {
    if (reason.empty()) {
        stats.accepted++;
    } else {
        std::cout << "Warning: " << fileName << " line " << lineNumber << ": "
                  << reason << std::endl;
        stats.rejected++;
    }
}

// Prints "<file>: X accepted, Y rejected" plus optional extra details.
void printSummary(const std::string& fileName, const LoadStats& stats, const std::string& extra) {
    std::cout << fileName << ": " << stats.accepted << " accepted, "
              << stats.rejected << " rejected";
    if (!extra.empty()) {
        std::cout << " " << extra;
    }
    std::cout << std::endl;
}

} // namespace FileLoader
