#pragma once
#include "Node.h"
#include <vector>
#include <map>
#include <memory>
#include <utility>
#include <functional>

class Board {
public:
    Board();

    void generateRandomLevel(const std::vector<int>& rowLengths);
    void updateLighting();
    bool isSolved() const;
    void rotateNodeAt(int q, int r);
    void forEachNodeReadonly(const std::function<void(int q, int r, const Node* node)>& action) const;

private:
    std::map<std::pair<int, int>, std::unique_ptr<Node>> m_nodes;
    Node* m_centerNode;

    void buildGridVertices(const std::vector<int>& rowLengths);
    void mapNeighbors();
    void generateSpanningTree();
    void addRandomCycles(int numCycles);
    void scrambleBoard();

    void connectNodes(Node* a, int dirAtoB, Node* b, int dirBtoA);

    int calculateDynamicCycleCount() const;
};