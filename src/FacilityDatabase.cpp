// FacilityDatabase.cpp
// Purpose: Loading, display, and ID search for facilities.
// Author:  Anish Sandaka

#include "FacilityDatabase.h"
#include "StringUtils.h"

#include <iostream>
#include <sstream>

// Starts with an empty list and an empty hash table.
FacilityDatabase::FacilityDatabase() : table(HashTable::DEFAULT_CAPACITY) {}

// Reads facilities.txt (ID|Name|Building|Category).
LoadStats FacilityDatabase::loadFromFile(const std::string& path, const std::string& fileName,
                                         const CampusGraph& graph) {
    LoadStats stats;
    std::ifstream input;
    if (!FileLoader::openFile(input, path, fileName)) {
        return stats;
    }
    std::vector<std::string> fields;
    int lineNumber = 0;
    while (FileLoader::readRecord(input, lineNumber, fields)) {
        std::string reason = FileLoader::checkFields(fields, 4);
        if (reason.empty()) {
            reason = addFacilityRecord(fields, graph);
        }
        FileLoader::countRecord(stats, fileName, lineNumber, reason);
    }
    return stats;
}

// Checks one record in this order: ID format, unknown building, duplicate ID.
// Adds it if it is OK. Returns "" if accepted, otherwise the reason.
std::string FacilityDatabase::addFacilityRecord(const std::vector<std::string>& fields,
                                                const CampusGraph& graph) {
    std::string id = Facility::normalizeId(fields[0]);
    std::string name = fields[1];
    std::string buildingInput = fields[2];
    std::string category = fields[3];

    if (!Facility::isValidId(id)) {
        return "invalid facility ID '" + fields[0] +
               "' (must be F followed by exactly three digits)";
    }
    int buildingIndex = graph.findBuilding(buildingInput);
    if (buildingIndex < 0) {
        return "unknown building '" + buildingInput + "' for facility " + id;
    }
    Facility facility(id, name, graph.getBuildingName(buildingIndex), category);
    if (!table.insert(facility)) {
        return "duplicate facility ID " + id + " (first record kept)";
    }
    facilities.push_back(facility);
    return "";
}

// Prints facilities in the order they were accepted.
void FacilityDatabase::displayAll() const {
    if (facilities.empty()) {
        std::cout << "No facilities are loaded." << std::endl;
        std::cout << "Total facilities: 0" << std::endl;
        return;
    }
    std::cout << "ID | Name | Building | Category" << std::endl;
    for (std::size_t i = 0; i < facilities.size(); ++i) {
        std::cout << facilities[i].toString() << std::endl;
    }
    std::cout << "Total facilities: " << facilities.size() << std::endl;
}

// Normalizes, validates, then uses the hash table (never a scan).
void FacilityDatabase::searchById(const std::string& rawInput) const {
    std::string id = Facility::normalizeId(rawInput);
    if (!Facility::isValidId(id)) {
        std::cout << "Invalid format: '" << StringUtils::trim(rawInput)
                  << "'. An ID is F followed by exactly three digits (e.g. F001)." << std::endl;
        return;
    }
    const Facility* found = table.find(id);
    if (found == nullptr) {
        std::cout << "Not found: no facility with ID " << id << "." << std::endl;
        return;
    }
    std::cout << "Found: " << found->getId() << " | " << found->getName() << " | "
              << found->getBuilding() << " | " << found->getCategory() << std::endl;
}

// Builds the HASH-7 diagnostic text.
std::string FacilityDatabase::diagnosticText() const {
    std::ostringstream out;
    out << "(hash capacity " << table.getCapacity() << ", entries " << table.getSize()
        << ", collisions " << table.getCollisions() << ")";
    return out.str();
}

// Number of accepted facilities.
int FacilityDatabase::count() const {
    return static_cast<int>(facilities.size());
}

// Read-only access to the table.
const HashTable& FacilityDatabase::getTable() const {
    return table;
}
