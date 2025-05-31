#ifndef UI_SETUP_H
#define UI_SETUP_H

#include <cstdint>
#include "Window.h"

/**
 * @brief Draws the main UI window with buttons for starting the simulation, 
 *                                              opening settings, or exiting
 * 
 * @param mainWindow Reference to the main window object
 * @param settingsWindow Reference to the settings window (to switch)
 * @param activeWindow Reference to a pointer that controls the currently active window
 * @param activeButtonColor Color to be applied for the buttons
 * @param choosingFont Reference to a flag controlling font selection mode
 * @param choosingTheme Reference to a flag controlling theme selection mode
 */
void drawMainWindow(
    Window& mainWindow,
    Window& settingsWindow,
    Window*& activeWindow,
    uint16_t activeButtonColor,
    bool& choosingFont,
    bool& choosingTheme
);

/**
 * @brief Draws the settings window UI, allowing users to select fonts or themes.
 * 
 * @param mainWindow Reference to the main window (to switch to mainMenu)
 * @param settingsWindow Reference to the settings window being drawn
 * @param activeWindow Reference to a pointer that controls the currently active window
 * @param activeButtonColor Color to be used for the buttons
 * @param choosingFont Reference to a flag controlling font selection mode
 * @param choosingTheme Reference to a flag controlling theme selection mode
 */
void drawSettingsWindow(
    Window& mainWindow,
    Window& settingsWindow,
    Window*& activeWindow,
    uint16_t activeButtonColor,
    bool& choosingFont,
    bool& choosingTheme
);

/**
 * @brief Draws a simple exit screen with a centered "EXITING..." message.
 * 
 * @param exitWindow Reference to the exit window being drawn
 */
void drawExitWindow(Window& exitWindow);

#endif 
