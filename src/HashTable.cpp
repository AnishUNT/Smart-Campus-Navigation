// HashTable.cpp
// Purpose: Implementation of the separate-chaining facility ID hash table.
// Author:  Anish Sandaka

#include "HashTable.h"
#include "StringUtils.h"

// Builds an empty table with 'capacity' empty chains.
HashTable::HashTable(int capacity)
    : buckets(capacity > 0 ? capacity : 1), entryCount(0), collisionCount(0) {}

// Horner's-rule polynomial hash with base 31, reduced mod capacity at every
// step so the value never overflows and always ends in range.
int HashTable::hash(const std::string& key) const {
    std::string normalized = StringUtils::toUpper(StringUtils::trim(key));
    unsigned long capacity = static_cast<unsigned long>(buckets.size());
    unsigned long h = 0;
    for (std::string::size_type i = 0; i < normalized.size(); ++i) {
        unsigned long c = static_cast<unsigned char>(normalized[i]);
        h = (h * 31UL + c) % capacity;
    }
    return static_cast<int>(h);
}

// Rejects duplicates before touching the chain or the collision counter.
bool HashTable::insert(const Facility& facility) {
    std::string key = StringUtils::toUpper(StringUtils::trim(facility.getId()));
    if (contains(key)) {
        return false;
    }
    std::vector<Facility>& chain = buckets[hash(key)];
    if (!chain.empty()) {
        ++collisionCount;   // accepted insert into a nonempty bucket
    }
    chain.push_back(facility);
    ++entryCount;
    return true;
}

// Hashes the key and searches only that bucket's chain.
const Facility* HashTable::find(const std::string& key) const {
    std::string normalized = StringUtils::toUpper(StringUtils::trim(key));
    const std::vector<Facility>& chain = buckets[hash(normalized)];
    for (std::size_t i = 0; i < chain.size(); ++i) {
        if (chain[i].getId() == normalized) {
            return &chain[i];
        }
    }
    return nullptr;
}

// True if find() succeeds.
bool HashTable::contains(const std::string& key) const {
    return find(key) != nullptr;
}

// Number of buckets.
int HashTable::getCapacity() const {
    return static_cast<int>(buckets.size());
}

// Number of stored facilities.
int HashTable::getSize() const {
    return entryCount;
}

// Number of collisions counted during accepted insertions.
int HashTable::getCollisions() const {
    return collisionCount;
}
