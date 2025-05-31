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
 * @brief Runs and manages the main boids simulation including updating, drawing
 *             and handling interactions between preys and predators
 */
class Simulation {
public:
    /**
     * @brief Construct a new Simulation object
     */
    Simulation();

    /**
     * @brief Initializes preys, predators, grid, and parameter manager
     * 
     * @param parlcd_mem_base Pointer to parallel LCD memory
     * @param mem_base Pointer to base memory
     */
    void initialize(unsigned char* parlcd_mem_base, unsigned char* mem_base);

    /**
     * @brief Runs the main simulation loop, handling rendering and updates
     * 
     * @param parlcd_mem_base Pointer to parallel LCD memory
     * @param mem_base Pointer to base memory
     */
    void run(unsigned char* parlcd_mem_base, unsigned char* mem_base);

    /**
     * @brief Updates positions, forces, and interactions between boids
     * 
     * @param mem_base Pointer to base memory
     */
    void update(unsigned char* mem_base);

    /**
     * @brief Renders the simulation to the framebuffer and LCD.
     * 
     * @param parlcd_mem_base Pointer to memory-mapped parlcd registers
     * @param mem_base Pointer to base memory
     */
    void render(unsigned char* parlcd_mem_base, unsigned char* mem_base);

    /**
     * @brief Clears the screen and framebuffer after simulation ends.
     * 
     * @param parlcd_mem_base Pointer to memory-mapped parlcd registers
     */
    void cleanup(unsigned char* parlcd_mem_base);

private:
    /**
     * @brief Clears the spatial grid used for spatial partitioning
     */
    void clearGrid();

    /**
     * @brief Initializes the spatial grid based on simulation resolution
     */
    void setupGrid();

    /**
     * @brief Returns the grid cell index for a given position
     * 
     * @param position 2D position in space
     * @return sf::Vector2i Grid cell index
     */
    sf::Vector2i getGridCell(const sf::Vector2f& position);
    
    /**
     * @brief Boid population and spatial management
     * 
     * - preys: List of all prey boids
     * - predators: List of predator boids 
     * - grid: 3D spatial grid for partitioning the simulation space 
     *                         to improve neighbor search efficiency
     */
    std::vector<Prey> preys;                         
    std::vector<Predator> predators;                 
    std::vector<std::vector<std::vector<int>>> grid; 

    /**
     * @brief Handles user-adjustable parameters like weights and distances
     */
    ParameterManager paramManager;

    /**
     * @brief Spatial resolution constants
     * 
     * GRID_CELL_SIZE defines how large each grid cell is in pixels
     * GRID_WIDTH and GRID_HEIGHT are derived from the screen dimensions
     */
    static const int GRID_CELL_SIZE = 50;            
    const int GRID_WIDTH;                            
    const int GRID_HEIGHT;                           

    /**
     * @brief Color used to clear the screen before rendering each frame
     */
    unsigned short background_color;

};

#endif 
