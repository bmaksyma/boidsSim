#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <time.h>

#include "fonts/font_manager.h"

#include "Window.h"
#include "ui/Button.h"
#include "utils/flushFrameBuffer.h"
#include "utils/draw_utils.h"
#include "platform/mzapo_parlcd.h"
#include "platform/mzapo_phys.h"
#include "platform/mzapo_regs.h"
#include "ui/ui_setup.h"
#include "font_color_logic.h"
#include "simulation/Simulation.hpp"

extern const int WIDTH = 480;
extern const int HEIGHT = 320;
extern const float MAX_SPEED = 10.0f;
extern const float MAX_FORCE = 1.3f;
extern const float PERCEPTION_RADIUS = 25.0f;
extern const float SEPARATION_RADIUS = 10.0f;
extern const float FOOD_ATTRACTION = 0.8f;
extern const float ALIGNMENT_WEIGHT = 1.0f;
extern const float COHESION_WEIGHT = 1.0f;
extern const float SEPARATION_WEIGHT = 1.5f;
extern const float MARGIN_SIZE = 40.0f;
extern const float TURN_FORCE = 5.0f;
extern const float PREY_SIZE = 10.0f;
extern const float PREDATOR_SIZE = 20.0f;
extern const int PREYS_COUNT = 100;
extern const int PREDATORS_COUNT = 5;
extern const float KILL_DISTANCE = 5.0f;
extern font_descriptor_t* CUR_FONT = &font_rom8x16;
extern uint16_t PREY_COLOR = 0x001F; 
extern uint16_t PREDATOR_COLOR = 0xF800;

const uint16_t COLOR_RED = 0xF800;
const uint16_t COLOR_GREEN = 0x07E0;
const uint16_t COLOR_MAGENTA = 0xF81F;
const uint16_t COLOR_ORANGE = 0xFD20;
const uint16_t COLOR_NAVY = 0x000F;
const uint16_t COLOR_LIME = 0x07E0;
const uint16_t COLOR_LIGHT_BLUE = 0xAEDC;

std::vector<uint16_t> buttonColors = {
    COLOR_RED, COLOR_LIME, COLOR_LIGHT_BLUE,
    COLOR_ORANGE, COLOR_MAGENTA
};
std::vector<font_descriptor_t*> availableFonts = {
    &font_rom8x16,
    &font_winFreeSystem14x16
};

extern unsigned short *fb = nullptr;

void draw_pixel(int x, int y, unsigned short color) {
    if (color != 65535 && x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        fb[(x % WIDTH) + WIDTH * (y % HEIGHT)] = color;
    }
}

