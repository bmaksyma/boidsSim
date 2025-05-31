#pragma once

#include "Boid.hpp"
#include <vector>

/**
 * @brief Represents a prey boid that follows flocking behavior
 * 
 * The Prey class inherits from Boid and adds flocking logic such as
 * alignment, cohesion, and separation based on nearby boids
 */
class Prey : public Boid {
public:
    /**
     * @brief Construct a new Prey object at the given coordinates
     * 
     * @param x Initial X position
     * @param y Initial Y position
     */
    Prey(float x, float y);

    /**
     * @brief Applies flocking behavior based on nearby prey
     * 
     * @param preys Vector of pointers to nearby prey boids
     */
    void flock(const std::vector<Prey*>& preys);

    /**
     * @brief Applies a force to the preys acceleration
     * 
     * @param force The vector of force to apply
     */
    void applyForce(const sf::Vector2f& force);

    /**
     * @brief Calculates the alignment force based on nearby prey
     * 
     * Tries to match velocity with nearby boids
     * @param preys Vector of pointers to nearby prey boids
     * @return Alignment steering force
     */
    sf::Vector2f align(const std::vector<Prey*>& preys) const;

    /**
     * @brief Calculates the cohesion force based on nearby prey
     * 
     * Steers towards the average position of nearby boids
     * @param preys Vector of pointers to nearby prey boids
     * @return Cohesion steering force
     */
    sf::Vector2f cohesion(const std::vector<Prey*>& preys) const;

    /**
     * @brief Calculates the separation force based on nearby prey
     * 
     * Steers away from nearby boids
     * @param preys Vector of pointers to nearby prey boids
     * @return Separation steering force
     */
    sf::Vector2f separation(const std::vector<Prey*>& preys) const;

    /**
     * @brief Calculates the force needed to avoid screen edges
     * 
     * @return Border-avoiding force vector
     */
    sf::Vector2f avoidBorders() const override;
};
