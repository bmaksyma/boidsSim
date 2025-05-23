#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H
#include <string>
#include <cstdint>
#include "fonts/font_types.h"
#include "fonts/font_manager.h"

extern unsigned short *fb;

void draw_pixel(int x, int y, unsigned short int color);
void draw_rect(unsigned short* fb, int x, int y, int w, int h, unsigned short int color);
void draw_rounded_rect(unsigned short* fb, int x, int y, int w, int h, int radius, unsigned short int color);
// void draw_text(unsigned short* fb, int x, int y,font_descriptor_t* font, const std::string &text, uint16_t color);

#endif