#include <string>
#include <functional>
#include <cstdint>
#include <cstdint>
#include "font_types.h"
#include "font_manager.h"

#ifndef BUTTON_h
#define BUTTON_h
class Button {
public:
    int x, y, width, height;
    uint16_t color;
    std::string text;
    bool selected = false;

    std::function<void()> action;  
    
    Button(int x, int y, int w, int h, uint16_t color, const std::string &text, std::function<void()> action)
        : x(x), y(y), width(w), height(h), color(color), text(text), selected(false), action(action) {}

    void draw(unsigned short* fb, font_descriptor_t* font) ;

    void activate();
    // void draw_rect(unsigned short* fb, int x, int y, int w, int h, unsigned short int color) ;
};

#endif
