#ifndef TEXT_DRAW_H
#define TEXT_DRAW_H

#include "fonts/font_types.h"

/**
 * @brief Draws one character at a given position
 * 
 * @param fb Framebuffer
 * @param x X position
 * @param y Y position
 * @param fdes Font to use
 * @param ch Character to draw
 * @param color Text color
 * @param scale Text size index
 */
void draw_char(unsigned short* fb, int x, int y, font_descriptor_t* fdes, char ch, unsigned short color, int scale);

/**
 * @brief Draws a text string starting at a position
 * 
 * @param fb Framebuffer
 * @param x X position
 * @param y Y position
 * @param fdes Font to use
 * @param text String to draw
 * @param color Text color
 * @param scale Text size index
 */
void draw_text(unsigned short* fb, int x, int y, font_descriptor_t* fdes, const char* text, unsigned short color, int scale);

#endif 
