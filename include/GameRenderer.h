#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Constants.h"

class GameRenderer {
public:
    GameRenderer();
    void draw(sf::RenderWindow& window, const Board& board);
    static float calculateScale(int numRows, float screenWidth, float screenHeight);
    static sf::Vector2f getPixelPosition(int q, int r, float screenWidth, float screenHeight, int midR, float scale);
};