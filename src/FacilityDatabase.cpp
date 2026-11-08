// FacilityDatabase.cpp
// Purpose: Loading, display, and ID search for facilities.
// Author:  Anish Sandaka

#include "FacilityDatabase.h"
#include "StringUtils.h"

#include <iostream>
#include <sstream>

// Starts with an empty list and an empty hash table.
FacilityDatabase::FacilityDatabase() : table(HashTable::DEFAULT_CAPACITY) {}

// Validation order: (field count, empty fields - done by FileLoader),
// ID format, unknown building, duplicate ID.
LoadStats FacilityDatabase::loadFromFile(const std::string& path, const std::string& fileName,
                                         const CampusGraph& graph) {
    RecordHandler handler = [this, &graph](const std::vector<std::string>& fields) -> std::string {
        std::string id = Facility::normalizeId(fields[0]);
        const std::string& name = fields[1];
        const std::string& buildingInput = fields[2];
        const std::string& category = fields[3];

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
    };
    return FileLoader::loadRecords(path, fileName, 4, handler);
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
