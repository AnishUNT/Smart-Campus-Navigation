// DepartmentTree.cpp
// Purpose: Implementation of the department general tree.
// Author:  Anish Sandaka

#include "DepartmentTree.h"
#include "StringUtils.h"

#include <iostream>

// Empty tree.
DepartmentTree::DepartmentTree() : root(nullptr), nodeCount(0) {}

// Releases all nodes (postorder delete).
DepartmentTree::~DepartmentTree() {
    destroy(root);
}

// Deletes children first, then the node itself.
void DepartmentTree::destroy(DepartmentNode* node) {
    if (node == nullptr) {
        return;
    }
    for (std::size_t i = 0; i < node->children.size(); ++i) {
        destroy(node->children[i]);
    }
    delete node;
}

// Reads departments.txt (DepartmentName|ParentName).
LoadStats DepartmentTree::loadFromFile(const std::string& path, const std::string& fileName) {
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
            reason = addDepartmentRecord(fields[0], fields[1]);
        }
        FileLoader::countRecord(stats, fileName, lineNumber, reason);
    }
    return stats;
}

// Applies the root rules, then adds the department under its parent.
// Returns "" if accepted, otherwise the reason it was rejected.
std::string DepartmentTree::addDepartmentRecord(const std::string& name,
                                                const std::string& parentName) {
    bool parentIsNone = StringUtils::equalsIgnoreCase(parentName, "NONE");

    if (root == nullptr) {
        if (!parentIsNone) {
            return "unknown parent '" + parentName +
                   "' (the first department must be the root with parent NONE)";
        }
        return addRoot(name);
    }
    if (parentIsNone) {
        return "department '" + name + "' has parent NONE but a root ('" +
               root->name + "') already exists";
    }
    return addChild(name, parentName);
}

// Creates the root if the tree is empty.
std::string DepartmentTree::addRoot(const std::string& name) {
    std::string trimmed = StringUtils::trim(name);
    if (trimmed.empty()) {
        return "department name cannot be blank";
    }
    if (root != nullptr) {
        return "a root department already exists";
    }
    root = new DepartmentNode(trimmed, nullptr);
    nodeCount = 1;
    return "";
}

// Adds a child to the end of the parent's child list.
std::string DepartmentTree::addChild(const std::string& name, const std::string& parentName) {
    std::string trimmed = StringUtils::trim(name);
    std::string trimmedParent = StringUtils::trim(parentName);
    if (trimmed.empty()) {
        return "department name cannot be blank";
    }
    DepartmentNode* parent = find(trimmedParent);
    if (parent == nullptr) {
        return "unknown parent department '" + trimmedParent + "'";
    }
    DepartmentNode* existing = find(trimmed);
    if (existing != nullptr) {
        return "duplicate department '" + trimmed + "' (already exists as '" +
               existing->name + "')";
    }
    parent->children.push_back(new DepartmentNode(trimmed, parent));
    ++nodeCount;
    return "";
}

// Searches the whole tree in preorder.
DepartmentNode* DepartmentTree::find(const std::string& name) const {
    return findHelper(root, StringUtils::trim(name));
}

// Recursive preorder search.
DepartmentNode* DepartmentTree::findHelper(DepartmentNode* node, const std::string& name) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (StringUtils::equalsIgnoreCase(node->name, name)) {
        return node;
    }
    for (std::size_t i = 0; i < node->children.size(); ++i) {
        DepartmentNode* result = findHelper(node->children[i], name);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

// Counts parent links up to the root.
int DepartmentTree::depthOf(const DepartmentNode* node) const {
    int depth = 0;
    while (node != nullptr && node->parent != nullptr) {
        node = node->parent;
        ++depth;
    }
    return depth;
}

// True if the tree has no root.
bool DepartmentTree::isEmpty() const {
    return root == nullptr;
}

// Number of departments.
int DepartmentTree::size() const {
    return nodeCount;
}

// Prints the details of one department or a not-found message.
void DepartmentTree::displaySearch(const std::string& name) const {
    if (isEmpty()) {
        std::cout << "No departments are loaded." << std::endl;
        return;
    }
    const DepartmentNode* node = find(name);
    if (node == nullptr) {
        std::cout << "Department '" << StringUtils::trim(name) << "' was not found." << std::endl;
        return;
    }
    std::cout << "Name: " << node->name << std::endl;
    std::cout << "Parent: " << (node->parent == nullptr ? "None" : node->parent->name) << std::endl;
    std::cout << "Direct children: " << node->children.size() << std::endl;
    std::cout << "Depth: " << depthOf(node) << std::endl;
}

// Preorder with four spaces of indentation per depth level.
void DepartmentTree::displayHierarchy() const {
    if (isEmpty()) {
        std::cout << "No departments are loaded; the department tree is empty." << std::endl;
        return;
    }
    std::vector<const DepartmentNode*> nodes = preorder();
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        std::cout << std::string(4 * depthOf(nodes[i]), ' ') << nodes[i]->name << std::endl;
    }
}

// Preorder sequence, one department per line, no indentation.
void DepartmentTree::displayPreorder() const {
    if (isEmpty()) {
        std::cout << "No departments are loaded; the department tree is empty." << std::endl;
        return;
    }
    std::vector<const DepartmentNode*> nodes = preorder();
    std::cout << "Preorder traversal:" << std::endl;
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        std::cout << nodes[i]->name << std::endl;
    }
}

// Collects nodes: visit node, then each child subtree in insertion order.
std::vector<const DepartmentNode*> DepartmentTree::preorder() const {
    std::vector<const DepartmentNode*> out;
    preorderHelper(root, out);
    return out;
}

// Recursive preorder step.
void DepartmentTree::preorderHelper(const DepartmentNode* node,
                                    std::vector<const DepartmentNode*>& out) const {
    if (node == nullptr) {
        return;
    }
    out.push_back(node);
    for (std::size_t i = 0; i < node->children.size(); ++i) {
        preorderHelper(node->children[i], out);
    }
}
