#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include "ui/Button.h"
#include "fonts/font_manager.h"

/**
 * @brief UI window that holds buttons and handles drawing and button selection
 */
class Window {
public:
    std::vector<Button> buttons;      
    int selected_index = 0;            
    uint16_t background_color = 0x0000;
    int current_font_index = 0;       
    font_descriptor_t* current_font = &font_rom8x16; 

    // Create a window with an optional background color
    Window(uint16_t bg = 0x0000) : background_color(bg) {}

    void nextBtn();

    void previousBtn();

    void addButton(Button button);

    void activateSelected();

    void drawBtn(unsigned short* fb);

    // Draw window
    void drawWindow(unsigned char* parlcd_mem_base, unsigned short* fb);
};

#endif
