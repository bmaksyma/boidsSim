#include "text_draw.h"  

void draw_pixel(unsigned short* fb, int x, int y, unsigned short color) {
    if (x >= 0 && x < 480 && y >= 0 && y < 320) {
        fb[x + y * 480] = color;
    }
}

void draw_char(unsigned short* fb, int x, int y, font_descriptor_t* fdes, char ch, unsigned short color, int scale) {
    if (!fdes || !fdes->bits) return;
    if ((ch < fdes->firstchar) || (ch - fdes->firstchar >= fdes->size)) return;

    int index = ch - fdes->firstchar;
    int char_width = fdes->width ? fdes->width[index] : fdes->maxwidth;
    int char_height = fdes->height;
    const font_bits_t* bits = fdes->bits;

    
    int char_offset = index * char_height;

    // For each row
    for (int row = 0; row < char_height; row++) {
        font_bits_t row_bits = bits[char_offset + row];
        for (int col = 0; col < char_width; col++) {
            if (row_bits & (1 << (15 - col))) {
                // Prints a block of pixels
                for (int dy = 0; dy < scale; dy++) {
                    for (int dx = 0; dx < scale; dx++) {
                        draw_pixel(fb, x + col * scale + dx, y + row * scale + dy, color);
                    }
                }

            }
        }
    }
}

void draw_text(unsigned short* fb, int x, int y, font_descriptor_t* fdes, const char* text, unsigned short color, int scale) {
    while (*text) {
        char ch = *text;
        int index = ch - fdes->firstchar;
        if (index >= 0 && index < fdes->size) {
            draw_char(fb, x, y, fdes, ch, color, scale);
            int char_width = fdes->width ? fdes->width[index] : fdes->maxwidth;
            x += (char_width * scale) + scale;  //  spacing
        }
        text++;
    }
}

