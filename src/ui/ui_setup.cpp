#include "ui_setup.h"
#include "ui/theme_colors.h"
#include "Window.h"
#include "utils/text_draw.h"
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <cstring>

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

void drawExitWindow(Window& exitWindow) {
    exitWindow.background_color = getCurrentBackgroundColor();
    exitWindow.buttons.clear();
    
    extern unsigned short *fb;
    extern font_descriptor_t* CUR_FONT;
    
    uint16_t bgColor = getCurrentBackgroundColor();
    uint16_t textColor = (bgColor == 0xFFFF) ? 0x0000 : 0xFFFF;
    
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        fb[i] = bgColor;
    }
    
    const char* exitText = "EXITING...";
    
    int scale = 3;
    int char_width = 8 * scale;
    int char_height = 16 * scale;
    int textWidth = strlen(exitText) * char_width;
    
    int text_x = (WIDTH - textWidth) / 2;
    int text_y = (HEIGHT - char_height) / 2;
    
    draw_text(fb, text_x, text_y, CUR_FONT, exitText, textColor, scale);
}