#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <functional>
#include <cstdint>
#include "fonts/font_types.h"

/**
 * @brief UI button that can display text and do an action when activated
 */
class Button {
public:
    int x, y;                 
    int width, height;         
    uint16_t color;            
    std::string text;          
    bool selected = false;     

    std::function<void()> action;

    /**
     * @brief Creates a new button
     */
    Button(int x, int y, int w, int h, uint16_t color, const std::string& text, std::function<void()> action)
        : x(x), y(y), width(w), height(h), color(color), text(text), selected(false), action(action) {}

    /**
     * @brief Draws the button
     */
    void draw(unsigned short* fb, font_descriptor_t* font);

    void activate();
};

#endif 
