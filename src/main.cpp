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
#include "ui/theme_colors.h"
#include "simulation/Simulation.hpp"
#include "globals.h"

void switchToTheme(int themeIndex) {
    currentThemeIndex = themeIndex % themeNames.size();
    if (currentThemeIndex == 0) {
        currentBackgroundColor = 0xFFFF;
        PREY_COLOR = 0x001F;
        PREDATOR_COLOR = 0xF800;
    } else {
        currentBackgroundColor = 0x0000;
        PREY_COLOR = 0x07E0;
        PREDATOR_COLOR = 0xF81F;
    }
}

std::vector<uint16_t>& getCurrentThemeColors() {
    return (currentThemeIndex == 0) ? whiteThemeColors : darkThemeColors;
}

uint16_t getCurrentBackgroundColor() {
    return currentBackgroundColor;
}

void draw_pixel(int x, int y, unsigned short color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        fb[(x % WIDTH) + WIDTH * (y % HEIGHT)] = color;
    }
}
/**
 * @brief Main function to run the Boids simulation UI
 * 
 * - Initializes framebuffer and platform-specific memory
 * - Draws UI windows and handles user input via knobs
 * - Allows color, font, and theme selection
 * - Starts the simulation loop on command
 * 
 * @return 0 on success, 1 on framebuffer allocation failure
 */

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

    Window mainWindow;
    Window settingsWindow;
    Window exitWindow;
    Window* activeWindow = &mainWindow;

    uint16_t activeButtonColor = getCurrentThemeColors()[0];

    bool showingExit = false;
    bool choosingFont = false;
    bool choosingTheme = false;
    bool simRunning = false;
    int fontID = 0;
    int themeID = 0;

    switchToTheme(0);
    buttonColors = getCurrentThemeColors();
    
    mainWindow.background_color = getCurrentBackgroundColor();
    settingsWindow.background_color = getCurrentBackgroundColor();

    drawMainWindow(mainWindow, settingsWindow, activeWindow, activeButtonColor, choosingFont, choosingTheme);
    drawSettingsWindow(mainWindow, settingsWindow, activeWindow, activeButtonColor, choosingFont, choosingTheme);
    drawExitWindow(exitWindow);

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
        
        if (showingExit) {
            drawExitWindow(exitWindow);
            
            parlcd_write_cmd(parlcd_mem_base, 0x2c);
            for (int i = 0; i < WIDTH * HEIGHT; i++) {
                parlcd_write_data(parlcd_mem_base, fb[i]);
            }
            
            usleep(2000000);
            for (int i = 0; i < WIDTH * HEIGHT; ++i) {
                fb[i] = 0x0000;
            }
            break;
        }

        if (red_knob_pressed) {
            showingExit = true;
            std::cout << "Exiting: red knob pressed\n";
            // break;
        }
        int8_t delta = green_knob - last_green_knob;
        
        if (choosingFont) {
            handleFontChoice(delta, fontID, availableFonts, activeWindow, &mainWindow, &settingsWindow);
            CUR_FONT = availableFonts[fontID];
            if (green_knob_just_pressed) {
                choosingFont = false;
                std::cout << "Font confirmed and applied!\n";
                usleep(200 * 1000);
            }
        }
        else if (choosingTheme) {
            handleThemeChoice(delta, themeID, buttonColors, activeWindow, &mainWindow, &settingsWindow);
            if (green_knob_just_pressed) {
                choosingTheme = false;
                std::cout << "Theme confirmed and applied!\n";
                usleep(200 * 1000);
            }
        } else {
            if (delta != 0) {
                if (delta > 2) {
                    activeWindow->nextBtn();
                }
                else if (delta < (-2)) {
                    activeWindow->previousBtn();
                }
            }
            if (green_knob_just_pressed) {
                Button& selectedBtn = activeWindow->buttons[activeWindow->selected_index];
                
                if (selectedBtn.text == "Exit") {
                    showingExit = true;
                    // break;
                }
                else if (selectedBtn.text == "Font") {
                    choosingFont = true;
                    std::cout << "Entering font selection mode - rotate knob to cycle fonts, press to confirm\n";
                }
                else if (selectedBtn.text == "Theme") {
                    choosingTheme = true;
                    std::cout << "Entering theme selection mode - rotate knob to cycle themes, press to confirm\n";
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
        
        unsigned int elapsed_us = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_nsec - start_time.tv_nsec) / 1000;
        if (elapsed_us < FRAME_TIME_US) {
            usleep(FRAME_TIME_US - elapsed_us);
        }
    }
    flushFramebuffer(parlcd_mem_base, fb);
    free(fb);
    return 0;
}