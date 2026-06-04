#pragma once
#include <array>
#include <bitset>

class Node {
public:
    Node();

    void rotateClockwise();
    void rotateCounterClockwise();

    void activateArm(int direction);
    bool hasArm(int direction) const;

    void setLit(bool state);
    bool isLit() const;

    void setNeighbor(int direction, Node* neighbor);
    Node* getNeighbor(int direction);
    const Node* getNeighbor(int direction) const;

private:
    std::bitset<6> m_arms;
    bool m_isLit;
    std::array<Node*, 6> m_neighbors;
};