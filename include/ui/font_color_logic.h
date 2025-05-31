#ifndef HANDLECOLORCHOICE_H
#define HANDLECOLORCHOICE_H

#include <vector>
#include <cstdint>
#include "Window.h"
#include "fonts/font_types.h"

/**
 * @brief Handles cycling through available fonts using knob rotation
 * 
 * This updates the selected font index and applies the font to the UI windows
 * 
 * @param delta Change in knob rotation 
 * @param selectedFontID Reference to the current font index, will be updated
 * @param availableFonts Vector of pointers to font descriptors
 * @param activeWindow Pointer to the currently active window
 * @param mainWindow Pointer to the main window
 * @param settingsWindow Pointer to the settings window
 */
void handleFontChoice(
    int8_t delta,
    int& selectedFontID, 
    std::vector<font_descriptor_t*>& availableFonts,
    Window* activeWindow,
    Window* mainWindow,
    Window* settingsWindow
);

/**
 * @brief Handles cycling through available UI themes using knob rotation
 * 
 * This updates the selected theme index and applies the theme to button colors and backgrounds.
 * 
 * @param delta Change in knob rotation
 * @param selectedThemeID Reference to the current theme index, that will be updated
 * @param buttonColors Reference to the vector storing button color scheme
 * @param activeWindow Pointer to the currently active window
 * @param mainWindow Pointer to the main window
 * @param settingsWindow Pointer to the settings window
 */
void handleThemeChoice(
    int8_t delta,
    int& selectedThemeID,
    std::vector<uint16_t>& buttonColors,
    Window* activeWindow,
    Window* mainWindow,
    Window* settingsWindow
);

#endif
