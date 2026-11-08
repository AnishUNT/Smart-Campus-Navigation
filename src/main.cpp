// main.cpp
// Purpose: Program entry point. Loads the four data files in the required
//          order (from data/ or a directory given on the command line), then
//          runs the Milestone 1 menu. Data-structure work lives in the
//          CampusGraph, FacilityDatabase, and DepartmentTree modules.
// Author:  Anish Sandaka

#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "FacilityDatabase.h"
#include "FileLoader.h"
#include "StringUtils.h"

#include <iostream>
#include <string>

// Reads one full line from standard input. Returns false at end of input.
static bool readLine(const std::string& prompt, std::string& out) {
    std::cout << prompt;
    if (!std::getline(std::cin, out)) {
        if (std::cin.fail() && !std::cin.eof()) {
            std::cin.clear();   // recover from a stream error
        }
        return false;
    }
    return true;
}

// Prompts for a name. Returns false (after printing an error) if the input
// is blank or input has ended, so the caller can return to the menu.
static bool readName(const std::string& prompt, std::string& out) {
    if (!readLine(prompt, out)) {
        std::cout << std::endl << "Error: no input received." << std::endl;
        return false;
    }
    out = StringUtils::trim(out);
    if (out.empty()) {
        std::cout << "Error: name cannot be blank." << std::endl;
        return false;
    }
    return true;
}

// Parses text as a whole non-negative integer (digits only after trimming).
// Returns -1 if the text is blank, contains non-digits, or is too long.
static int parseMenuNumber(const std::string& text) {
    std::string trimmed = StringUtils::trim(text);
    if (trimmed.empty() || trimmed.size() > 6) {
        return -1;
    }
    int value = 0;
    for (std::string::size_type i = 0; i < trimmed.size(); ++i) {
        if (trimmed[i] < '0' || trimmed[i] > '9') {
            return -1;
        }
        value = value * 10 + (trimmed[i] - '0');
    }
    return value;
}

// Prints the Milestone 1 menu (numbering from Section 6.9).
static void printMenu() {
    std::cout << std::endl
              << "===== Smart Campus Navigation (Milestone 1) =====" << std::endl
              << "1. View all facilities" << std::endl
              << "2. Search facility by ID" << std::endl
              << "3. View campus graph" << std::endl
              << "4. Display graph counts and degrees" << std::endl
              << "5. Find reachable buildings (BFS)" << std::endl
              << "6. Add department" << std::endl
              << "7. Search department" << std::endl
              << "8. Display department hierarchy" << std::endl
              << "9. Run preorder traversal" << std::endl
              << "0. Exit" << std::endl;
}

// Menu option 6: root without a parent prompt if the tree is empty;
// otherwise an existing parent is required.
static void addDepartment(DepartmentTree& tree) {
    std::string name;
    if (!readName("Enter new department name: ", name)) {
        return;
    }
    std::string error;
    if (tree.isEmpty()) {
        error = tree.addRoot(name);
        if (error.empty()) {
            std::cout << "Added '" << name << "' as the root department." << std::endl;
        }
    } else {
        const DepartmentNode* existing = tree.find(name);
        if (existing != nullptr) {
            std::cout << "Error: department '" << existing->name << "' already exists." << std::endl;
            return;
        }
        std::string parentName;
        if (!readName("Enter parent department name: ", parentName)) {
            return;
        }
        error = tree.addChild(name, parentName);
        if (error.empty()) {
            std::cout << "Added '" << name << "' under '" << tree.find(parentName)->name
                      << "'." << std::endl;
        }
    }
    if (!error.empty()) {
        std::cout << "Error: " << error << ". The tree was not changed." << std::endl;
    }
}

// Loads all files, then repeats the menu until the user chooses 0.
int main(int argc, char* argv[]) {
    std::string dataDir = "data";
    if (argc > 1) {
        dataDir = argv[1];
    }
    std::cout << "Loading data from directory: " << dataDir << std::endl;

    CampusGraph graph;
    FacilityDatabase facilities;
    DepartmentTree departments;

    // Required load order: buildings, connections, facilities, departments.
    LoadStats stats;
    stats = graph.loadBuildings(FileLoader::joinPath(dataDir, "buildings.txt"), "buildings.txt");
    FileLoader::printSummary("buildings.txt", stats);

    stats = graph.loadConnections(FileLoader::joinPath(dataDir, "connections.txt"), "connections.txt");
    FileLoader::printSummary("connections.txt", stats);

    stats = facilities.loadFromFile(FileLoader::joinPath(dataDir, "facilities.txt"), "facilities.txt", graph);
    FileLoader::printSummary("facilities.txt", stats, facilities.diagnosticText());

    stats = departments.loadFromFile(FileLoader::joinPath(dataDir, "departments.txt"), "departments.txt");
    FileLoader::printSummary("departments.txt", stats);

    bool running = true;
    while (running) {
        printMenu();
        std::string input;
        if (!readLine("Enter choice: ", input)) {
            std::cout << std::endl << "End of input. Exiting." << std::endl;
            break;
        }
        int choice = parseMenuNumber(input);
        std::string name;

        switch (choice) {
            case 1:
                facilities.displayAll();
                break;
            case 2:
                if (readLine("Enter facility ID: ", name)) {
                    facilities.searchById(name);
                } else {
                    std::cout << std::endl << "Error: no input received." << std::endl;
                }
                break;
            case 3:
                graph.display();
                break;
            case 4:
                graph.displayCountsAndDegrees();
                break;
            case 5:
                if (readName("Enter starting building: ", name)) {
                    graph.displayReachable(name);
                }
                break;
            case 6:
                addDepartment(departments);
                break;
            case 7:
                if (readName("Enter department name: ", name)) {
                    departments.displaySearch(name);
                }
                break;
            case 8:
                departments.displayHierarchy();
                break;
            case 9:
                departments.displayPreorder();
                break;
            case 0:
                std::cout << "Goodbye." << std::endl;
                running = false;
                break;
            default:
                std::cout << "Error: invalid choice '" << StringUtils::trim(input)
                          << "'. Please enter a number from 0 to 9." << std::endl;
                break;
        }
    }
    return 0;
}
