#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

#include <vector>
#include <random>
#include <cmath>
#include <iostream>
#include "Vector2Util.hpp"
#include "Boid.hpp"
#include "Prey.hpp"
#include "Predator.hpp"
#include "draw_utils.h"
#include "Window.h"
#include "Button.h"
#include "SceneManager.h"

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

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
extern const int PREYS_COUNT = 30;
extern const int PREDATORS_COUNT = 5;
extern const float KILL_DISTANCE = 5.0f;



unsigned short *fb = nullptr;
unsigned short background_color = 0xffff;

std::mt19937 gen;
std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
std::uniform_real_distribution<float> speedDis(1.0f, MAX_SPEED);

void draw_pixel(int x, int y, unsigned short int color) {
  if (color != 65535) {
    fb[(x % 480) + 480 * (y % 320)] = color;
  }
}

int main(int argc, char *argv[]) {
  std::random_device rd;
  gen = std::mt19937(rd());
  std::uniform_int_distribution<> posDis_x(0, WIDTH);
  std::uniform_int_distribution<> posDis_y(0, HEIGHT);

  std::vector<Prey> preys;
    preys.reserve(PREYS_COUNT);
    for (int i = 0; i < PREYS_COUNT; i++) {
        preys.emplace_back(posDis_x(gen), posDis_y(gen));
    }

  std::vector<Predator> predators;
    predators.reserve(PREDATORS_COUNT);
    for (int i = 0; i < PREDATORS_COUNT; i++) {
        predators.emplace_back(posDis_x(gen), posDis_y(gen));
    }

  unsigned char *parlcd_mem_base, *mem_base;
  int ptr;
  fb = (unsigned short *)malloc(320 * 480 * 2);

  printf("Hello world\n");

  // Explicitly cast the return value of map_phys_address
  parlcd_mem_base = (unsigned char *)map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  if (parlcd_mem_base == NULL)
    exit(1);

  mem_base = (unsigned char *)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  if (mem_base == NULL)
    exit(1);

  parlcd_hx8357_init(parlcd_mem_base);

  struct timespec loop_delay;
  loop_delay.tv_sec = 0;
  loop_delay.tv_nsec = 150 * 1000 * 1000;

  Window mainWindow;
  Window gameWindow;
  Window settingsWindow;
  
  UIManager ui;

  // Window mainMenu(0x001F);  // Blue background
  // ui.render(parlcd_mem_base);
 Window window;

    // Add three buttons
    window.buttons.push_back(Button(50, 50, 120, 40, 0xF800, "Start", []() {
        std::cout << "Start pressed\n";
    }));

    window.buttons.push_back(Button(50, 110, 120, 40, 0x07E0, "Settings", []() {
        std::cout << "Settings pressed\n";
    }));

    window.buttons.push_back(Button(50, 170, 120, 40, 0x001F, "Exit", []() {
        std::cout << "Exit pressed\n";
    }));

    // Set first button as selected
    window.buttons[0].selected = true;
    window.selected_index = 0;

    // Draw the window
    window.drawWindow(parlcd_mem_base);
   usleep(5000 * 1000);

  while (1) {
    int r = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    if ((r & 0x7000000) != 0) {
        break;
    }


    for (int i = 0; i < 320 * 480; i++) {
        fb[i] = background_color;
    }

    for (auto& prey : preys) {
        prey.update();
        prey.flock(preys);
        prey.draw();
    }

    for (auto& predator : predators) {
      predator.update();
      predator.hunt(preys);
      
      bool killed = predator.tryToKill(preys);
      
      if (killed) {
        if (preys.size() < PREYS_COUNT) {
          preys.emplace_back(posDis_x(gen), posDis_y(gen));
        }
      }
      predator.draw();
  }

    // Update the display
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (int i = 0; i < 480 * 320; i++) {
        parlcd_write_data(parlcd_mem_base, fb[i]);
    }

    clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
  }

  parlcd_write_cmd(parlcd_mem_base, 0x2c);
  for (ptr = 0; ptr < 480 * 320; ptr++) {
    parlcd_write_data(parlcd_mem_base, 0);
  }

  printf("Goodbye world\n");

  return 0;
}