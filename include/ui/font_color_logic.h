#ifndef HANDLECOLORCHOICE_H
#define HANDLECOLORCHOICE_H

#include <vector>
#include <cstdint>
#include "Window.h"

/**
 * @brief Handles color selection using knob input
 * 
 * Changes the selected color and updates all button colors
 */
void handleColorChoice(int8_t delta, int& selectedColorID, uint16_t& activeButtonColor,
    std::vector<uint16_t>& buttonColors,
    Window* activeWindow, Window* mainWindow, Window* settingsWindow);

/**
 * @brief Handles font selection using knob input
 * 
 * Changes the current font in all windows
 */
void handleFontChoice(int8_t delta, int& selectedFontID,
    std::vector<font_descriptor_t*>& availableFonts,
    Window* activeWindow, Window* mainWindow, Window* settingsWindow);

#endif
