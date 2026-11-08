// CampusGraph.cpp
// Purpose: Implementation of the adjacency-list campus graph and BFS.
// Author:  Anish Sandaka

#include "CampusGraph.h"
#include "StringUtils.h"

#include <iostream>
#include <queue>

// Empty graph: no buildings, no connections.
CampusGraph::CampusGraph() : connectionCount(0) {}

// Reads buildings.txt. Each record has one field: the building name.
LoadStats CampusGraph::loadBuildings(const std::string& path, const std::string& fileName) {
    LoadStats stats;
    std::ifstream input;
    if (!FileLoader::openFile(input, path, fileName)) {
        return stats;
    }
    std::vector<std::string> fields;
    int lineNumber = 0;
    while (FileLoader::readRecord(input, lineNumber, fields)) {
        std::string reason = FileLoader::checkFields(fields, 1);
        if (reason.empty() && !addBuilding(fields[0])) {
            reason = "duplicate building '" + fields[0] + "' (matches existing '" +
                     buildingNames[findBuilding(fields[0])] + "')";
        }
        FileLoader::countRecord(stats, fileName, lineNumber, reason);
    }
    return stats;
}

// Reads connections.txt. Each record has two fields: BuildingA|BuildingB.
LoadStats CampusGraph::loadConnections(const std::string& path, const std::string& fileName) {
    LoadStats stats;
    std::ifstream input;
    if (!FileLoader::openFile(input, path, fileName)) {
        return stats;
    }
    std::vector<std::string> fields;
    int lineNumber = 0;
    while (FileLoader::readRecord(input, lineNumber, fields)) {
        std::string reason = FileLoader::checkFields(fields, 2);
        if (reason.empty()) {
            reason = addConnectionRecord(fields[0], fields[1]);
        }
        FileLoader::countRecord(stats, fileName, lineNumber, reason);
    }
    return stats;
}

// Validates one connection record and adds it if it is OK.
// Returns "" if accepted, otherwise the reason it was rejected.
std::string CampusGraph::addConnectionRecord(const std::string& nameA, const std::string& nameB) {
    int a = findBuilding(nameA);
    int b = findBuilding(nameB);
    if (a < 0 && b < 0) {
        return "unknown buildings '" + nameA + "' and '" + nameB + "'";
    }
    if (a < 0) {
        return "unknown building '" + nameA + "'";
    }
    if (b < 0) {
        return "unknown building '" + nameB + "'";
    }
    if (a == b) {
        return "self-connection '" + buildingNames[a] + "' is not allowed";
    }
    if (hasConnection(a, b)) {
        return "duplicate connection " + buildingNames[a] + " - " + buildingNames[b];
    }
    addConnection(a, b);
    return "";
}

// Adds a new vertex with an empty neighbor list.
bool CampusGraph::addBuilding(const std::string& name) {
    std::string trimmed = StringUtils::trim(name);
    if (trimmed.empty() || findBuilding(trimmed) >= 0) {
        return false;
    }
    buildingNames.push_back(trimmed);
    adjacency.push_back(std::vector<int>());
    return true;
}

// Appends each endpoint to the other's list (preserves insertion order).
bool CampusGraph::addConnection(int a, int b) {
    if (a < 0 || b < 0 || a >= getBuildingCount() || b >= getBuildingCount() ||
        a == b || hasConnection(a, b)) {
        return false;
    }
    adjacency[a].push_back(b);
    adjacency[b].push_back(a);
    ++connectionCount;
    return true;
}

