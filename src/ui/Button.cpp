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

    void Button::draw(unsigned short* fb, font_descriptor_t* font) {
        if (!font) {
            std::cerr << "Error: font is nullptr in Button::draw\n";
            return;
        }
        int scale = 2;
        uint16_t draw_color = selected ? 0xFFE0 : color;

        // Reuse draw_rect
        draw_rect(fb, x, y, width, height, draw_color);
        
        int text_width = text.length() * font->maxwidth;
        int text_height = font->height;

        int text_x = x + (width - text_width) / 2;
        int text_y = y + (height - text_height) / 2;
        draw_text(fb, 10, 10, &font_rom8x16, "Aquarium (c)", 0xFFFF, scale);
        
        draw_text(fb, text_x, text_y, font, text.c_str(), 0x0000, scale);
    

    }


// void draw_rect(unsigned short* fb, int x, int y, int w, int h, unsigned short int color)  {

//     for (int i = 0; i < w * h; ++i) {
//         fb[i] = color;
//     }

// }
// void draw_rect(unsigned short* fb, int x, int y, int w, int h, unsigned short color) {
//     for (int dy = 0; dy < h; ++dy) {
//         for (int dx = 0; dx < w; ++dx) {
//             int px = x + dx;
//             int py = y + dy;
//             if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
//                 fb[py * WIDTH + px] = color;
//             }
//         }
//     }
// }


