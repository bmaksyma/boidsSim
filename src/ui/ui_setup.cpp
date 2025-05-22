#include "ui_setup.h"
#include "Window.h"
#include <iostream>
#include <unistd.h>
#include <cstdint>

// Color definitions
const uint16_t COLOR_BLACK       = 0x0000;
const uint16_t COLOR_WHITE       = 0xFFFF;
const uint16_t COLOR_RED         = 0xF800;
const uint16_t COLOR_GREEN       = 0x07E0;
const uint16_t COLOR_BLUE        = 0x001F;
const uint16_t COLOR_YELLOW      = 0xFFE0;
const uint16_t COLOR_CYAN        = 0x07FF;
const uint16_t COLOR_MAGENTA     = 0xF81F;
const uint16_t COLOR_GRAY        = 0x8410;
const uint16_t COLOR_ORANGE      = 0xFD20;
const uint16_t COLOR_PINK        = 0xF81F;
const uint16_t COLOR_BROWN       = 0xA145;
const uint16_t COLOR_PURPLE      = 0x8010;
const uint16_t COLOR_NAVY        = 0x000F;
const uint16_t COLOR_TEAL        = 0x0438;
const uint16_t COLOR_LIME        = 0x07E0;
const uint16_t COLOR_LIGHT_BLUE  = 0xAEDC;
const uint16_t COLOR_DARK_GREEN  = 0x0320;
const uint16_t COLOR_LIGHT_GRAY  = 0xC618;
const uint16_t COLOR_DARK_GRAY   = 0x4208;

const int WIDTH = 480;
const int HEIGHT = 320;

void drawMainWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow, uint16_t activeButtonColor, bool& choosingColor, bool& choosingFont){
    int w = 200, h = 60, spacing = 5;
    int total_height = 3 * h + 2 * spacing;
    int start_y = (HEIGHT - total_height) / 2;
    int x = (WIDTH - w) / 2;

    mainWindow.background_color = COLOR_NAVY;
      
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

void drawSettingsWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow, uint16_t activeButtonColor, bool& choosingColor, bool& choosingFont){
    int w = 200, h = 60, spacing = 5;
    int total_height = 3 * h + 2 * spacing;
    int start_y = (HEIGHT - total_height) / 2;
    int x = (WIDTH - w) / 2;
    settingsWindow.background_color = COLOR_NAVY;

    settingsWindow.addButton(Button(x, start_y, w, h, activeButtonColor, "Color", [&]() {
        std::cout << "Color\n";
        choosingColor = true;
    }));
    settingsWindow.addButton(Button(x, start_y + h + spacing, w, h, activeButtonColor, "Font", [&]() {
        std::cout << "Font\n";
        choosingFont = true; 

    }));
    settingsWindow.addButton(Button(x, start_y + 2 * (h + spacing), w, h, activeButtonColor, "Size", [&]() {
        std::cout << "Readjust\n";
    }));
    settingsWindow.addButton(Button(x, start_y + 2 * (h + spacing), w, h, activeButtonColor, "Back", [&]() {
        std::cout << "Returning to Main Menu\n";
        activeWindow = &mainWindow;
    }));
}

