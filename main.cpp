#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <cmath>
#include <iostream>
#include "Vector2Util.hpp"
#include "Boid.hpp"
#include "Predator.hpp"
#include "Prey.hpp"

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

std::mt19937 gen;
std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
std::uniform_real_distribution<float> speedDis(1.0f, MAX_SPEED);

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

int main() {
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

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fish Swarm Simulation");
    window.setFramerateLimit(60);
    
    
    // sf::Vector2f* foodPosition = nullptr;

    grid.resize(GRID_WIDTH);
    for (int i = 0; i < GRID_WIDTH; i++) {
        grid[i].resize(GRID_HEIGHT);
    }

    while (window.isOpen()) {        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color(255,255,255)); // White background
        
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
                        // Skip self
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
            prey.draw(window);
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
      predator.draw(window);
    }



        window.display();
    }
    return 0;
}