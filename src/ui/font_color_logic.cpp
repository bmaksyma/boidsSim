#include "font_color_logic.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include "Window.h"



void handleColorChoice(int8_t delta, int& selectedColorID, uint16_t& activeButtonColor,
    std::vector<uint16_t>& buttonColors,
    Window* activeWindow,Window* mainWindow, Window* settingsWindow){
    if (delta != 0) {
        // Rotate through colors based on knob rotation
        if (delta > 2) {
            selectedColorID = (selectedColorID + 1) % buttonColors.size();
        } else {
            selectedColorID = (selectedColorID - 1 + buttonColors.size()) % buttonColors.size();
        }
        std::cout << "Selected Color: " << buttonColors[selectedColorID] << "\n";
        // Immediately apply color to show preview on current active window
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
        // Knob rotation to go through fonts
        if (delta > -2) {
            selectedFontID = (selectedFontID + 1) % availableFonts.size();
        } else 
            selectedFontID = (selectedFontID - 1 + availableFonts.size()) % availableFonts.size();
        
        activeWindow->current_font = availableFonts[selectedFontID];
        std::cout << "Selected Font index: " << selectedFontID << "\n";
    }
    mainWindow->current_font = availableFonts[selectedFontID];
    settingsWindow->current_font = availableFonts[selectedFontID];
        
}
