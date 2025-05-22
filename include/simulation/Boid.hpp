#pragma once
#include <vector>
#include "Vector2Util.hpp"
// #include <SFML/Graphics.hpp>

class Boid {
public:
    Boid(float x, float y, float size);
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float size;
    int color;
    // sf::Color color;
 
    sf::Vector2f getPosition() const { return position; }

    sf::Vector2f avoidBorders() const;
    void applyForce(const sf::Vector2f& force);

    void update();
    void draw() const;
    // void draw(sf::RenderWindow& window) const;
};