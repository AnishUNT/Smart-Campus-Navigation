// Facility.h
// Purpose: A single campus facility record (ID, name, building, category)
//          plus static helpers for normalizing and validating facility IDs.
// Author:  Anish Sandaka

#ifndef FACILITY_H
#define FACILITY_H

#include <string>

class Facility {
public:
    // Creates an empty facility (needed for containers).
    Facility();

    // Creates a facility; id should already be normalized to uppercase.
    Facility(const std::string& id, const std::string& name,
             const std::string& building, const std::string& category);

    // Accessors.
    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getBuilding() const;
    const std::string& getCategory() const;

    // Returns "ID | Name | Building | Category".
    std::string toString() const;

    // Trims whitespace and converts to uppercase (e.g. " f004 " -> "F004").
    static std::string normalizeId(const std::string& rawId);

    // True if a normalized ID is 'F' followed by exactly three digits.
    static bool isValidId(const std::string& normalizedId);

private:
    std::string id;
    std::string name;
    std::string building;
    std::string category;
};

#endif
