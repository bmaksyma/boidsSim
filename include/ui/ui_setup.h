#ifndef UI_SETUP_H
#define UI_SETUP_H

#include <cstdint>
#include "Window.h"

void drawMainWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow, uint16_t activeButtonColor, bool& choosingFont, bool& choosingTheme);
void drawSettingsWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow, uint16_t activeButtonColor, bool& choosingFont, bool& choosingTheme);
void drawExitWindow(Window& exitWindow);
#endif