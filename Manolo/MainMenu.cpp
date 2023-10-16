// MainMenu.cpp

#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu() {
    // Constructor code, if needed
}

MainMenu::~MainMenu() {
    // Destructor code, if needed
}

void MainMenu::Show() {
    bool running = true;
    while (running) {
        // Render the menu options (e.g., "Play", "Options", "Exit")
        std::cout << "Main Menu\n";
        std::cout << "1. Play\n";
        std::cout << "2. Options\n";
        std::cout << "3. Exit\n";

        // Handle user input
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            // Start the game
            running = false;
            break;
        case 2:
            // Handle options (if needed)
            break;
        case 3:
            // Exit the game
            running = false;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}
