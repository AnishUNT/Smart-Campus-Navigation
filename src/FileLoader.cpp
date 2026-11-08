// FileLoader.cpp
// Purpose: Implementation of the shared record-file reader.
// Author:  Anish Sandaka

#include "FileLoader.h"
#include "StringUtils.h"

#include <fstream>
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

// Prints a rejection warning in a consistent format.
static void printWarning(const std::string& fileName, int lineNumber, const std::string& reason) {
    std::cout << "Warning: " << fileName << " line " << lineNumber << ": " << reason << std::endl;
}

// Reads the file line by line and applies generic validation before
// handing each record to the module handler.
LoadStats loadRecords(const std::string& path, const std::string& fileName,
                      std::size_t expectedFields, const RecordHandler& handler) {
    LoadStats stats;
    std::ifstream input(path.c_str());
    if (!input.is_open()) {
        std::cout << "Error: could not open " << fileName << " (" << path
                  << "); treating it as empty." << std::endl;
        return stats;
    }
    stats.opened = true;

    std::string line;
    int lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        if (StringUtils::isBlankOrComment(line)) {
            continue;
        }

        std::vector<std::string> fields = StringUtils::split(line, '|');
        for (std::size_t i = 0; i < fields.size(); ++i) {
            fields[i] = StringUtils::trim(fields[i]);
        }

        // Check 1: field count.
        if (fields.size() != expectedFields) {
            std::ostringstream reason;
            reason << "wrong field count (expected " << expectedFields
                   << ", found " << fields.size() << ")";
            printWarning(fileName, lineNumber, reason.str());
            ++stats.rejected;
            continue;
        }

        // Check 2: every field is required.
        bool hasEmpty = false;
        std::size_t emptyIndex = 0;
        for (std::size_t i = 0; i < fields.size(); ++i) {
            if (fields[i].empty()) {
                hasEmpty = true;
                emptyIndex = i;
                break;
            }
        }
        if (hasEmpty) {
            std::ostringstream reason;
            reason << "empty required field (field " << (emptyIndex + 1) << ")";
            printWarning(fileName, lineNumber, reason.str());
            ++stats.rejected;
            continue;
        }

        // Remaining checks are specific to each file.
        std::string reason = handler(fields);
        if (reason.empty()) {
            ++stats.accepted;
        } else {
            printWarning(fileName, lineNumber, reason);
            ++stats.rejected;
        }
    }
    return stats;
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
