#pragma once
#include "Boid.hpp"

class Prey : public Boid {
public:
    Prey(float x, float y);
    void flock(const std::vector<Prey*>& preys);    
    void applyForce(const sf::Vector2f& force);
    sf::Vector2f align(const std::vector<Prey*>& preys) const;
    sf::Vector2f cohesion(const std::vector<Prey*>& preys) const;
    sf::Vector2f separation(const std::vector<Prey*>& preys) const;
    sf::Vector2f avoidBorders() const;
};