#pragma once
#include <vector>
#include "Vector2Util.hpp"

class Boid {
public:
    Boid(float x, float y, float size);
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float size;
    unsigned short int color;

    sf::Vector2f avoidBorders() const;
    void applyForce(const sf::Vector2f& force);

    void update();
    void draw() const;
};
