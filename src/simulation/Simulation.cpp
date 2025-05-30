#define _POSIX_C_SOURCE 200112L

#include "simulation/Simulation.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <random>
#include <cmath>
#include <iostream>
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#include "ui/theme_colors.h"
#include "fonts/font_types.h"
#include "fonts/font_manager.h"
#include "utils/text_draw.h"
#include "utils/draw_utils.h"
#include "globals.h"


std::mt19937 gen;
std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
std::uniform_real_distribution<float> speedDis(1.0f, MAX_SPEED);


extern void draw_pixel(int x, int y, unsigned short int color); 

Simulation::Simulation() 
    : GRID_WIDTH(WIDTH / GRID_CELL_SIZE + 1),
      GRID_HEIGHT(HEIGHT / GRID_CELL_SIZE + 1),
      background_color(getCurrentBackgroundColor()) {
    
    std::random_device rd;
    gen = std::mt19937(rd());
}

void Simulation::initialize(unsigned char* parlcd_mem_base, unsigned char* mem_base) {
    std::uniform_int_distribution<> posDis_x(0, WIDTH);
    std::uniform_int_distribution<> posDis_y(0, HEIGHT);
    
    preys.clear();
    preys.reserve(PREYS_COUNT);
    for (int i = 0; i < PREYS_COUNT; i++) {
        preys.emplace_back(posDis_x(gen), posDis_y(gen));
    }
    
    predators.clear();
    predators.reserve(PREDATORS_COUNT);
    for (int i = 0; i < PREDATORS_COUNT; i++) {
        predators.emplace_back(posDis_x(gen), posDis_y(gen));
    }
    setupGrid();

    paramManager.initAdjustableParams();
    paramManager.setLastBlueKnob(*(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o) & 0xFF);
}

void Simulation::run(unsigned char* parlcd_mem_base, unsigned char* mem_base) {
    int initial_knob = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    bool initial_green_pressed = (initial_knob & 0x02000000) != 0;
    bool initial_blue_pressed = (initial_knob & 0x01000000) != 0;
    
    if (initial_green_pressed) {
        while ((*(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o) & 0x02000000) != 0) {
            usleep(10000);
        }
    }
    
    if (initial_blue_pressed) {
        while ((*(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o) & 0x01000000) != 0) {
            usleep(10000);
        }
    }
    
    while (1) {
        uint32_t knob_data = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
        bool green_pressed = (knob_data & 0x02000000) != 0;
        bool blue_pressed = (knob_data & 0x01000000) != 0;
        
        static bool last_blue_pressed = false;
        static uint64_t last_blue_press_time = 0;
        static const uint64_t DOUBLE_PRESS_TIME = 500000;
        
        if (blue_pressed && !last_blue_pressed) {
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            uint64_t current_time = ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
            
            if (current_time - last_blue_press_time < DOUBLE_PRESS_TIME) {
                paramManager.toggleParamAdjustMode();
                std::cout << "Parameter adjustment mode: " << (paramManager.isParamAdjustMode() ? "ON" : "OFF") << std::endl;
            }
            
            last_blue_press_time = current_time;
        }
        last_blue_pressed = blue_pressed;
        
        paramManager.handleBlueKnob(knob_data);
        
        if (green_pressed) {
            break;
        }
        background_color = getCurrentBackgroundColor();
        for (int i = 0; i < HEIGHT * WIDTH; i++) {
            fb[i] = background_color;
        }
        
        update(mem_base);
        render(parlcd_mem_base, mem_base);
        paramManager.drawParamUI(mem_base);        
        parlcd_write_cmd(parlcd_mem_base, 0x2c);
        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            parlcd_write_data(parlcd_mem_base, fb[i]);
        }
    }
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x000000;
    cleanup(parlcd_mem_base);
}

void Simulation::update(unsigned char* mem_base) {
    clearGrid();    
    for (int i = 0; i < preys.size(); i++) {
        sf::Vector2i cell = getGridCell(preys[i].getPosition());
        grid[cell.x][cell.y].push_back(i);
    }
    
    for (int i = 0; i < preys.size(); i++) {
        sf::Vector2i cell = getGridCell(preys[i].getPosition());
        
        std::vector<Prey*> neighbors;
        
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = cell.x + dx;
                int ny = cell.y + dy;
                
                if (nx < 0 || nx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT)
                    continue;
                
                for (int neighborIdx : grid[nx][ny]) {
                    if (neighborIdx == i)
                        continue;                            
                    float distance = Vector2Util::distance(preys[i].getPosition(), preys[neighborIdx].getPosition());
                    if (distance < PERCEPTION_RADIUS) {
                        neighbors.push_back(&preys[neighborIdx]);
                    }
                }
            }
        }
        
        preys[i].update();
        preys[i].flock(neighbors);
    }
    
    std::uniform_int_distribution<> posDis_x(0, WIDTH);
    std::uniform_int_distribution<> posDis_y(0, HEIGHT);
    
    for (auto& predator : predators) {
        predator.update();
        predator.hunt(preys);
        
        bool killed = predator.tryToKill(preys);
        
        if (killed) {
            if (preys.size() < PREYS_COUNT) {
                preys.emplace_back(posDis_x(gen), posDis_y(gen));
                uint32_t blue_color = 0x0000FF;
                *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = blue_color;
            }
            uint32_t red_color = 0xFF0000;
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = red_color;
        }
    }
}

void Simulation::render(unsigned char* parlcd_mem_base, unsigned char* mem_base) {
    for (auto& prey : preys) {
        prey.draw();
    }
    
    for (auto& predator : predators) {
        predator.draw();
    }
    
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x000000;
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x000000;
}

void Simulation::cleanup(unsigned char* parlcd_mem_base) {
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        parlcd_write_data(parlcd_mem_base, 0);
    }  
}

void Simulation::clearGrid() {
    grid.clear();
    grid.resize(GRID_WIDTH);
    for (int i = 0; i < GRID_WIDTH; i++) {
        grid[i].resize(GRID_HEIGHT);
    }
}

void Simulation::setupGrid() {
    grid.resize(GRID_WIDTH);
    for (int i = 0; i < GRID_WIDTH; i++) {
        grid[i].resize(GRID_HEIGHT);
    }
}

sf::Vector2i Simulation::getGridCell(const sf::Vector2f& position) {
    int gridX = static_cast<int>(position.x) / GRID_CELL_SIZE;
    int gridY = static_cast<int>(position.y) / GRID_CELL_SIZE;
    gridX = std::max(0, std::min(gridX, GRID_WIDTH - 1));
    gridY = std::max(0, std::min(gridY, GRID_HEIGHT - 1));
    
    return sf::Vector2i(gridX, gridY);
}