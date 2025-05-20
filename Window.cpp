#include <vector>
#include <iostream>
#include "Button.h"
#include "Window.h"
#include "draw_utils.h"
#include "mzapo_parlcd.h"
#include "flushFrameBuffer.h"
#include "font_types.h"
// #include "font_manager.h"
#include "text_draw.h"

// Global font list (can be shared across files)
// std::vector<font_descriptor_t*> available_fonts = {
//     &font_rom8x16,
//     &font_winFreeSystem14x16
// };

// Display size
#define HEIGHT 320
#define WIDTH 480

// Activates the currently selected button
void Window::activateSelected() {
    if (!buttons.empty())
        buttons[selected_index].activate();
}

// Moves selection to the previous button
void Window::previousBtn() {
    if (buttons.empty()) return;
    buttons[selected_index].selected = false;
    selected_index = (selected_index - 1 + buttons.size()) % buttons.size();
    buttons[selected_index].selected = true;
}

// Moves selection to the next button
void Window::nextBtn() {
    if (buttons.empty()) return;
    buttons[selected_index].selected = false;
    selected_index = (selected_index + 1) % buttons.size();
    buttons[selected_index].selected = true;
}

// Adds a button and sets it selected if it's the first
void Window::addButton(Button button) {
    buttons.push_back(button);
    if (buttons.size() == 1) {
        buttons[0].selected = true;
        selected_index = 0;
    }
}

// Optional: draws all buttons with default font (unused)
void Window::drawBtn(unsigned short* fb) {
    font_descriptor_t* fallback_font = available_fonts[0];
    for (Button& btn : buttons) {
        btn.draw(fb, fallback_font);
    }
}

// Main draw function for the window
void Window::drawWindow(unsigned char* parlcd_mem_base, unsigned short* fb) {
    if (!current_font) {
        std::cerr << "Error: Window::current_font is not set!\n";
        return;
    }
    

    // Fill screen with background color
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        fb[i] = background_color;
    }

    // Draw all buttons using the current font
    for (auto& btn : buttons) {
        btn.draw(fb, current_font);
    }

    // Flush framebuffer to LCD
    parlcd_write_cmd(parlcd_mem_base, 0x2C);
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        parlcd_write_data(parlcd_mem_base, fb[i]);
    }
}
