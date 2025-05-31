#pragma once

#include <vector>
#include <cstdint>
#include <SFML/System/Vector2.hpp> 
#include "Vector2Util.hpp"

/**
 * @brief Represents a single Boid in a flocking simulation
 * 
 * Boids simulate natural flocking behavior using basic steering forces
 */
class Boid {
public:
    /**
     * @brief Construct a new Boid object
     * 
     * @param x Initial x-position
     * @param y Initial y-position
     * @param size Size of the boid
     */
    Boid(float x, float y, float size);

    /**
    * @brief Position and velocity of the boid in 2D space
    * 
    * The current position is represented as a vector with x and y coordinates
    * The current velocity is a vector indicating the speed and direction
    * The current acceleration is a vector indicating the change in velocity
    * Size represents the visual size of the boid, affecting its appearance
    * Color is used for drawing the boid 
    */
    sf::Vector2f position;      
    sf::Vector2f velocity;      
    sf::Vector2f acceleration;  
    float size;                 
    int color;                  

    /**
     * @brief Get the current position
     * 
     * @return sf::Vector2f Current position
     */
    sf::Vector2f getPosition() const {return position;};

    /**
     * @brief Calculate steering force to avoid screen borders
     * 
     * @return sf::Vector2f Steering force
     */
    sf::Vector2f avoidBorders() const;

    /**
     * @brief Apply a force to the boid's acceleration
     * 
     * @param force Force vector to apply
     */
    void applyForce(const sf::Vector2f& force);

    /**
     * @brief Update the boid's position, velocity and resets acceleration
     */
    void update();

    /**
     * @brief Draw the boid as a triangle on screen using global framebuffer
     */
    void draw() const;
};
