#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <time.h>

#include "font_manager.h"

#include "Window.h"
#include "Button.h"
#include "flushFrameBuffer.h"
#include "draw_utils.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "ui_setup.h"

// Screen dimensions
const int WIDTH = 480;
const int HEIGHT = 320;

const uint16_t COLOR_RED         = 0xF800;
const uint16_t COLOR_GREEN       = 0x07E0;
const uint16_t COLOR_BLUE        = 0x001F;
const uint16_t COLOR_MAGENTA     = 0xF81F;
const uint16_t COLOR_ORANGE      = 0xFD20;
const uint16_t COLOR_PINK        = 0xF81F;
const uint16_t COLOR_PURPLE      = 0x8010;
const uint16_t COLOR_NAVY        = 0x000F;
const uint16_t COLOR_LIME        = 0x07E0;
const uint16_t COLOR_LIGHT_BLUE  = 0xAEDC;

std::vector<uint16_t> buttonColors = {
    COLOR_RED, COLOR_LIME, COLOR_LIGHT_BLUE,
    COLOR_ORANGE, COLOR_MAGENTA
};

// Framebuffer
unsigned short *fb = nullptr;

// Draw a single pixel
void draw_pixel(int x, int y, unsigned short color) {
    if (color != 65535 && x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        fb[(x % WIDTH) + WIDTH * (y % HEIGHT)] = color;
    }
}

int main() {
    // Allocate framebuffer
    fb = (unsigned short *)malloc(WIDTH * HEIGHT * sizeof(unsigned short));
    if (!fb) {
        std::cerr << "Failed to allocate framebuffer\n";
        return 1;
    }

    // Initialize hardware
    unsigned char *parlcd_mem_base = (unsigned char *)map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if (!parlcd_mem_base) exit(1);

    unsigned char *mem_base = (unsigned char *)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (!mem_base) exit(1);

    parlcd_hx8357_init(parlcd_mem_base);

    // UI Setup
    Window mainWindow;
    Window settingsWindow;
    Window* activeWindow = &mainWindow;// pointer to the currently active window
    std::vector<std::string> colorNames = {
        "Red", "Green", "Blue", "Yellow", "Orange", "Purple"
    };
    std::vector<font_descriptor_t*> availableFonts = {
        &font_rom8x16,
        &font_winFreeSystem14x16
    };

    int selectedColorID = 0;       // User-fixed index
    uint16_t activeButtonColor = COLOR_GREEN;  // Default  button color

    bool choosingColor = false;   
    bool choosingFont = false; 
    int fontID = 0;

    mainWindow.background_color = COLOR_NAVY;
    settingsWindow.background_color = COLOR_NAVY;

    drawMainWindow(mainWindow, settingsWindow, activeWindow, activeButtonColor);
    drawSettingsWindow(mainWindow, settingsWindow, activeWindow, activeButtonColor);
    settingsWindow.buttons[0].selected = true;
    settingsWindow.selected_index = 0;

    mainWindow.current_font = availableFonts[0];
    settingsWindow.current_font = availableFonts[0];

    uint32_t knob_data = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    int last_green_knob = (knob_data >> 8) & 0xFF;
    bool last_green_knob_pressed = false;

    while (true) {
        knob_data = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);

        int green_knob = (knob_data >> 8) & 0xFF;
        bool red_knob_pressed   = (knob_data & 0x04000000) != 0; // bit 26
        bool green_knob_pressed = (knob_data & 0x02000000) != 0; // bit 25
        bool green_knob_just_pressed = green_knob_pressed && !last_green_knob_pressed;

        if (red_knob_pressed) {
            std::cout << "Exiting: red knob pressed\n";
            break;
        }
        // knob rotation delta
        int delta = green_knob - last_green_knob;
        if (delta > 128) delta -= 256;
        if (delta < -128) delta += 256;
        
        // Handle different modes based on current state
        if (choosingColor) {
            // In color selection mode
            if (delta != 0) {
                // Rotate through colors based on knob rotation
                if (delta > 0) {
                    selectedColorID = (selectedColorID + 1) % buttonColors.size();
                } else {
                    selectedColorID = (selectedColorID - 1 + buttonColors.size()) % buttonColors.size();
                }
                std::cout << "Selected Color: " << colorNames[selectedColorID] << "\n";
                // Immediately apply color to show preview on current active window
                activeButtonColor = buttonColors[selectedColorID];
                for (auto& btn : activeWindow->buttons) {
                    btn.color = activeButtonColor;
                }
            }
            // Press knob to confirm color selection and exit color mode
            if (green_knob_just_pressed) {
                choosingColor = false;
                std::cout << "Color confirmed and applied!\n";
                usleep(200 * 1000);
            }
        }
        else if (choosingFont) {
            // In font selection mode
            if (delta != 0) {
                // Rotate through fonts based on knob rotation
                if (delta > 0) {
                    fontID = (fontID + 1) % availableFonts.size();
                } else {
                    fontID = (fontID - 1 + availableFonts.size()) % availableFonts.size();
                }
                
                activeWindow->current_font = availableFonts[fontID];
                std::cout << "Selected Font index: " << fontID << "\n";
            }
            
            // Press knob to confirm font selection and exit font mode
            if (green_knob_just_pressed) {
                choosingFont = false;
                std::cout << "Font confirmed and applied!\n";
                usleep(200 * 1000);
            }
        }
        else {
            // Normal navigation mode
            if (delta != 0) {
                if (delta > 0) activeWindow->nextBtn();
                else if (delta < 0) activeWindow->previousBtn();
            }
            
            if (green_knob_just_pressed) {
                Button& selectedBtn = activeWindow->buttons[activeWindow->selected_index];
                
                if (selectedBtn.text == "Exit") {
                    break;
                }
                else if (selectedBtn.text == "Color") {
                    choosingColor = true;
                    std::cout << "Entering color selection mode - rotate knob to cycle colors, press to confirm\n";
                }
                else if (selectedBtn.text == "Font") {
                    choosingFont = true;
                    std::cout << "Entering font selection mode - rotate knob to cycle fonts, press to confirm\n";
                }
                else selectedBtn.activate();
                
                usleep(200 * 1000);
            }
        }
        // Update state for next iteration
        last_green_knob = green_knob;
        last_green_knob_pressed = green_knob_pressed;
        
        // Render the display
        
        for (int i = 0; i < WIDTH * HEIGHT; ++i)
            fb[i] = activeWindow->background_color;

        if (activeWindow && parlcd_mem_base) 
            activeWindow->drawWindow(parlcd_mem_base, fb);

        flushFramebuffer(parlcd_mem_base, fb);
    }


    flushFramebuffer(parlcd_mem_base, fb);
    free(fb);
    return 0;
}


/*
bool handled = false;

if (choosingColor && green_knob_pressed) {
    handled = true;
    if (delta > 0) selectedColorID = (selectedColorID + 1) % buttonColors.size();
    else if (delta < 0) selectedColorID = (selectedColorID - 1 + buttonColors.size()) % buttonColors.size();
    std::cout << "Selected Color: " << colorNames[selectedColorID] << "\n";
}

if (choosingFont && green_knob_pressed) {
    handled = true;
    if (delta > 0) fontID = (fontID + 1) % availableFonts.size();
    else if (delta < 0) fontID = (fontID - 1 + availableFonts.size()) % availableFonts.size();
    activeWindow->current_font = availableFonts[fontID];
    std::cout << "Selected Font index: " << fontID << "\n";
}

if (!handled) {
    if (delta > 0) activeWindow->nextBtn();
    else if (delta < 0) activeWindow->previousBtn();
}

*/
