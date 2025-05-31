#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include <string>
#include <cstdint>
#include "fonts/font_types.h"
#include "fonts/font_manager.h"

/**
 * @brief Framebuffer pointer used to directly write pixel data for rendering
 * 
 * Points to an initialized framebuffer for drawing
 */
extern unsigned short *fb;

/**
 * @brief Draw a single pixel on the screen
 * 
 * @param x X-coordinate of the pixel
 * @param y Y-coordinate of the pixel
 * @param color 16-bit RGB color value
 */
void draw_pixel(int x, int y, unsigned short int color);

/**
 * @brief Draw a filled rectangle at the specified position and dimensions
 * 
 * @param fb Pointer to the framebuffer
 * @param x Top-left X-coordinate
 * @param y Top-left Y-coordinate
 * @param w Width of the rectangle
 * @param h Height of the rectangle
 * @param color 16-bit RGB565 fill color
 */
void draw_rect(unsigned short* fb, int x, int y, int w, int h, unsigned short int color);

/**
 * @brief Draw a filled rectangle with rounded corners
 * 
 * This function fills the rectangle and renders rounded corners using circles
 * 
 * @param fb Pointer to the framebuffer
 * @param x Top-left X-coordinate
 * @param y Top-left Y-coordinate
 * @param w Width of the rectangle
 * @param h Height of the rectangle
 * @param radius Radius of the corner rounding
 * @param color 16-bit RGB fill color
 */
void draw_rounded_rect(unsigned short* fb, int x, int y, int w, int h, int radius, unsigned short int color);

#endif 