int main() {
    fb = (unsigned short *)malloc(WIDTH * HEIGHT * sizeof(unsigned short));
    if (!fb) {
        std::cerr << "Failed to allocate framebuffer\n";
        return 1;
    }

    unsigned char *parlcd_mem_base = (unsigned char *)map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if (!parlcd_mem_base) exit(1);

    unsigned char *mem_base = (unsigned char *)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (!mem_base) exit(1);

    parlcd_hx8357_init(parlcd_mem_base);

    // UI Setup
    Window mainWindow;
    Window settingsWindow;
    Window* activeWindow = &mainWindow;
    std::vector<std::string> colorNames = {
        "Red", "Green", "Blue", "Yellow", "Orange", "Purple"
    };

    int selectedColorID = 0;
    uint16_t activeButtonColor = COLOR_GREEN;

    bool choosingColor = false;   
    bool choosingFont = false;
    bool simRunning = false;
    int fontID = 0;

    mainWindow.background_color = COLOR_NAVY;
    settingsWindow.background_color = COLOR_NAVY;

    drawMainWindow(mainWindow, settingsWindow, activeWindow, activeButtonColor, choosingColor, choosingFont);
    drawSettingsWindow(mainWindow, settingsWindow, activeWindow, activeButtonColor, choosingColor, choosingFont);
    settingsWindow.buttons[0].selected = true;
    settingsWindow.selected_index = 0;

    mainWindow.current_font = availableFonts[0];
    settingsWindow.current_font = availableFonts[0];

    const unsigned int FRAME_TIME_US = 16667; // time to obtain 60 FPS

    uint32_t knob_data = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    int8_t last_green_knob = (knob_data >> 8) & 0xFF;
    bool last_green_knob_pressed = false;

    while (true) {
        struct timespec start_time;
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        
        knob_data = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);

        int8_t green_knob = (knob_data >> 8) & 0xFF;
        bool red_knob_pressed = (knob_data & 0x04000000) != 0; 
        bool green_knob_pressed = (knob_data & 0x02000000) != 0; 
        bool green_knob_just_pressed = green_knob_pressed && !last_green_knob_pressed;

        if (red_knob_pressed) {
            std::cout << "Exiting: red knob pressed\n";
            break;
        }
        int8_t delta = green_knob - last_green_knob;
        if (choosingColor) {
            handleColorChoice(delta, selectedColorID, activeButtonColor, buttonColors, activeWindow, &mainWindow, &settingsWindow);
            if (green_knob_just_pressed) {
                choosingColor = false;
                PREY_COLOR = buttonColors[selectedColorID];
                PREDATOR_COLOR = buttonColors[selectedColorID -1];
                std::cout << "Color confirmed and applied!\n";
                usleep(200 * 1000);
            }
        }
        else if (choosingFont) {
            // Font selection mode
            handleFontChoice(delta, fontID, availableFonts, activeWindow, &mainWindow, &settingsWindow);
            CUR_FONT = availableFonts[fontID];
            if (green_knob_just_pressed) {
                choosingFont = false;
                std::cout << "Font confirmed and applied!\n";
                usleep(200 * 1000);
            }
        } else {
            // Normal choosing mode
            if (delta != 0) {
                if (delta >= (int8_t)2) activeWindow->nextBtn();
                else if (delta <= (int8_t)(-2)) activeWindow->previousBtn();
            }
            if (green_knob_just_pressed) {
                Button& selectedBtn = activeWindow->buttons[activeWindow->selected_index];
                
                if (selectedBtn.text == "Exit") {break;}
                else if (selectedBtn.text == "Color") {
                    choosingColor = true;
                    std::cout << "Entering color selection mode - rotate knob to cycle colors, press to confirm\n";
                }else if (selectedBtn.text == "Font") {
                    choosingFont = true;
                    std::cout << "Entering font selection mode - rotate knob to cycle fonts, press to confirm\n";
                }
                else if (selectedBtn.text == "Start"){
                    simRunning = true;
                    std::cout << "Starting simulation\n";
                    Simulation simulation;
                    simulation.initialize(parlcd_mem_base, mem_base);
                    simulation.run(parlcd_mem_base, mem_base);
                    std::cout << "Simulation finished\n";
                }
                else selectedBtn.activate();
                usleep(200 * 1000);
            }
        }
        last_green_knob = green_knob;
        last_green_knob_pressed = green_knob_pressed;
        
        for (int i = 0; i < WIDTH * HEIGHT; ++i)
            fb[i] = activeWindow->background_color;
        
        if (activeWindow && parlcd_mem_base) 
            activeWindow->drawWindow(parlcd_mem_base, fb);

        flushFramebuffer(parlcd_mem_base, fb);

        struct timespec end_time;
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        
        unsigned int elapsed_us = (end_time.tv_sec - start_time.tv_sec) * 1000000 + 
                                 (end_time.tv_nsec - start_time.tv_nsec) / 1000;
        if (elapsed_us < FRAME_TIME_US) {
            usleep(FRAME_TIME_US - elapsed_us);
        }
    }
    flushFramebuffer(parlcd_mem_base, fb);
    free(fb);
    return 0;
}



