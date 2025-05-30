#pragma once
#include <vector>
#include "Vector2Util.hpp"
// #include <SFML/Graphics.hpp>
/**
 * @brief Represents a boid in the flocking simulation
 */
class Boid {
public:
    
    Boid(float x, float y, float size);
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float size;
    int color;
    // sf::Color color;
    // Returns current position
    sf::Vector2f getPosition() const { return position; }

    // Calculates a steering force to avoid edges
    sf::Vector2f avoidBorders() const;

    // Applies a force to the boid's acceleration
    void applyForce(const sf::Vector2f& force);

    // Updates the boid's parameters
    void update();

    // Draws the boid as a triangle on the screen
    void draw() const;
};