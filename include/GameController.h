#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "GameRenderer.h"
#include "Constants.h"

class GameController {
public:
    GameController();
    void run();

private:
    sf::RenderWindow m_window;
    Board m_board;
    GameRenderer m_renderer;

    bool m_isLevelSolved;
    void loadLevel(int level);
    int m_currentLevel;
    void processEvents();
    void render();
    void handleMouseClick(int mouseX, int mouseY, bool isLeftClick);
};