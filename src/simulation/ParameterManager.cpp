#include "simulation/ParameterManager.hpp"
#include "ui/theme_colors.h"
#include "fonts/font_types.h"
#include "utils/text_draw.h"
#include "utils/draw_utils.h"
#include "mzapo_regs.h"
#include <time.h>
#include <iostream>

extern const int WIDTH;
extern const int HEIGHT;
extern float MARGIN_SIZE;
extern float COHESION_WEIGHT;
extern float ALIGNMENT_WEIGHT;
extern float SEPARATION_WEIGHT;
extern font_descriptor_t* CUR_FONT;
extern unsigned short *fb;

ParameterManager::ParameterManager() {
}

void ParameterManager::initAdjustableParams() {
    adjustableParams.clear();
    adjustableParams.push_back({"MARGIN_SIZE", &MARGIN_SIZE, 10.0f, 100.0f, 5.0f});
    adjustableParams.push_back({"COHESION_WEIGHT", &COHESION_WEIGHT, 0.0f, 2.0f, 0.1f});
    adjustableParams.push_back({"ALIGNMENT_WEIGHT", &ALIGNMENT_WEIGHT, 0.0f, 2.0f, 0.1f});
    adjustableParams.push_back({"SEPARATION_WEIGHT", &SEPARATION_WEIGHT, 0.0f, 2.0f, 0.1f});
}

void ParameterManager::toggleParamAdjustMode() {
    paramAdjustMode = !paramAdjustMode;
    if (!paramAdjustMode) {
        showingSelectedParam = false;
    }
}

void ParameterManager::handleBlueKnob(uint32_t knob_data) {
    if (!paramAdjustMode) return;
    
    int8_t blue_knob = knob_data & 0xFF;
    int8_t delta = blue_knob - lastBlueKnob;
    lastBlueKnob = blue_knob;
    
    bool blue_button_pressed = (knob_data & 0x01000000) != 0;
    static bool last_blue_button_pressed = false;
    bool blue_button_just_pressed = blue_button_pressed && !last_blue_button_pressed;

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint64_t current_time = ts.tv_sec * 1000000 + ts.tv_nsec / 1000;

    if (blue_button_just_pressed) {
        showingSelectedParam = !showingSelectedParam;
    }
    
    if (delta != 0) {
        if (showingSelectedParam) {
            if (delta > 2) {
                currentParamIndex = (currentParamIndex + 1) % adjustableParams.size();
                paramSelectionTime = current_time;
                std::cout << "Selected parameter: " << adjustableParams[currentParamIndex].name 
                          << " = " << *adjustableParams[currentParamIndex].value << std::endl;
            } else if (delta < -2) {
                currentParamIndex = (currentParamIndex - 1 + adjustableParams.size()) % adjustableParams.size();
                paramSelectionTime = current_time;
                std::cout << "Selected parameter: " << adjustableParams[currentParamIndex].name 
                          << " = " << *adjustableParams[currentParamIndex].value << std::endl;
            }
        } else {
            AdjustableParam& param = adjustableParams[currentParamIndex];
            
            if (delta > 2) {
                *param.value += param.step;
                if (*param.value > param.max) *param.value = param.max;
            } else if (delta < -2) {
                *param.value -= param.step;
                if (*param.value < param.min) *param.value = param.min;
            }
        }
    }
    last_blue_button_pressed = blue_button_pressed;
}

void ParameterManager::drawParamUI(unsigned char* mem_base) {
    if (!paramAdjustMode) {
        *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x000000;
        return;
    }
    int width = 180;
    int height = 50;
    int x = WIDTH - width - 10;
    int y = 10;
    int strS = 2;
    int corner_radius = 7;
    std::vector<uint16_t> themeColors = getCurrentThemeColors();
    uint16_t bgColor = getCurrentBackgroundColor();
    uint16_t textColor = (bgColor == 0xFFFF) ? 0x0000 : 0xFFFF;
    uint16_t accentColor = themeColors[2];

    draw_rounded_rect(fb, x - strS, y - strS, width + 2*strS, height + 2*strS, corner_radius + strS, textColor);
    draw_rounded_rect(fb, x, y, width, height, corner_radius, bgColor);

    if (!adjustableParams.empty()) {
        AdjustableParam& param = adjustableParams[currentParamIndex];

        float normalized = (*param.value - param.min) / (param.max - param.min);
        
        int num_leds = static_cast<int>(normalized * 32);
        if (num_leds > 32) num_leds = 32;
        if (num_leds < 0) num_leds = 0;
        
        uint32_t led_pattern = 0;
        for (int i = 0; i < num_leds; i++) {
            led_pattern |= (1 << i);
        }
        *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = led_pattern;
        // name
        draw_text(fb, x + 10, y + 10, CUR_FONT, param.name.c_str(), textColor, 1);
        std::string value_str = std::to_string(*param.value);
        size_t decimal_pos = value_str.find('.');
        if (decimal_pos != std::string::npos && decimal_pos + 3 < value_str.length()) {
            value_str = value_str.substr(0, decimal_pos + 3);
        }
        // value
        draw_text(fb, x + 10, y + 25, CUR_FONT, value_str.c_str(), accentColor, 1);
    }
}