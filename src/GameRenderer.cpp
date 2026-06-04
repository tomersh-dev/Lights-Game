#include "GameRenderer.h"
#include <cmath>
#include <SFML/System/Angle.hpp>

GameRenderer::GameRenderer() {}

sf::Vector2f GameRenderer::getPixelPosition(int q, int r, float screenWidth, float screenHeight) {
    float rawX = (q + r * 0.5f) * Config::HEX_SPACING;
    float rawY = r * Config::HEX_SPACING * (std::sqrt(3.0f) / 2.0f);

    float centerX = (Config::LEVEL1_CENTER_Q + Config::LEVEL1_CENTER_R * 0.5f) * Config::HEX_SPACING;
    float centerY = Config::LEVEL1_CENTER_R * Config::HEX_SPACING * (std::sqrt(3.0f) / 2.0f);

    float finalX = (rawX - centerX) + (screenWidth / 2.0f);
    float finalY = (rawY - centerY) + (screenHeight / 2.0f);

    return { finalX, finalY };
}

void GameRenderer::draw(sf::RenderWindow& window, const Board& board) {
    float winWidth = static_cast<float>(window.getSize().x);
    float winHeight = static_cast<float>(window.getSize().y);

    sf::CircleShape nodeShape(Config::NODE_RADIUS);
    nodeShape.setOrigin({ Config::NODE_RADIUS, Config::NODE_RADIUS });
    nodeShape.setOutlineThickness(Config::NODE_OUTLINE);
    nodeShape.setOutlineColor(Config::COLOR_OUTLINE);

    sf::RectangleShape armShape({ Config::ARM_LENGTH, Config::ARM_THICKNESS });
    armShape.setOrigin({ 0.0f, Config::ARM_THICKNESS / 2.0f });

    board.forEachNodeReadonly([&](int q, int r, const Node* node) {
        sf::Vector2f pixelPos = getPixelPosition(q, r, winWidth, winHeight);

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
        });
}