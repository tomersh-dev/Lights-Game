#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "GameRenderer.h"
#include "Constants.h"
#include <optional>

class GameController {
public:
    GameController();
    void run();

private:
    void processEvents();
    void render();
    std::optional<std::pair<int, int>> getClickedHexagon(int mouseX, int mouseY) const;
    void handleMouseClick(int mouseX, int mouseY, bool isLeftClick);
    void loadLevel(int level);

    sf::RenderWindow m_window;
    Board m_board;
    GameRenderer m_renderer;

    bool m_isLevelSolved;
    bool m_isGameFinished;
    int m_currentLevel;

    sf::Font m_font;
    sf::Text m_messageText;
};