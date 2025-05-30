#include "ui_setup.h"
#include "ui/theme_colors.h"
#include "Window.h"
#include <iostream>
#include <unistd.h>
#include <cstdint>

const int WIDTH = 480;
const int HEIGHT = 320;

void drawMainWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow, uint16_t activeButtonColor, bool& choosingFont, bool& choosingTheme){
    int w = 200, h = 50, spacing = 8;
    int total_height = 3 * h + 2 * spacing;
    int start_y = (HEIGHT - total_height) / 2;
    int x = (WIDTH - w) / 2;

    mainWindow.background_color = getCurrentBackgroundColor();
      
    mainWindow.addButton(Button(x, start_y, w, h, activeButtonColor, "Start", [](){
        std::cout << "Start clicked\n";
    }));
    mainWindow.addButton(Button(x, start_y + h + spacing, w, h, activeButtonColor, "Setup", [&](){
        std::cout << "Settings clicked\n";
        activeWindow = &settingsWindow;
    }));
    mainWindow.addButton(Button(x, start_y + 2 * (h + spacing), w, h, activeButtonColor, "Exit", [](){
        std::cout << "Exit clicked\n";
    }));
}

void drawSettingsWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow, uint16_t activeButtonColor, bool& choosingFont, bool& choosingTheme){
    int w = 200, h = 50, spacing = 8;
    int total_height = 3 * h + 2 * spacing;
    int start_y = (HEIGHT - total_height) / 2;
    int x = (WIDTH - w) / 2;
    
    settingsWindow.background_color = getCurrentBackgroundColor();

    settingsWindow.addButton(Button(x, start_y, w, h, activeButtonColor, "Font", [&]() {
        std::cout << "Font\n";
        choosingFont = true; 
    }));
    settingsWindow.addButton(Button(x, start_y + h + spacing, w, h, activeButtonColor, "Theme", [&]() {
        std::cout << "Theme\n";
        choosingTheme = true;
    }));
    settingsWindow.addButton(Button(x, start_y + 2 * (h + spacing), w, h, activeButtonColor, "Back", [&]() {
        std::cout << "Returning to Main Menu\n";
        activeWindow = &mainWindow;
    }));
}