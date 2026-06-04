#include "GameRenderer.h"
#include <cmath>
#include <SFML/System/Angle.hpp>
#include <algorithm>

GameRenderer::GameRenderer() {}

float GameRenderer::calculateScale(int numRows, float screenWidth, float screenHeight) {
    float boardWidth = numRows * Config::HEX_SPACING;
    float boardHeight = numRows * Config::HEX_SPACING * (std::sqrt(3.0f) / 2.0f);
    float scaleX = (screenWidth * 0.90f) / boardWidth;
    float scaleY = (screenHeight * 0.85f) / boardHeight;
    return std::min(1.0f, std::min(scaleX, scaleY));
}

sf::Vector2f GameRenderer::getPixelPosition(int q, int r, float screenWidth, float screenHeight, int midR, float scale) {
    float scaledSpacing = Config::HEX_SPACING * scale;

    float rawX = (q + r * 0.5f) * scaledSpacing;
    float rawY = r * scaledSpacing * (std::sqrt(3.0f) / 2.0f);

    float centerX = (0 + midR * 0.5f) * scaledSpacing;
    float centerY = midR * scaledSpacing * (std::sqrt(3.0f) / 2.0f);

    float finalX = (rawX - centerX) + (screenWidth / 2.0f);
    float finalY = (rawY - centerY) + (screenHeight * 0.52f);

    return { finalX, finalY };
}

void GameRenderer::draw(sf::RenderWindow& window, const Board& board) {
    float winWidth = static_cast<float>(window.getSize().x);
    float winHeight = static_cast<float>(window.getSize().y);
    int numRows = board.getNumRows();
    int midR = numRows / 2;
    float scale = calculateScale(numRows, winWidth, winHeight);

    float scaledRadius = Config::NODE_RADIUS * scale;
    float scaledOutline = Config::NODE_OUTLINE * scale;
    float scaledArmLength = Config::ARM_LENGTH * scale;
    float scaledArmThickness = Config::ARM_THICKNESS * scale;

    sf::CircleShape nodeShape(scaledRadius);
    nodeShape.setOrigin({ scaledRadius, scaledRadius });
    nodeShape.setOutlineThickness(scaledOutline);
    nodeShape.setOutlineColor(Config::COLOR_OUTLINE);

    sf::RectangleShape armShape({ scaledArmLength, scaledArmThickness });
    armShape.setOrigin({ 0.0f, scaledArmThickness / 2.0f });

    board.forEachNodeReadonly([&](int q, int r, const Node* node) {
        sf::Vector2f pixelPos = getPixelPosition(q, r, winWidth, winHeight, midR, scale);

        sf::Color activeColor = node->isLit() ? Config::COLOR_LIT : Config::COLOR_UNLIT;

        armShape.setPosition(pixelPos);
        armShape.setFillColor(activeColor);

        for (int dir = 0; dir < 6; ++dir) {
            if (node->hasArm(dir)) {
                armShape.setRotation(sf::degrees(dir * 60.0f));
                window.draw(armShape);
            }
        }

        nodeShape.setPosition(pixelPos);
        nodeShape.setFillColor(activeColor);
        window.draw(nodeShape);

        return false;
        });
}