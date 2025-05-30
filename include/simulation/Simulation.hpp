#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>
#include <string>
#include <cstdint>
#include "simulation/Boid.hpp"
#include "simulation/Prey.hpp"
#include "simulation/Predator.hpp"
#include "simulation/ParameterManager.hpp"


/**
 * @brief Runs the main boid simulation, including rendering 
 */
class Simulation {
public:
    Simulation();

    void initialize(unsigned char* parlcd_mem_base, unsigned char* mem_base);
    
    void run(unsigned char* parlcd_mem_base, unsigned char* mem_base);

    void update(unsigned char* mem_base);
    
    void render(unsigned char* parlcd_mem_base, unsigned char* mem_base);
    
    void cleanup(unsigned char* parlcd_mem_base);
private:
    void clearGrid();
    void setupGrid();
    sf::Vector2i getGridCell(const sf::Vector2f& position);
    
    std::vector<Prey> preys;
    std::vector<Predator> predators;
    std::vector<std::vector<std::vector<int>>> grid;

    ParameterManager paramManager;

    static const int GRID_CELL_SIZE = 50;
    const int GRID_WIDTH;
    const int GRID_HEIGHT;
    unsigned short background_color;
};

#endif 