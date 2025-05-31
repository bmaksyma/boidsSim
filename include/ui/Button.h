#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <functional>
#include <cstdint>
#include "fonts/font_types.h"

/**
 * @class Button
 * @brief Represents a UI button element that can be drawn on the screen and activated
 * 
 * This class defines a basic interactive button component that:
 * - Displays labeled text with color and optional selection highlight
 * - Triggers a user-defined callback when activated
 * - Supports custom font rendering and visual scaling
 */
class Button {
public:
    /**
     * @brief Default constructor for Button, initializes with default values
     * @variable x, y, width, height, color, text, and bool selected
     */
    int x;                      
    int y;                      
    int width;                 
    int height;                
    uint16_t color;             
    std::string text;           
    bool selected = false;      

    /**
     * @brief Callback function to execute when the button is activated
     */
    std::function<void()> action;

    /**
     * @brief Constructor for a new Button
     * 
     * @param x X-coordinate of the button on screen
     * @param y Y-coordinate of the button on screen
     * @param w Width of the button
     * @param h Height of the button
     * @param color Button background color
     * @param text Text label shown on the button
     * @param action Function to execute on activation
     */
    Button(int x, int y, int w, int h, uint16_t color, const std::string& text, std::function<void()> action);

    /**
     * @brief Draws the button to the framebuffer using the specified font
     * 
     * @param fb Pointer to the framebuffer
     * @param font Pointer to the font descriptor
     */
    void draw(unsigned short* fb, font_descriptor_t* font);

    /**
     * @brief Activates the button's action
     */
    void activate();
};

#endif 