#include <vector>
#include "Button.h"
#include "font_manager.h"
#ifndef WINDOW_H
#define WINDOW_H

class Window {
public:
    std::vector<Button> buttons;
    int selected_index = 0;
    uint16_t background_color =  0x0000;
    Window(uint16_t bg = 0x0000) : background_color(bg) {}

    int current_font_index = 0;
    
    font_descriptor_t* current_font = &font_rom8x16;
  
    void nextBtn();
    void drawBtn(unsigned short* fb);
    void previousBtn();
    void addButton( Button button);
    
    void activateSelected();
    void drawWindow(unsigned char* parlcd_mem_base, unsigned short* fb);
};

#endif