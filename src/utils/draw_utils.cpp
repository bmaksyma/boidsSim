#include "draw_utils.h"
#include "mzapo_parlcd.h"
#include "fonts/font_types.h"   
#include <iostream>

#define WIDTH 480
#define HEIGHT 320

void draw_rect(unsigned short* fb, int x, int y, int w, int h, unsigned short int color) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int px = x + j;
            int py = y + i;
            if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
                int index = py * WIDTH + px;
                ((unsigned short*)fb)[index] = color;
            }
        }
    }
}

void draw_rounded_rect(unsigned short* fb, int x, int y, int w, int h, int radius, unsigned short int color) {
    for (int j = y + radius; j < y + h - radius; j++) {
        for (int i = x; i < x + w; i++) {
            draw_pixel(i, j, color);
        }
    }
    for (int j = y; j < y + radius; j++) {
        for (int i = x + radius; i < x + w - radius; i++) {
            draw_pixel(i, j, color);
        }
    }

    for (int j = y + h - radius; j < y + h; j++) {
        for (int i = x + radius; i < x + w - radius; i++) {
            draw_pixel(i, j, color);
        }
    }    
    for (int j = -radius; j <= radius; j++) {
        for (int i = -radius; i <= radius; i++) {
            if (i*i + j*j <= radius*radius) {
                draw_pixel(x + radius + i, y + radius + j, color);
                draw_pixel(x + w - radius + i, y + radius + j, color);
                draw_pixel(x + radius + i, y + h - radius + j, color);
                draw_pixel(x + w - radius + i, y + h - radius + j, color);
            }
        }
    }
}

