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
extern const int PREYS_COUNT = 100;
extern const int PREDATORS_COUNT = 5;
extern const float KILL_DISTANCE = 5.0f;

const int GRID_CELL_SIZE = 50;
const int GRID_WIDTH = WIDTH / GRID_CELL_SIZE + 1;
const int GRID_HEIGHT = HEIGHT / GRID_CELL_SIZE + 1;

std::vector<std::vector<std::vector<int>>> grid;

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

void clearGrid() {
  grid.clear();
  grid.resize(GRID_WIDTH);
  for (int i = 0; i < GRID_WIDTH; i++) {
      grid[i].resize(GRID_HEIGHT);
  }
}

sf::Vector2i getGridCell(const sf::Vector2f& position) {
  int gridX = static_cast<int>(position.x) / GRID_CELL_SIZE;
  int gridY = static_cast<int>(position.y) / GRID_CELL_SIZE;
  gridX = std::max(0, std::min(gridX, GRID_WIDTH - 1));
  gridY = std::max(0, std::min(gridY, GRID_HEIGHT - 1));
  
  return sf::Vector2i(gridX, gridY);
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


    //-------------------------------------------------------------------------------
    grid.resize(GRID_WIDTH);
    for (int i = 0; i < GRID_WIDTH; i++) {
        grid[i].resize(GRID_HEIGHT);
    }
    //-------------------------------------------------------------------------------

  while (1) {
    int r = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    if ((r & 0x7000000) != 0) {
        break;
    }

    for (int i = 0; i < 320 * 480; i++) {
        fb[i] = background_color;
    }

    clearGrid();
    for (int i = 0; i < preys.size(); i++) {
        sf::Vector2i cell = getGridCell(preys[i].getPosition());
        grid[cell.x][cell.y].push_back(i);
    }

    for (int i = 0; i < preys.size(); i++) {
      sf::Vector2i cell = getGridCell(preys[i].getPosition());
      
      std::vector<Prey*> neighbors;
      
      // Check current cell and adjacent cells (9 cells total)
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

  for (auto& prey : preys) {
      prey.draw();
  }

    // for (auto& prey : preys) {
    //     prey.update();
    //     prey.flock(preys);
    //     prey.draw();
    // }

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

    //clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
  }

  parlcd_write_cmd(parlcd_mem_base, 0x2c);
  for (ptr = 0; ptr < 480 * 320; ptr++) {
    parlcd_write_data(parlcd_mem_base, 0);
  }

  printf("Goodbye world\n");

  return 0;
}