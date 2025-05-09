#pragma once
#include "Boid.hpp"
#include "Prey.hpp"

class Predator : public Boid {
public:
    Predator(float x, float y);
    
    void hunt(const std::vector<Prey>& preys);
    void applyForce(const sf::Vector2f& force);
    sf::Vector2f avoidBorders() const;
};