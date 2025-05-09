#include <vector>
#include "Button.h"

#ifndef WINDOW_H
#define WINDOW_H

class Window {
public:
    std::vector<Button> buttons;
    int selected_index = 0;

    void nextBtn();
    void drawBtn(unsigned char* parlcd);
    void previousBtn();

    void activateSelected();
    void drawWindow(unsigned char* fb, uint16_t color);
};

#endif