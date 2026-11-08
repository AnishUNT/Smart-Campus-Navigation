// FileLoader.h
// Purpose: Generic record-file reader shared by all modules. It reads complete
//          lines with getline, skips blank/comment lines, splits and trims
//          fields, checks field count and empty fields, and passes each
//          remaining record to a module-specific handler. It prints a warning
//          for every rejected record and an error if the file cannot be opened.
// Author:  Anish Sandaka

#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <functional>
#include <string>
#include <vector>

// Counts collected while loading one file.
struct LoadStats {
    int accepted;
    int rejected;
    bool opened;

    LoadStats() : accepted(0), rejected(0), opened(false) {}
};

// A record handler receives the trimmed fields of one record.
// It returns an empty string if the record was accepted, otherwise the
// rejection reason (which the loader prints with filename and line number).
typedef std::function<std::string(const std::vector<std::string>&)> RecordHandler;

namespace FileLoader {

// Joins a directory and file name with a single '/'.
std::string joinPath(const std::string& directory, const std::string& fileName);

// Loads the file at path. fileName is the name shown in messages.
// expectedFields is the exact number of '|'-separated fields required.
LoadStats loadRecords(const std::string& path, const std::string& fileName,
                      std::size_t expectedFields, const RecordHandler& handler);

// Prints the standard load summary line. extra is appended if not empty.
void printSummary(const std::string& fileName, const LoadStats& stats,
                  const std::string& extra = "");

} // namespace FileLoader

#endif
