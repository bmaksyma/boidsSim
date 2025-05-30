#ifndef HANDLECOLORCHOICE_H
#define HANDLECOLORCHOICE_H

#include <vector>
#include <cstdint>
#include "Window.h"

<<<<<<< HEAD
/**
 * @brief Handles color selection using knob input
 * 
 * Changes the selected color and updates all button colors
 */
=======
#ifndef HANDLECOLORCHOICE_H
#define HANDLECOLORCHOICE_H

<<<<<<< HEAD
=======
>>>>>>> 80cfabae661db1298bfeb21cd3b2285444112c24
void handleColorChoice(int8_t delta, int& selectedColorID, uint16_t& activeButtonColor,
    std::vector<uint16_t>& buttonColors,
    Window* activeWindow, Window* mainWindow, Window* settingsWindow);

<<<<<<< HEAD
/**
 * @brief Handles font selection using knob input
 * 
 * Changes the current font in all windows
 */
void handleFontChoice(int8_t delta, int& selectedFontID,
    std::vector<font_descriptor_t*>& availableFonts,
    Window* activeWindow, Window* mainWindow, Window* settingsWindow);

#endif
=======
>>>>>>> 11130fa8575f0b72df754e120a1f9af013e2b6cf
void handleFontChoice(int8_t delta, int& selectedFontID, 
    std::vector<font_descriptor_t*>& availableFonts,Window* activeWindow,Window* mainWindow, Window* settingsWindow);

void handleThemeChoice(int8_t delta, int& selectedThemeID,
    std::vector<uint16_t>& buttonColors, Window* activeWindow, Window* mainWindow, Window* settingsWindow);
<<<<<<< HEAD
#endif
=======

#endif
>>>>>>> 80cfabae661db1298bfeb21cd3b2285444112c24
>>>>>>> 11130fa8575f0b72df754e120a1f9af013e2b6cf
