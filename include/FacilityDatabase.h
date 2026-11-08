// FacilityDatabase.h
// Purpose: Holds all accepted facilities in file order (for display) and the
//          custom ID hash table (for lookup). Loads facilities.txt.
// Author:  Anish Sandaka

#ifndef FACILITY_DATABASE_H
#define FACILITY_DATABASE_H

#include "CampusGraph.h"
#include "Facility.h"
#include "FileLoader.h"
#include "HashTable.h"

#include <string>
#include <vector>

class FacilityDatabase {
public:
    // Creates an empty database; the hash table uses its default capacity.
    FacilityDatabase();

    // Loads facilities.txt. Buildings are validated against the graph and
    // stored with the spelling from buildings.txt.
    LoadStats loadFromFile(const std::string& path, const std::string& fileName,
                           const CampusGraph& graph);

    // Displays every facility in file order with a total (FAC-2).
    void displayAll() const;

    // Searches by ID using the hash table and prints one of three outcomes:
    // invalid format, not found, or found (FAC-3).
    void searchById(const std::string& rawInput) const;

    // Returns "(hash capacity C, entries N, collisions K)" for the summary.
    std::string diagnosticText() const;

    // Number of accepted facilities.
    int count() const;

    // Read-only access to the hash table (diagnostics).
    const HashTable& getTable() const;

private:
    std::vector<Facility> facilities;   // file order
    HashTable table;                    // ID lookup
};

#endif
