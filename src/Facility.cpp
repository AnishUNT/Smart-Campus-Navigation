// Facility.cpp
// Purpose: Implementation of the Facility record and ID helpers.
// Author:  Anish Sandaka

#include "Facility.h"
#include "StringUtils.h"

#include <cctype>

// Default constructor: all fields empty.
Facility::Facility() {}

// Stores the four fields of a facility.
Facility::Facility(const std::string& id, const std::string& name,
                   const std::string& building, const std::string& category)
    : id(id), name(name), building(building), category(category) {}

// Returns the uppercase facility ID.
const std::string& Facility::getId() const { return id; }

// Returns the facility name.
const std::string& Facility::getName() const { return name; }

// Returns the building name (spelling from buildings.txt).
const std::string& Facility::getBuilding() const { return building; }

// Returns the category name.
const std::string& Facility::getCategory() const { return category; }

// Formats the facility on one line.
std::string Facility::toString() const {
    return id + " | " + name + " | " + building + " | " + category;
}

// Trim first, then uppercase, as required by Section 4.1.
std::string Facility::normalizeId(const std::string& rawId) {
    return StringUtils::toUpper(StringUtils::trim(rawId));
}

// Valid format: exactly 4 characters, 'F' then three digits.
bool Facility::isValidId(const std::string& normalizedId) {
    if (normalizedId.size() != 4 || normalizedId[0] != 'F') {
        return false;
    }
    for (std::string::size_type i = 1; i < 4; ++i) {
        if (!std::isdigit(static_cast<unsigned char>(normalizedId[i]))) {
            return false;
        }
    }
    return true;
}
