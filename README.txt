Smart Campus Navigation and Facility Analytics System - Milestone 1
===================================================================

PURPOSE
-------
A C++ console program that loads campus data from four text files and lets
the user:
  - view facilities and look them up by ID (custom hash table),
  - view the campus building graph, its counts and degrees,
  - find reachable buildings with Breadth-First Search,
  - add, search, and display departments in a general tree (preorder).

BUILD (UNT CSE CELL machines, Linux, or macOS)
----------------------------------------------
Using the Makefile:
    make

Without make (equivalent command):
    g++ -std=c++11 -Wall -Wextra -pedantic -Iinclude src/*.cpp -o campus

Remove compiled files:
    make clean

RUN
---
Run from the project root so the relative data/ path resolves:
    ./campus                         (loads data/ by default)
    ./campus testdata/collision      (optional data-directory argument)
    ./campus testdata/validation

The program loads buildings.txt, connections.txt, facilities.txt, and
departments.txt (in that order) from the data directory, prints any warnings
and one summary line per file, then shows the menu:

    1. View all facilities
    2. Search facility by ID
    3. View campus graph
    4. Display graph counts and degrees
    5. Find reachable buildings (BFS)
    6. Add department
    7. Search department
    8. Display department hierarchy
    9. Run preorder traversal
    0. Exit

Scripted testing example:
    printf '2\nf004\n5\nLibrary\n0\n' | ./campus

DIRECTORY LAYOUT
----------------
    include/                Header files
        StringUtils.h       trim / case / split helpers
        FileLoader.h        shared record-file reader and load summary
        Facility.h          facility record and ID validation
        HashTable.h         custom separate-chaining hash table (ID key)
        FacilityDatabase.h  facility list + hash table, display and ID search
        CampusGraph.h       adjacency-list graph and BFS
        DepartmentTree.h    general tree (DepartmentNode, DepartmentTree)
    src/                    Implementation files (.cpp), including main.cpp
    data/                   The four instructor-provided input files
    testdata/               Our own test data directories:
        collision/          IDs F001, F014, F027 collide (bucket 11 of 17)
        validation/         malformed, duplicate, unknown-reference records
        missing_files/      only buildings.txt present
        empty_tree/         empty departments.txt (add root from menu)
        components/         graph with three connected components
        windows_crlf/       sample data with CRLF endings and extra spaces
    Makefile
    README.txt
    CONTRIBUTIONS.txt
    design_document.txt
    test_results.txt
    .gitignore

DESIGN SUMMARY (details in design_document.txt)
-----------------------------------------------
  Hash function:      h = 0; for each char c of the uppercase ID:
                      h = (h * 31 + c) mod capacity
  Collision strategy: separate chaining, capacity 17 (HashTable::DEFAULT_CAPACITY)
  Graph:              adjacency list (vector of neighbor-index vectors)
  Tree:               general tree, each node holds a vector of child pointers
