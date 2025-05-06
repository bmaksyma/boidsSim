#pragma once
// #include <SFML/Graphics.hpp>
#include <vector>
#include "Vector2Util.hpp"

class Fish {
public:
    Fish(float x, float y);
    // Fish(float x, float y, const sf::Color& customColor);
    
    void update();
    void flock(const std::vector<Fish>& fishes);
    void draw() const;
    
private:
    void applyForce(const sf::Vector2f& force);
    sf::Vector2f align(const std::vector<Fish>& fishes) const;
    sf::Vector2f cohesion(const std::vector<Fish>& fishes) const;
    sf::Vector2f separation(const std::vector<Fish>& fishes) const;
    sf::Vector2f seekFood(const sf::Vector2f* foodPos) const;

    sf::Vector2f avoidBorders() const;
    void turn(float angle);
    

    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float size;
    unsigned short int color;
};