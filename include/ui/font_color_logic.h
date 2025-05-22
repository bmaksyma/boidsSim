#include <vector>
#include <cstdint>
#include "Window.h"


#ifndef HANDLECOLORCHOICE_H
#define HANDLECOLORCHOICE_H


void handleColorChoice(int delta, int& selectedColorID, uint16_t& activeButtonColor,
    std::vector<uint16_t>& buttonColors,
    Window* activeWindow);

void handleFontChoice(int delta, int& selectedFontID, 
    std::vector<font_descriptor_t*>& availableFonts,Window* activeWindow);
#endif  