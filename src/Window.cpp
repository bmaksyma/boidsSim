#include <vector>
#include <iostream>
#include "ui/Button.h"
#include "Window.h"
#include "utils/draw_utils.h"
#include "platform/mzapo_parlcd.h"
#include "utils/flushFrameBuffer.h"
#include "fonts/font_types.h"
// #include "font_manager.h"
#include "utils/text_draw.h"

// Global font list (can be shared across files)
// std::vector<font_descriptor_t*> available_fonts = {
//     &font_rom8x16,
//     &font_winFreeSystem14x16
// };

#define HEIGHT 320
#define WIDTH 480

void Window::activateSelected() {
    if (!buttons.empty())
        buttons[selected_index].activate();
}

void Window::previousBtn() {
    if (buttons.empty()) return;
    buttons[selected_index].selected = false;
    selected_index = (selected_index - 1 + buttons.size()) % buttons.size();
    buttons[selected_index].selected = true;
}

void Window::nextBtn() {
    if (buttons.empty()) return;
    buttons[selected_index].selected = false;
    selected_index = (selected_index + 1) % buttons.size();
    buttons[selected_index].selected = true;
}

void Window::addButton(Button button) {
    buttons.push_back(button);
    if (buttons.size() == 1) {
        buttons[0].selected = true;
        selected_index = 0;
    }
}

void Window::drawBtn(unsigned short* fb) {
    font_descriptor_t* fallback_font = available_fonts[0];
    for (Button& btn : buttons) {
        btn.draw(fb, fallback_font);
    }
}

void Window::drawWindow(unsigned char* parlcd_mem_base, unsigned short* fb) {
    if (!current_font) {
        std::cerr << "Error: Window::current_font is not set!\n";
        return;
    }
    
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        fb[i] = background_color;
    }

    for (auto& btn : buttons) {
        btn.draw(fb, current_font);
    }

    parlcd_write_cmd(parlcd_mem_base, 0x2C);
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        parlcd_write_data(parlcd_mem_base, fb[i]);
    }
}
