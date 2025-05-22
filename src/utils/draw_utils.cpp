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




