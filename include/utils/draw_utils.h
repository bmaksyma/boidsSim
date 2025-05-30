#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include <string>
#include <cstdint>
#include "fonts/font_types.h"
#include "fonts/font_manager.h"

/**
 * @brief Framebuffer pointer used for drawing pixels and shapes
 */
extern unsigned short *fb;

/**
 * @brief Draw one pixel at [x, y] with a color
 */
void draw_pixel(int x, int y, unsigned short int color);

/**
 * @brief Draw a filled rectangle
 */
void draw_rect(unsigned short* fb, int x, int y, int w, int h, unsigned short int color);

/**
 * @brief Draw a filled rectangle with rounded corners
 */
void draw_rounded_rect(unsigned short* fb, int x, int y, int w, int h, int radius, unsigned short int color);

#endif 
