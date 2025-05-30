#ifndef UI_SETUP_H
#define UI_SETUP_H

#include <cstdint>
#include "Window.h"

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
