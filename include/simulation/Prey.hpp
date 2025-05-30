#pragma once

#include "Boid.hpp"
#include <vector>

/**
 * @brief A prey boid that follows flocking behavior 
 * Modifying alignment, cohesion, separation
 */
class Prey : public Boid {
public:
    Prey(float x, float y);

    /**
     * @brief Applies flocking behavior using nearby preys
     */
    void flock(const std::vector<Prey*>& preys);

    /**
     * @brief Applies a force to the prey's acceleration
     */
    void applyForce(const sf::Vector2f& force);

    /**
     * @brief Calculates alignment force 
     */
    sf::Vector2f align(const std::vector<Prey*>& preys) const;

    /**
     * @brief Calculates cohesion force 
     */
    sf::Vector2f cohesion(const std::vector<Prey*>& preys) const;

    /**
     * @brief Calculates separation force 
     */
    sf::Vector2f separation(const std::vector<Prey*>& preys) const;

    /**
     * @brief Calculates force to avoid screen edges
     */
    sf::Vector2f avoidBorders() const;
};
