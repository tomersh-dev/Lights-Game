#pragma once
#include <SFML/Graphics/Color.hpp>

namespace Config {
    // ---------------- Window Settings ----------------
    constexpr unsigned int WINDOW_WIDTH = 1024;
    constexpr unsigned int WINDOW_HEIGHT = 768;
    constexpr unsigned int FPS_LIMIT = 60;

    // ---------------- View Settings ----------------
    constexpr float HEX_SPACING = 120.0f;     // המרחק בין מרכזי שתי נורות
    constexpr float NODE_RADIUS = 25.0f;      // רדיוס הנורה
    constexpr float ARM_LENGTH = 55.0f;       // אורך הזרוע
    constexpr float ARM_THICKNESS = 6.0f;     // עובי הזרוע
    constexpr float NODE_OUTLINE = 2.0f;      // עובי קו המתאר של הנורה

    constexpr int LEVEL1_CENTER_Q = 0;
    constexpr int LEVEL1_CENTER_R = 2;

    // ---------------- Colors ----------------
    inline const sf::Color COLOR_LIT(255, 215, 0);
    inline const sf::Color COLOR_UNLIT(100, 100, 100);
    inline const sf::Color COLOR_OUTLINE(50, 50, 50);
    inline const sf::Color COLOR_BACKGROUND(30, 30, 30);
}