#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>
#include <string>
#include <cstdint>
#include "simulation/Boid.hpp"
#include "simulation/Prey.hpp"
#include "simulation/Predator.hpp"

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
    
    void toggleParamAdjustMode() { paramAdjustMode = !paramAdjustMode; }

    bool isInParamAdjustMode() const { return paramAdjustMode; }

private:
    void clearGrid();
    void setupGrid();
    sf::Vector2i getGridCell(const sf::Vector2f& position);
    
    std::vector<Prey> preys;
    std::vector<Predator> predators;
    std::vector<std::vector<std::vector<int>>> grid;

    bool paramAdjustMode = false;
    int currentParamIndex = 0;
    int8_t lastBlueKnob = 0;
    bool showingSelectedParam = false;
    uint64_t paramSelectionTime = 0;
    static const uint64_t PARAM_DISPLAY_TIME = 2000000;
    struct AdjustableParam {
        std::string name;
        float* value;
        float min;
        float max;
        float step;
    };

    std::vector<AdjustableParam> adjustableParams;
    void initAdjustableParams();
    void drawParamUI(unsigned char* mem_base);    
    void handleBlueKnob(uint32_t knob_data);

    static const int GRID_CELL_SIZE = 50;
    const int GRID_WIDTH;
    const int GRID_HEIGHT;
    unsigned short background_color;
};

#endif 