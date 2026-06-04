#pragma once
#include <SFML/Graphics/Color.hpp>

namespace Config {
    constexpr unsigned int WINDOW_WIDTH = 1024;
    constexpr unsigned int WINDOW_HEIGHT = 768;
    constexpr unsigned int FPS_LIMIT = 60;

    constexpr unsigned int MAX_LEVELS = 3;

    constexpr float HEX_SPACING = 70.0f;

    constexpr float NODE_RADIUS = 12.0f;

    constexpr float ARM_LENGTH = HEX_SPACING / 2.0f;
    constexpr float ARM_THICKNESS = 4.0f;
    constexpr float NODE_OUTLINE = 2.0f;

    inline const sf::Color COLOR_LIT(255, 215, 0);
    inline const sf::Color COLOR_UNLIT(100, 100, 100);
    inline const sf::Color COLOR_OUTLINE(50, 50, 50);
    inline const sf::Color COLOR_BACKGROUND(30, 30, 30);

    constexpr unsigned int FONT_SIZE = 40;
    inline const sf::Color COLOR_TEXT(255, 255, 255);
}