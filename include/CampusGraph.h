// CampusGraph.h
// Purpose: Undirected, unweighted campus graph stored as an adjacency list.
//          Buildings are vertices (kept in buildings.txt order); walkways are
//          edges (neighbors kept in connection insertion order). Provides
//          display, counts/degrees, and BFS reachability.
// Author:  Anish Sandaka

#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

#include "FileLoader.h"

#include <string>
#include <vector>

// One BFS result entry: the building index and its distance from the start.
struct BfsVisit {
    int building;
    int distance;
};

class CampusGraph {
public:
    // Creates an empty graph.
    CampusGraph();

    // Loads buildings.txt (one name per record; duplicates ignoring case rejected).
    LoadStats loadBuildings(const std::string& path, const std::string& fileName);

    // Loads connections.txt (A|B; unknown, self, duplicate, and reversed
    // duplicate connections rejected).
    LoadStats loadConnections(const std::string& path, const std::string& fileName);

    // Adds a building. Returns false if a building with the same name
    // (ignoring case) already exists.
    bool addBuilding(const std::string& name);

    // Adds an undirected edge between two building indices (both endpoints
    // updated, counted once). Returns false for self or duplicate edges.
    bool addConnection(int a, int b);

    // Returns the index of a building matching name (trimmed, case-insensitive),
    // or -1 if none matches.
    int findBuilding(const std::string& name) const;

    // True if a and b are already directly connected.
    bool hasConnection(int a, int b) const;

    // Accessors.
    int getBuildingCount() const;
    int getConnectionCount() const;
    int getDegree(int index) const;
    const std::string& getBuildingName(int index) const;
    const std::vector<int>& getNeighbors(int index) const;

    // Runs BFS from start; returns visited buildings in visit order with
    // their distances. A building is marked visited when it is enqueued.
    std::vector<BfsVisit> bfs(int start) const;

    // Prints every building with its neighbors (GRAPH-3).
    void display() const;

    // Prints totals and the degree of every building (GRAPH-4).
    void displayCountsAndDegrees() const;

    // Prompts are handled in main; this prints BFS results for a typed name
    // (BFS-1 to BFS-3).
    void displayReachable(const std::string& startName) const;

private:
    // Validates and adds one connection record from connections.txt.
    std::string addConnectionRecord(const std::string& nameA, const std::string& nameB);

    std::vector<std::string> buildingNames;        // vertex index -> name
    std::vector<std::vector<int> > adjacency;      // vertex index -> neighbors
    int connectionCount;                           // undirected edges, counted once
};

#endif
