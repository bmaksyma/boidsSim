#include <vector>
#include "Button.h"

#ifndef WINDOW_H
#define WINDOW_H

class Window {
public:
    std::vector<Button> buttons;
    int selected_index = 0;
    uint16_t background_color;
    Window(uint16_t bg = 0x0000) : background_color(bg) {}
    
    void nextBtn();
    void drawBtn(unsigned char* parlcd);
    void previousBtn();

    void activateSelected();
    void drawWindow(unsigned char* fb);
};

#endif