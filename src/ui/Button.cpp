#include <string>
#include <functional>
#include <cstdint>
#include "Button.h"
#include "font_types.h"
#include "utils/draw_utils.h"
#include "mzapo_parlcd.h"
// #include "font_manager.h"
#include "text_draw.h"
#include <iostream>

const int WIDTH = 480;
const int HEIGHT = 320;
void Button::activate() {
    if (action) {
        action();
    }
}

// void Button::draw(unsigned short* fb, font_descriptor_t* font) {
//     if (!font) {
//         std::cerr << "Error: font is nullptr in Button::draw\n";
//         return;
//     }
//     int scale = 2;
//     uint16_t draw_color = selected ?  : color;

//     // Reuse draw_rect
//     draw_rect(fb, x, y, width, height, draw_color);
    
//     int text_width = text.length() * font->maxwidth;
//     int text_height = font->height;

//     int text_x = x + (width - text_width) / 2;
//     int text_y = y + (height - text_height) / 2;
//     draw_text(fb, 10, 10, &font_rom8x16, "Aquarium (c)", 0xFFFF, scale);
    
//     draw_text(fb, text_x, text_y, font, text.c_str(), 0x0000, scale);
// }

void Button::draw(unsigned short* fb, font_descriptor_t* font) {
    if (!font) {
        std::cerr << "Error: font is nullptr in Button::draw\n";
        return;
    }
    
    int scale = 2;
    int corner_radius = 7;
    
    if (selected) {
        int strS = 4;
        int8_t red = (color >> 11) & 0x1F;
        uint8_t green = (color >> 5) & 0x3F;
        uint8_t blue = color & 0x1F;
        uint8_t inv_red = 0x1F - red;
        uint8_t inv_green = 0x3F - green;
        uint8_t inv_blue = 0x1F - blue;
        uint16_t new_color = (inv_red << 11) | (inv_green << 5);
        draw_rounded_rect(fb, x - strS, y - strS, width + 2*strS, height + 2*strS, corner_radius + strS, new_color);
    }
    draw_rounded_rect(fb, x, y, width, height, corner_radius, color);
    
    int text_width = text.length() * font->maxwidth;
    int text_height = font->height;
    
    int text_x = x + (width - text_width) / 2 - font->maxwidth;
    int text_y = y + (height - text_height) / 2;
    
    draw_text(fb, text_x, text_y, font, text.c_str(), 0x0000, scale);
}


