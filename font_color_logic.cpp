#include "font_color_logic.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include "Window.h"



void handleColorChoice(int delta, int& selectedColorID, unit16_t& activeButtonColor,
    std::vector<uint16_t>& buttonColors,
    Window* activeWindow){
    if (delta != 0) {
        // Rotate through colors based on knob rotation
        if (delta > 0) {
            selectedColorID = (selectedColorID + 1) % buttonColors.size();
        } else {
            selectedColorID = (selectedColorID - 1 + buttonColors.size()) % buttonColors.size();
        }
        std::cout << "Selected Color: " << buttonColors[selectedColorID] << "\n";
        // Immediately apply color to show preview on current active window
        activeButtonColor = buttonColors[selectedColorID];
        for (auto& btn : activeWindow->buttons) {
            btn.color = activeButtonColor;
        }
    }
}

