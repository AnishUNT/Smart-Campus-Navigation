// HashTable.h
// Purpose: Custom hash table keyed by facility ID, using separate chaining.
//          Supports insertion, lookup, duplicate-key detection, and a
//          collision counter used by the load diagnostic (HASH-7).
// Author:  Anish Sandaka

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "Facility.h"

#include <string>
#include <vector>

class HashTable {
public:
    // Number of buckets used when no capacity is given. A prime number
    // spreads keys more evenly with the polynomial hash.
    static const int DEFAULT_CAPACITY = 17;

    // Creates a table with the given number of buckets (minimum 1).
    explicit HashTable(int capacity = DEFAULT_CAPACITY);

    // Custom polynomial string hash:
    //   h = 0; for each character c: h = (h * 31 + c) mod capacity
    // The key is normalized to uppercase first. Returns 0..capacity-1.
    int hash(const std::string& key) const;

    // Inserts a facility using its ID as the key.
    // Returns false (and changes nothing) if the ID is already present.
    // A collision is counted when an accepted insert lands in a nonempty bucket.
    bool insert(const Facility& facility);

    // Looks up a facility by ID (normalized to uppercase).
    // Returns a pointer to the stored facility, or nullptr if not found.
    const Facility* find(const std::string& key) const;

    // True if the key is already stored.
    bool contains(const std::string& key) const;

    // Diagnostics.
    int getCapacity() const;
    int getSize() const;
    int getCollisions() const;

private:
    // Each bucket is a chain of facilities; chains grow without a fixed limit.
    std::vector<std::vector<Facility> > buckets;
    int entryCount;
    int collisionCount;
};

#endif
