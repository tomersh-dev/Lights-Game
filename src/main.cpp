#include "GameController.h"
#include <iostream>
#include <fstream>
#include <cstdlib> 

int main() {
    try {
        GameController game;
        game.run();
    }
    catch (const std::exception& e) {
        std::ofstream logFile("log.txt");

        if (logFile.is_open()) {
            logFile << e.what() << "\n";
            logFile.close();
        }
        else {
            std::cerr << "Error: Could not open log.txt to write the error message.\n";
            std::cerr << "Original Error: " << e.what() << "\n";
        }

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}