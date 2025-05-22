#ifndef TEXT_DRAW_H
#define TEXT_DRAW_H

#include "fonts/font_types.h"

// drawing single character at (x, y)
void draw_char(unsigned short* fb, int x, int y, font_descriptor_t* fdes, char ch, unsigned short color);

// drawing string starting at (x, y)
void draw_text(unsigned short* fb, int x, int y, font_descriptor_t* fdes, const char* text, unsigned short color);

#endif // TEXT_DRAWER_H
