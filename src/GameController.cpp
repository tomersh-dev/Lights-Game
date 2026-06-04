#include "GameController.h"
#include <cmath>
#include <stdexcept>

GameController::GameController()
    : m_window(sf::VideoMode({ Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT }), "Lights Puzzle Game"),
    m_isLevelSolved(false),
    m_isGameFinished(false),
    m_currentLevel(1),
    m_messageText(m_font)
{
    m_window.setFramerateLimit(Config::FPS_LIMIT);
    bool fontLoaded = false;
    try {
        fontLoaded = m_font.openFromFile("resources/arial.ttf");
    }
    catch (...) {
        fontLoaded = false;
    }
    if (!fontLoaded) {
        throw std::runtime_error("Critical Error: Failed to load font 'resources/arial.ttf'.");
    }

    m_messageText.setCharacterSize(Config::FONT_SIZE);
    m_messageText.setFillColor(Config::COLOR_TEXT);
    m_messageText.setOutlineColor(sf::Color::Black);
    m_messageText.setOutlineThickness(2.0f);

    loadLevel(m_currentLevel);
}

void GameController::loadLevel(int level) {
    int middleLength = Config::INITIAL_LEVEL_SIZE + (level - 1) * Config::LEVEL_SIZE_INCREMENT;
    int numRows = middleLength;
    std::vector<int> rowLengths(numRows);
    int midIndex = numRows / 2;

    for (int r = 0; r < numRows; ++r) {
        int dist = std::abs(r - midIndex);
        rowLengths[r] = middleLength - dist;
    }

    m_board.generateRandomLevel(rowLengths);
    m_isLevelSolved = false;
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
            if (m_isGameFinished) {
                m_window.close();
                continue;
            }

            if (m_isLevelSolved) {
                if (m_currentLevel < Config::MAX_LEVELS) {
                    m_currentLevel++;
                    loadLevel(m_currentLevel);
                }
                else {
                    m_isGameFinished = true;
                    m_isLevelSolved = false;
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

std::optional<std::pair<int, int>> GameController::getClickedHexagon(int mouseX, int mouseY) const {
    float winWidth = static_cast<float>(m_window.getSize().x);
    float winHeight = static_cast<float>(m_window.getSize().y);
    int numRows = m_board.getNumRows();
    int midR = numRows / 2;

    float scale = GameRenderer::calculateScale(numRows, winWidth, winHeight);
    float scaledRadius = Config::NODE_RADIUS * scale;
    float clickRadiusSquared = scaledRadius * scaledRadius;

    std::optional<std::pair<int, int>> clickedNode = std::nullopt;

    m_board.forEachNodeReadonly([&](int q, int r, const Node* node) {
        sf::Vector2f nodePos = GameRenderer::getPixelPosition(q, r, winWidth, winHeight, midR, scale);

        float dx = nodePos.x - static_cast<float>(mouseX);
        float dy = nodePos.y - static_cast<float>(mouseY);
        float distanceSquared = (dx * dx) + (dy * dy);

        if (distanceSquared <= clickRadiusSquared) {
            clickedNode = std::make_pair(q, r);
        }
        return false;
        });

    return clickedNode;
}

void GameController::handleMouseClick(int mouseX, int mouseY, bool isLeftClick) {
    auto clickedHex = getClickedHexagon(mouseX, mouseY);

    if (clickedHex.has_value()) {
        m_board.rotateNodeAt(clickedHex->first, clickedHex->second, isLeftClick);

        if (m_board.isSolved()) {
            m_isLevelSolved = true;
        }
    }
}

void GameController::render() {
    m_window.clear(Config::COLOR_BACKGROUND);
    m_renderer.draw(m_window, m_board);

    if (m_isLevelSolved || m_isGameFinished) {

        if (m_isGameFinished) {
            m_messageText.setString("Game Finished! All levels completed.\nClick anywhere to exit.");
        }
        else if (m_isLevelSolved) {
            m_messageText.setString("Level " + std::to_string(m_currentLevel) + " Solved!\nClick anywhere to continue.");
        }

        sf::FloatRect textRect = m_messageText.getLocalBounds();
        m_messageText.setOrigin({ textRect.position.x + textRect.size.x / 2.0f,
                                 textRect.position.y + textRect.size.y / 2.0f });

        m_messageText.setPosition({ Config::WINDOW_WIDTH / 2.0f, Config::WINDOW_HEIGHT / 8.0f });

        m_window.draw(m_messageText);
    }

    m_window.display();
}