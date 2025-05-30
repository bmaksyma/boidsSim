#include <vector>
#include <cstdint>
#include "Window.h"

#ifndef HANDLECOLORCHOICE_H
#define HANDLECOLORCHOICE_H

void handleColorChoice(int8_t delta, int& selectedColorID, uint16_t& activeButtonColor,
    std::vector<uint16_t>& buttonColors,
    Window* activeWindow, Window* mainWindow, Window* settingsWindow);

void handleFontChoice(int8_t delta, int& selectedFontID, 
    std::vector<font_descriptor_t*>& availableFonts,Window* activeWindow,Window* mainWindow, Window* settingsWindow);

void handleThemeChoice(int8_t delta, int& selectedThemeID,
    std::vector<uint16_t>& buttonColors, Window* activeWindow, Window* mainWindow, Window* settingsWindow);

#endif