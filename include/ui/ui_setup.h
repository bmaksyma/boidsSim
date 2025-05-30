#ifndef UI_SETUP_H
#define UI_SETUP_H

#include <cstdint>
#include "Window.h"

<<<<<<< HEAD
void drawMainWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow, uint16_t activeButtonColor, bool& choosingFont, bool& choosingTheme);
void drawSettingsWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow, uint16_t activeButtonColor, bool& choosingFont, bool& choosingTheme);
=======
<<<<<<< HEAD
/**
 * @brief Sets up the main menu UI 
 * 
 * @param mainWindow Reference to the main window
 * @param settingsWindow Reference to the settings window
 * @param activeWindow Pointer to the currently active window
 */
void drawMainWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow,
                    uint16_t activeButtonColor, bool& choosingColor, bool& choosingFont);

/**
 * @brief Sets up the settings menu 
 * 
 * @param mainWindow Reference to the main window
 * @param settingsWindow Reference to the settings window
 * @param activeWindow Pointer to the currently active window
 */
void drawSettingsWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow,
                        uint16_t activeButtonColor, bool& choosingColor, bool& choosingFont);

#endif
=======
void drawMainWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow, uint16_t activeButtonColor, bool& choosingColor, bool& choosingFont, bool& choosingTheme);
void drawSettingsWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow, uint16_t activeButtonColor, bool& choosingColor, bool& choosingFont, bool& choosingTheme);
>>>>>>> 80cfabae661db1298bfeb21cd3b2285444112c24
>>>>>>> 11130fa8575f0b72df754e120a1f9af013e2b6cf
