#pragma once

#include "Boid.hpp"
#include "Prey.hpp"
#include <vector>

/**
 * @brief A Boid that behaves like a predator
 *
 * Predator objects chase and attempt to eliminate Prey in the simulation
 * They use steering behaviors to move toward targets, avoid borders,
 * and eliminate prey within a certain kill distance
 */
class Predator : public Boid {
public:
    /**
     * @brief Construct a new Predator at a given position
     * @param x Initial X coordinate
     * @param y Initial Y coordinate
     */
    Predator(float x, float y);

    /**
     * @brief Applies a steering force to pursue the closest visible prey
     * 
     * @param preys A vector of all Prey in the simulation
     */
    void hunt(const std::vector<Prey>& preys);

    /**
     * @brief Tries to eliminate prey within a kill distance
     * 
     * @param preys A reference to the prey list to modify
     * @return true if any prey was killed
     * @return false otherwise
     */
    bool tryToKill(std::vector<Prey>& preys);

    /**
     * @brief Adds an external force to the predator’s acceleration
     * 
     * @param force The force vector to apply
     */
    void applyForce(const sf::Vector2f& force);

    /**
     * @brief Calculates a steering force to avoid borders of the screen
     * 
     * @return The border-avoidance force vector
     */
    sf::Vector2f avoidBorders() const override;
};
