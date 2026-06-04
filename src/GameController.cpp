#include "GameController.h"
#include <iostream>
#include <cmath>

GameController::GameController()
    : m_window(sf::VideoMode({ Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT }), "Lights Puzzle Game"),
    m_isLevelSolved(false), m_currentLevel(1)
{
    m_window.setFramerateLimit(Config::FPS_LIMIT);
    loadLevel(m_currentLevel);
}

void GameController::loadLevel(int level) {
    int middleLength = 5 + (level - 1) * 2;
    int numRows = middleLength;
    std::vector<int> rowLengths(numRows);
    int midIndex = numRows / 2;

    for (int r = 0; r < numRows; ++r) {
        int dist = std::abs(r - midIndex);
        rowLengths[r] = middleLength - dist;
    }

    m_board.generateRandomLevel(rowLengths);
    m_isLevelSolved = false;
    std::cout << "Loading Level " << level << "..." << std::endl;
}

void GameController::run() {
    while (m_window.isOpen()) {
        processEvents();
        render();
    }
}

void GameController::processEvents() {
    while (const std::optional<sf::Event> event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }
        else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {

            if (m_isLevelSolved) {
                if (m_currentLevel < Config::MAX_LEVELS) {
                    m_currentLevel++;
                    loadLevel(m_currentLevel);
                }
                else {
                    std::cout << "Game Finished! All levels completed." << std::endl;
                    m_window.close();
                }
                continue;
            }

            if (mouseEvent->button == sf::Mouse::Button::Left) {
                handleMouseClick(mouseEvent->position.x, mouseEvent->position.y, true);
            }
            else if (mouseEvent->button == sf::Mouse::Button::Right) {
                handleMouseClick(mouseEvent->position.x, mouseEvent->position.y, false);
            }
        }
    }
}

void GameController::handleMouseClick(int mouseX, int mouseY, bool isLeftClick) {
    float winWidth = static_cast<float>(m_window.getSize().x);
    float winHeight = static_cast<float>(m_window.getSize().y);
    int midR = m_board.getNumRows() / 2;

    int clickedQ = -999;
    int clickedR = -999;
    bool nodeFound = false;

    m_board.forEachNodeReadonly([&](int q, int r, const Node* node) {
        if (nodeFound) return;

        sf::Vector2f nodePos = GameRenderer::getPixelPosition(q, r, winWidth, winHeight, midR);

        float dx = nodePos.x - static_cast<float>(mouseX);
        float dy = nodePos.y - static_cast<float>(mouseY);
        float distanceSquared = (dx * dx) + (dy * dy);

        if (distanceSquared <= (Config::NODE_RADIUS * Config::NODE_RADIUS)) {
            clickedQ = q;
            clickedR = r;
            nodeFound = true;
        }
        });

    if (nodeFound) {
        m_board.rotateNodeAt(clickedQ, clickedR, isLeftClick);

        if (m_board.isSolved()) {
            m_isLevelSolved = true;
            std::cout << "Level " << m_currentLevel << " Solved! Click anywhere to continue." << std::endl;
        }
    }
}

void GameController::render() {
    m_window.clear(Config::COLOR_BACKGROUND);
    m_renderer.draw(m_window, m_board);
    m_window.display();
}