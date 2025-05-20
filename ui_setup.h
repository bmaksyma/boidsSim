#ifndef UI_SETUP_H
#define UI_SETUP_H


void drawMainWindow(Window& mainWindow, Window& settingsWindow,Window*& activeWindow, uint16_t activeButtonColor);
void drawSettingsWindow(Window& mainWindow, Window& settingsWindow, Window*& activeWindow, uint16_t activeButtonColor, bool& choosingColor, bool& choosingFont);

#endif