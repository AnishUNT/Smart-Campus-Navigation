// DepartmentTree.h
// Purpose: General tree for the academic department hierarchy. Each node may
//          have any number of children (stored in insertion order). Supports
//          loading departments.txt, adding departments, searching, the
//          indented hierarchy display, and preorder traversal.
// Author:  Anish Sandaka

#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include "FileLoader.h"

#include <string>
#include <vector>

// One department in the tree.
struct DepartmentNode {
    std::string name;
    DepartmentNode* parent;                 // nullptr for the root
    std::vector<DepartmentNode*> children;  // insertion order

    // Creates a node with no children.
    DepartmentNode(const std::string& name, DepartmentNode* parent)
        : name(name), parent(parent) {}
};

class DepartmentTree {
public:
    // Creates an empty tree.
    DepartmentTree();

    // Frees every node.
    ~DepartmentTree();

    // Copying would double-free nodes, so it is disabled.
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    // Loads departments.txt (DepartmentName|ParentName). The first valid
    // record must use NONE as its parent; later parents must already exist.
    LoadStats loadFromFile(const std::string& path, const std::string& fileName);

    // Adds the root of an empty tree. Returns "" on success or an error message.
    std::string addRoot(const std::string& name);

    // Adds name under an existing parent. Returns "" on success or an error.
    std::string addChild(const std::string& name, const std::string& parentName);

    // Finds a department by name (trimmed, ignoring case); nullptr if missing.
    DepartmentNode* find(const std::string& name) const;

    // Edges from the root to node (root depth is 0).
    int depthOf(const DepartmentNode* node) const;

    // True if no departments exist.
    bool isEmpty() const;

    // Number of departments.
    int size() const;

    // Prints name, parent, direct-child count, and depth (TREE-4).
    void displaySearch(const std::string& name) const;

    // Prints the preorder hierarchy, indented 4 spaces per depth (TREE-5).
    void displayHierarchy() const;

    // Prints the preorder sequence without indentation (TREE-6).
    void displayPreorder() const;

    // Returns all nodes in preorder (root, then each child subtree in order).
    std::vector<const DepartmentNode*> preorder() const;

private:
    DepartmentNode* root;
    int nodeCount;

    // Validates and adds one record from departments.txt.
    std::string addDepartmentRecord(const std::string& name, const std::string& parentName);

    // Recursive helpers.
    void preorderHelper(const DepartmentNode* node, std::vector<const DepartmentNode*>& out) const;
    DepartmentNode* findHelper(DepartmentNode* node, const std::string& name) const;
    void destroy(DepartmentNode* node);
};

#endif
