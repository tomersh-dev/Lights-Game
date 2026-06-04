#include "Node.h"

Node::Node() : m_isLit(false) {
    m_arms.reset();
    m_neighbors.fill(nullptr);
}

void Node::rotateClockwise() {
    bool lastArm = m_arms[5];
    m_arms <<= 1;
    m_arms[0] = lastArm;
}

void Node::rotateCounterClockwise() {
    bool firstArm = m_arms[0];
    m_arms >>= 1;
    m_arms[5] = firstArm;
}

void Node::activateArm(int direction) {
    if (direction >= 0 && direction < 6) {
        m_arms.set(direction);
    }
}

bool Node::hasArm(int direction) const {
    if (direction >= 0 && direction < 6) {
        return m_arms.test(direction);
    }
    return false;
}

void Node::setLit(bool state) {
    m_isLit = state;
}

bool Node::isLit() const {
    return m_isLit;
}

void Node::setNeighbor(int direction, Node* neighbor) {
    if (direction >= 0 && direction < 6) {
        m_neighbors[direction] = neighbor;
    }
}

Node* Node::getNeighbor(int direction) {
    if (direction >= 0 && direction < 6) return m_neighbors[direction];
    return nullptr;
}

const Node* Node::getNeighbor(int direction) const {
    if (direction >= 0 && direction < 6) return m_neighbors[direction];
    return nullptr;
}