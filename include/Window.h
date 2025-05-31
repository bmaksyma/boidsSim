#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include <cstdint>
#include "ui/Button.h"
#include "fonts/font_manager.h"

/**
 * @brief Represents a UI window containing interactive buttons 
 *        and managing their rendering and selection
 */
class Window {
public:
    std::vector<Button> buttons;       
    int selected_index = 0;             
    uint16_t background_color = 0x0000; 
    int current_font_index = 0;         
    font_descriptor_t* current_font = &font_rom8x16; 

    /**
     * @brief Construct a new Window with an optional background color
     * 
     * @param bg Background color (default black)
     */
    Window(uint16_t bg = 0x0000);

    /**
     * @brief Move selection to the next button
     */
    void nextBtn();

    /**
     * @brief Move selection to the previous button
     */
    void previousBtn();

    /**
     * @brief Add a button to the window
     * 
     * @param button The button to add
     */
    void addButton(Button button);

    /**
     * @brief Executes activation of the currently selected button
     */
    void activateSelected();

    /**
     * @brief Draw all buttons in the window using the fallback font
     * 
     * @param fb Pointer to the framebuffer
     */
    void drawBtn(unsigned short* fb);

    /**
     * @brief Clear the screen and draw all buttons to the framebuffer and LCD
     * 
     * @param parlcd_mem_base Pointer to parlcd memory base 
     * @param fb Pointer to framebuffer
     */
    void drawWindow(unsigned char* parlcd_mem_base, unsigned short* fb);
};

#endif 