// Linear scan over building names, comparing ignoring case.
int CampusGraph::findBuilding(const std::string& name) const {
    std::string trimmed = StringUtils::trim(name);
    for (std::size_t i = 0; i < buildingNames.size(); ++i) {
        if (StringUtils::equalsIgnoreCase(buildingNames[i], trimmed)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Since edges are stored at both endpoints, checking a's list is enough;
// this also catches reversed duplicates (B|A after A|B).
bool CampusGraph::hasConnection(int a, int b) const {
    const std::vector<int>& neighbors = adjacency[a];
    for (std::size_t i = 0; i < neighbors.size(); ++i) {
        if (neighbors[i] == b) {
            return true;
        }
    }
    return false;
}

// Number of vertices.
int CampusGraph::getBuildingCount() const {
    return static_cast<int>(buildingNames.size());
}

// Number of undirected edges.
int CampusGraph::getConnectionCount() const {
    return connectionCount;
}

// Degree = number of distinct neighbors.
int CampusGraph::getDegree(int index) const {
    return static_cast<int>(adjacency[index].size());
}

// Building name with the spelling from buildings.txt.
const std::string& CampusGraph::getBuildingName(int index) const {
    return buildingNames[index];
}

// Neighbor indices in insertion order.
const std::vector<int>& CampusGraph::getNeighbors(int index) const {
    return adjacency[index];
}

// Standard BFS with a queue; visited is set when a vertex is enqueued so
// each vertex is enqueued at most once.
std::vector<BfsVisit> CampusGraph::bfs(int start) const {
    std::vector<BfsVisit> order;
    if (start < 0 || start >= getBuildingCount()) {
        return order;
    }
    std::vector<bool> visited(buildingNames.size(), false);
    std::vector<int> distance(buildingNames.size(), -1);
    std::queue<int> pending;

    visited[start] = true;
    distance[start] = 0;
    pending.push(start);

    while (!pending.empty()) {
        int current = pending.front();
        pending.pop();
        BfsVisit visit;
        visit.building = current;
        visit.distance = distance[current];
        order.push_back(visit);

        const std::vector<int>& neighbors = adjacency[current];
        for (std::size_t i = 0; i < neighbors.size(); ++i) {
            int next = neighbors[i];
            if (!visited[next]) {
                visited[next] = true;
                distance[next] = distance[current] + 1;
                pending.push(next);
            }
        }
    }
    return order;
}

// Prints "Building: neighbor, neighbor" or "(no connections)".
void CampusGraph::display() const {
    if (buildingNames.empty()) {
        std::cout << "No buildings are loaded; the campus graph is empty." << std::endl;
        return;
    }
    std::cout << "Campus graph (building: neighbors)" << std::endl;
    for (std::size_t i = 0; i < buildingNames.size(); ++i) {
        std::cout << buildingNames[i] << ": ";
        if (adjacency[i].empty()) {
            std::cout << "(no connections)";
        } else {
            for (std::size_t j = 0; j < adjacency[i].size(); ++j) {
                if (j > 0) {
                    std::cout << ", ";
                }
                std::cout << buildingNames[adjacency[i][j]];
            }
        }
        std::cout << std::endl;
    }
}

// Prints totals followed by one "Building: degree" line per building.
void CampusGraph::displayCountsAndDegrees() const {
    std::cout << "Total buildings: " << getBuildingCount() << std::endl;
    std::cout << "Total connections: " << getConnectionCount() << std::endl;
    if (buildingNames.empty()) {
        std::cout << "No buildings are loaded." << std::endl;
        return;
    }
    std::cout << "Degrees:" << std::endl;
    for (std::size_t i = 0; i < buildingNames.size(); ++i) {
        std::cout << "  " << buildingNames[i] << ": " << adjacency[i].size() << std::endl;
    }
}

// Looks up the start, runs BFS, and prints visit order and distances.
void CampusGraph::displayReachable(const std::string& startName) const {
    int start = findBuilding(startName);
    if (start < 0) {
        std::cout << "Error: unknown building '" << StringUtils::trim(startName) << "'." << std::endl;
        return;
    }
    std::vector<BfsVisit> order = bfs(start);
    std::cout << "Reachable buildings from " << buildingNames[start] << ":" << std::endl;
    for (std::size_t i = 0; i < order.size(); ++i) {
        std::cout << "  " << buildingNames[order[i].building]
                  << " (distance " << order[i].distance << ")" << std::endl;
    }
    std::cout << "Reachable: " << order.size() << " of " << getBuildingCount()
              << " buildings" << std::endl;
}
