#include "Board.h"
#include <queue>
#include <random>
#include <algorithm>
#include <set>

Board::Board() : m_centerNode(nullptr) {}

void Board::buildGridVertices(const std::vector<int>& rowLengths) {
    m_nodes.clear();
    int numRows = rowLengths.size();
    m_numRows = numRows;
    int midR = numRows / 2;

    for (int r = 0; r < numRows; ++r) {
        int length = rowLengths[r];

        int startQ = (r <= midR) ? -r : -midR;

        for (int i = 0; i < length; ++i) {
            int q = startQ + i;
            m_nodes[{q, r}] = std::make_unique<Node>();

            if (r == midR && i == length / 2) {
                m_centerNode = m_nodes[{q, r}].get();
                m_centerNode->setLit(true);
            }
        }
    }
}

void Board::mapNeighbors() {
    struct HexOffset {
        int q;
        int r;
    };

    static constexpr std::array<HexOffset, 6> HEX_DIRECTIONS = { {
        {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}, {1, -1}
    } };

    for (auto& pair : m_nodes) {
        int q = pair.first.first;
        int r = pair.first.second;
        Node* currentNode = pair.second.get();

        for (int i = 0; i < 6; ++i) {
            int neighborQ = q + HEX_DIRECTIONS[i].q;
            int neighborR = r + HEX_DIRECTIONS[i].r;

            auto it = m_nodes.find({ neighborQ, neighborR });
            if (it != m_nodes.end()) {
                currentNode->setNeighbor(i, it->second.get());
            }
            else {
                currentNode->setNeighbor(i, nullptr);
            }
        }
    }
}

void Board::updateLighting() {
    for (auto& pair : m_nodes) {
        pair.second->setLit(false);
    }

    if (m_centerNode == nullptr) {
        return;
    }

    std::queue<Node*> bfsQueue;

    m_centerNode->setLit(true);
    bfsQueue.push(m_centerNode);

    while (!bfsQueue.empty()) {
        Node* current = bfsQueue.front();
        bfsQueue.pop();

        for (int dir = 0; dir < 6; ++dir) {
            Node* neighbor = current->getNeighbor(dir);

            if (neighbor != nullptr && !neighbor->isLit()) {
                int oppositeDir = (dir + 3) % 6;

                if (current->hasArm(dir) && neighbor->hasArm(oppositeDir)) {
                    neighbor->setLit(true);
                    bfsQueue.push(neighbor);
                }
            }
        }
    }
}

bool Board::isSolved() const {
    for (const auto& pair : m_nodes) {
        if (!pair.second->isLit()) {
            return false;
        }
    }
    return true;
}

void Board::connectNodes(Node* a, int dirAtoB, Node* b, int dirBtoA) {
    if (a != nullptr) a->activateArm(dirAtoB);
    if (b != nullptr) b->activateArm(dirBtoA);
}

void Board::generateSpanningTree() {
    if (m_centerNode == nullptr) return;

    std::set<Node*> visited;
    visited.insert(m_centerNode);

    struct Edge {
        Node* from;
        int dir;
        Node* to;
        int oppositeDir;
    };
    std::vector<Edge> frontier;

    auto addEdges = [&](Node* n) {
        for (int i = 0; i < 6; ++i) {
            Node* neighbor = n->getNeighbor(i);
            if (neighbor != nullptr && visited.find(neighbor) == visited.end()) {
                int opposite = (i + 3) % 6;
                frontier.push_back({ n, i, neighbor, opposite });
            }
        }
        };

    addEdges(m_centerNode);

    std::mt19937 rng(std::random_device{}());

    while (!frontier.empty()) {
        std::uniform_int_distribution<int> dist(0, frontier.size() - 1);
        int idx = dist(rng);
        Edge edge = frontier[idx];

        frontier[idx] = frontier.back();
        frontier.pop_back();

        if (visited.find(edge.to) == visited.end()) {
            visited.insert(edge.to);
            connectNodes(edge.from, edge.dir, edge.to, edge.oppositeDir);
            addEdges(edge.to);
        }
    }
}


void Board::scrambleBoard() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 5);

    for (auto& pair : m_nodes) {
        Node* n = pair.second.get();
        int rotations = dist(rng);
        for (int i = 0; i < rotations; ++i) {
            n->rotateClockwise();
        }
    }
}

void Board::forEachNodeReadonly(const std::function<bool(int q, int r, const Node* node)>& action) const {
    for (const auto& pair : m_nodes) {
        if (action(pair.first.first, pair.first.second, pair.second.get())) {
            break;
        }
    }
}

void Board::rotateNodeAt(int q, int r, bool clockwise) {
    auto it = m_nodes.find(std::make_pair(q, r));

    if (it != m_nodes.end()) {
        if (clockwise) {
            it->second->rotateClockwise();
        }
        else {
            it->second->rotateCounterClockwise();
        }
        updateLighting();
    }
}

void Board::generateRandomLevel(const std::vector<int>& rowLengths) {
    buildGridVertices(rowLengths);
    mapNeighbors();
    generateSpanningTree();
    scrambleBoard();
    updateLighting();
}