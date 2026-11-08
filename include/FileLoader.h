// FileLoader.h
// Purpose: Helper functions used by every module when loading a data file:
//          opening the file, reading the next record line, checking the
//          field count and empty fields, and printing warnings and the
//          load summary.
// Author:  Anish Sandaka

#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <fstream>
#include <string>
#include <vector>

// Counts collected while loading one file.
struct LoadStats {
    int accepted;
    int rejected;

    LoadStats() : accepted(0), rejected(0) {}
};

namespace FileLoader {

// Joins a directory and file name with a single '/'.
std::string joinPath(const std::string& directory, const std::string& fileName);

// Opens path for reading. If it cannot be opened, prints an error naming
// the file and returns false (the caller treats the file as empty).
bool openFile(std::ifstream& input, const std::string& path, const std::string& fileName);

// Reads lines until it finds one that is not blank and not a comment.
// Splits it on '|' and trims every field. lineNumber is updated to the
// line that was read. Returns false at the end of the file.
bool readRecord(std::ifstream& input, int& lineNumber, std::vector<std::string>& fields);

// Checks that there are exactly expectedFields fields and none are empty.
// Returns "" if the record is OK, otherwise the rejection reason.
std::string checkFields(const std::vector<std::string>& fields, int expectedFields);

// Updates the counts for one record. If reason is not empty, the record
// was rejected and a warning with the file name and line number is printed.
void countRecord(LoadStats& stats, const std::string& fileName, int lineNumber,
                 const std::string& reason);

// Prints the standard load summary line. extra is appended if not empty.
void printSummary(const std::string& fileName, const LoadStats& stats,
                  const std::string& extra = "");

} // namespace FileLoader

#endif
