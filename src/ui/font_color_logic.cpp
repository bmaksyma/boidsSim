#include "font_color_logic.h"
#include "ui/theme_colors.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include "Window.h"

void handleColorChoice(int8_t delta, int& selectedColorID, uint16_t& activeButtonColor,
    std::vector<uint16_t>& buttonColors,
    Window* activeWindow,Window* mainWindow, Window* settingsWindow){
    if (delta != 0) {
        if (delta > 2) {
            selectedColorID = (selectedColorID + 1) % buttonColors.size();
        } else if (delta < -2) {
            selectedColorID = (selectedColorID - 1 + buttonColors.size()) % buttonColors.size();
        }
        std::cout << "Selected Color: " << buttonColors[selectedColorID] << "\n";
        activeButtonColor = buttonColors[selectedColorID];
        for (auto& btn : mainWindow->buttons) {
            btn.color = activeButtonColor;
        }
        for (auto& btn : settingsWindow->buttons) {
            btn.color = activeButtonColor;
        }
    }
}

void handleFontChoice(int8_t delta, int& selectedFontID, 
    std::vector<font_descriptor_t*>& availableFonts,Window* activeWindow,Window* mainWindow, Window* settingsWindow){
             
    if (delta != 0) {
        if (delta > 2) {
            selectedFontID = (selectedFontID + 1) % availableFonts.size();
        } else if (delta < -2) {
            selectedFontID = (selectedFontID - 1 + availableFonts.size()) % availableFonts.size();
        }
        
        activeWindow->current_font = availableFonts[selectedFontID];
        mainWindow->current_font = availableFonts[selectedFontID];
        settingsWindow->current_font = availableFonts[selectedFontID];
        std::cout << "Selected Font index: " << selectedFontID << "\n";
    }
}

void handleThemeChoice(int8_t delta, int& selectedThemeID,
    std::vector<uint16_t>& buttonColors, Window* activeWindow, Window* mainWindow, Window* settingsWindow) {
    
    if (delta != 0) {
        if (delta > 2) {
            selectedThemeID = (selectedThemeID + 1) % themeNames.size();
        } else if (delta < -2) {
            selectedThemeID = (selectedThemeID - 1 + themeNames.size()) % themeNames.size();
        }
        switchToTheme(selectedThemeID);
        buttonColors = getCurrentThemeColors();
        
        uint16_t newBgColor = getCurrentBackgroundColor();
        mainWindow->background_color = newBgColor;
        settingsWindow->background_color = newBgColor;
        activeWindow->background_color = newBgColor;
        
        for (auto& btn : mainWindow->buttons) {
            btn.color = buttonColors[0];
        }
        for (auto& btn : settingsWindow->buttons) {
            btn.color = buttonColors[0];
        }
        
        std::cout << "Selected Theme: " << themeNames[selectedThemeID] << "\n";
    }
}