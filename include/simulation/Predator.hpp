#pragma once
#include "Boid.hpp"
#include "Prey.hpp"
/**
 * @brief A predator boid that can eliminate prey
 */
class Predator : public Boid {
public:
    Predator(float x, float y);

    /**
     * @brief Brings the predator toward nearby prey
     * @param preys List of prey to hunt
     */
    void hunt(const std::vector<Prey>& preys);

    /**
     * @brief Tries to kill a prey if it's close enough
     */
    bool tryToKill(std::vector<Prey>& preys);

    void applyForce(const sf::Vector2f& force);

    /**
     * @brief Calculates a steering force to avoid screen edges
     */
    sf::Vector2f avoidBorders() const;
};