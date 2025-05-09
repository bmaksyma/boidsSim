#pragma once
// #include <SFML/Graphics.hpp>
#include <vector>
#include "Vector2Util.hpp"

class Boid {
public:
    Boid(float x, float y);
    // Boid(float x, float y, const sf::Color& customColor);
    
    void update();
    void flock(const std::vector<Boid>& boids);
    void draw() const;
    
private:
    void applyForce(const sf::Vector2f& force);
    sf::Vector2f align(const std::vector<Boid>& boids) const;
    sf::Vector2f cohesion(const std::vector<Boid>& boids) const;
    sf::Vector2f separation(const std::vector<Boid>& boids) const;
    sf::Vector2f seekFood(const sf::Vector2f* foodPos) const;

    sf::Vector2f avoidBorders() const;
    void turn(float angle);
    

    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float size;
    unsigned short int color;
};
