#include <string>
#include <functional>
#include <cstdint>
#include "Button.h"
#include "font_types.h"
#include "utils/draw_utils.h"
#include "mzapo_parlcd.h"
#include "text_draw.h"
#include "ui/theme_colors.h"

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
    int corner_radius = 7;
    int strS = 4;
    uint16_t bd_color = getCurrentBackgroundColor();
    uint16_t text_color = (bd_color == 0xFFFF) ? 0x0000 : 0xFFFF;
    if (selected) {
        draw_rounded_rect(fb, x - strS, y - strS, width + 2*strS, height + 2*strS, corner_radius + strS, text_color);
    }
    draw_rounded_rect(fb, x, y, width, height, corner_radius, color);
    
    int text_height = font->height;
    int text_width = 0;
    for (size_t i = 0; i < text.length(); ++i) {
        int idx = text[i] - font->firstchar;
        int char_width = (font->width && idx >= 0 && idx < font->size) ? font->width[idx] : font->maxwidth;
        text_width += char_width * scale;
        text_width += scale; 
    }
    
    int text_x = x + (width - text_width) / 2;
    int text_y = y + (height - text_height) / 2;
    
    draw_text(fb, text_x, text_y, font, text.c_str(), text_color, scale);
}


