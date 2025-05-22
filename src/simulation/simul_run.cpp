// #include "simul_run.h"
// #include "Boid.hpp"
// #include "Prey.hpp"
// #include "Predator.hpp"
// #include "draw_utils.h"
// #include "Vector2Util.hpp"
// #include "mzapo_phys.h"
// #include "mzapo_parlcd.h"
// #include "mzapo_regs.h"

// #include <vector>
// #include <random>
// #include <unistd.h>
// #include <time.h>
// #include <iostream>

// const int WIDTH = 480;
// const int HEIGHT = 320;
// const int PREYS_COUNT = 100;
// const int PREDATORS_COUNT = 5;
// const float PERCEPTION_RADIUS = 25.0f;
// const int GRID_CELL_SIZE = 50;
// const int GRID_WIDTH = WIDTH / GRID_CELL_SIZE + 1;
// const int GRID_HEIGHT = HEIGHT / GRID_CELL_SIZE + 1;
// const uint16_t background_color = 0xffff;

// static std::vector<std::vector<std::vector<int>>> grid;
// static std::vector<Prey> preys;
// static std::vector<Predator> predators;
// static std::mt19937 gen;
// static std::uniform_int_distribution<> posDis_x(0, WIDTH);
// static std::uniform_int_distribution<> posDis_y(0, HEIGHT);

// unsigned short *fb = nullptr;

// sf::Vector2i getGridCell(const sf::Vector2f& position) {
//     int gridX = static_cast<int>(position.x) / GRID_CELL_SIZE;
//     int gridY = static_cast<int>(position.y) / GRID_CELL_SIZE;
//     gridX = std::max(0, std::min(gridX, GRID_WIDTH - 1));
//     gridY = std::max(0, std::min(gridY, GRID_HEIGHT - 1));
//     return sf::Vector2i(gridX, gridY);
// }

// void clearGrid() {
//     grid.clear();
//     grid.resize(GRID_WIDTH);
//     for (int i = 0; i < GRID_WIDTH; i++) {
//         grid[i].resize(GRID_HEIGHT);
//     }
// }

// void runSimulation() {
//     std::random_device rd;
//     gen.seed(rd());

//     preys.clear();
//     preys.reserve(PREYS_COUNT);
//     for (int i = 0; i < PREYS_COUNT; i++) {
//         preys.emplace_back(posDis_x(gen), posDis_y(gen));
//     }

//     predators.clear();
//     predators.reserve(PREDATORS_COUNT);
//     for (int i = 0; i < PREDATORS_COUNT; i++) {
//         predators.emplace_back(posDis_x(gen), posDis_y(gen));
//     }

//     fb = (unsigned short *)malloc(WIDTH * HEIGHT * sizeof(unsigned short));
//     if (!fb) return;

//     unsigned char *parlcd_mem_base = (unsigned char *)map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
//     if (!parlcd_mem_base) return;

//     unsigned char *mem_base = (unsigned char *)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
//     if (!mem_base) return;

//     parlcd_hx8357_init(parlcd_mem_base);

//     struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 150 * 1000 * 1000};

//     while (true) {
//         int r = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
//         if ((r & 0x7000000) != 0) {
//             break;
//         }

//         for (int i = 0; i < WIDTH * HEIGHT; ++i) fb[i] = background_color;

//         clearGrid();
//         for (int i = 0; i < preys.size(); i++) {
//             sf::Vector2i cell = getGridCell(preys[i].getPosition());
//             grid[cell.x][cell.y].push_back(i);
//         }

//         for (int i = 0; i < preys.size(); i++) {
//             sf::Vector2i cell = getGridCell(preys[i].getPosition());
//             std::vector<Prey*> neighbors;

//             for (int dx = -1; dx <= 1; dx++) {
//                 for (int dy = -1; dy <= 1; dy++) {
//                     int nx = cell.x + dx;
//                     int ny = cell.y + dy;
//                     if (nx < 0 || nx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT)
//                         continue;

//                     for (int neighborIdx : grid[nx][ny]) {
//                         if (neighborIdx == i) continue;
//                         float distance = Vector2Util::distance(preys[i].getPosition(), preys[neighborIdx].getPosition());
//                         if (distance < PERCEPTION_RADIUS) {
//                             neighbors.push_back(&preys[neighborIdx]);
//                         }
//                     }
//                 }
//             }

//             preys[i].update();
//             preys[i].flock(neighbors);
//         }

//         for (auto& prey : preys) prey.draw();

//         for (auto& predator : predators) {
//             predator.update();
//             predator.hunt(preys);
//             if (predator.tryToKill(preys)) {
//                 if (preys.size() < PREYS_COUNT) {
//                     preys.emplace_back(posDis_x(gen), posDis_y(gen));
//                 }
//             }
//             predator.draw();
//         }

//         parlcd_write_cmd(parlcd_mem_base, 0x2c);
//         for (int i = 0; i < WIDTH * HEIGHT; i++) {
//             parlcd_write_data(parlcd_mem_base, fb[i]);
//         }

//         clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
//     }

//     free(fb);
// }
