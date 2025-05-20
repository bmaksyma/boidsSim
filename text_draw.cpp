#include "text_draw.h"  

// Framebuffer must be defined externally

void draw_pixel(unsigned short* fb, int x, int y, unsigned short color) {
    if (x >= 0 && x < 480 && y >= 0 && y < 320) {
        fb[x + y * 480] = color;
    }
}

void draw_char(unsigned short* fb, int x, int y, font_descriptor_t* fdes, char ch, unsigned short color) {
    if (!fdes || !fdes->bits) return;
    if ((ch < fdes->firstchar) || (ch - fdes->firstchar >= fdes->size)) return;

    int index = ch - fdes->firstchar;
    int char_width = fdes->width ? fdes->width[index] : fdes->maxwidth;
    int char_height = fdes->height;

    const font_bits_t* bits = fdes->bits;

    // Calculate offset to this character's bitmap
    int char_offset = 0;
    for (int i = 0; i < index; i++) {
        char_offset += char_height;  // Each char has one 16-bit word per row
    }

    // For each row
    for (int row = 0; row < char_height; row++) {
        font_bits_t row_bits = bits[char_offset + row];
        for (int col = 0; col < char_width; col++) {
            // Start from MSB (bit 15) down
            if (row_bits & (1 << (15 - col))) {
                draw_pixel(fb, x + col, y + row, color);
            }
        }
    }
}



void draw_text(unsigned short* fb, int x, int y, font_descriptor_t* fdes, const char* text, unsigned short color) {
    while (*text) {
        char ch = *text;
        int index = ch - fdes->firstchar;
        if (index >= 0 && index < fdes->size) {
            draw_char(fb, x, y, fdes, ch, color);
            x += fdes->width ? fdes->width[index] : fdes->maxwidth;
        }
        text++;
    }
}

