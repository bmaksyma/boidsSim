#include <vector>
#include "Button.h"
#include "Window.h"

#include "mzapo_parlcd.h"

#define HEIGHT 320
#define WIDTH 480
void Window::activateSelected() {
    buttons[selected_index].activate();
}

//changes selected button to a previous one 
void Window::previousBtn() {
    buttons[selected_index].selected = false;
    selected_index = (selected_index - 1 + buttons.size()) % buttons.size();
    buttons[selected_index].selected = true;
}

void Window::nextBtn() {
    buttons[selected_index].selected = false;
    selected_index = (selected_index + 1) % buttons.size();
    buttons[selected_index].selected = true;
}

void Window::drawBtn(unsigned char* fb) {
    
    
    parlcd_write_cmd(fb, 0x2C);  // Start RAM write
    for (Button& btn : buttons) {
        for (int i = 0; i < btn.height; i++) {
            for (int j = 0; j < btn.width; j++) {
                uint16_t color = btn.selected ? 0xFFFF : btn.color;
                int screen_x = btn.x + j;
                int screen_y = btn.y + i;

                // Only draw inside screen bounds
                if (screen_x >= 0 && screen_x < WIDTH && screen_y >= 0 && screen_y < HEIGHT) {
                    parlcd_write_data(fb, color);
                }
            }
        }
    }
}

void Window:: drawWindow(unsigned char * fb){

    uint16_t bg_color = 0x0000;

    parlcd_write_cmd(fb, 0x2C);
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            parlcd_write_data(fb, bg_color);
        }
    }
    drawBtn(fb);

}


