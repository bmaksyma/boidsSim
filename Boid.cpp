#include "Boid.hpp"
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Vector2Util.hpp"

extern const int WIDTH;
extern const int HEIGHT;
extern const float MAX_SPEED;
extern const float MAX_FORCE;
extern const float PERCEPTION_RADIUS;
extern const float SEPARATION_RADIUS;
extern const float FOOD_ATTRACTION;
extern const float ALIGNMENT_WEIGHT;
extern const float COHESION_WEIGHT;
extern const float SEPARATION_WEIGHT;
extern const float MARGIN_SIZE;
extern const float TURN_FORCE;
extern const float PREY_SIZE;
extern const int PREYS_COUNT;

extern unsigned short *fb;

extern std::mt19937 gen;
extern std::uniform_real_distribution<float> dis;
extern std::uniform_real_distribution<float> speedDis;

Boid::Boid(float x, float y, float size) : position(x, y), size(size), color(0x981e) {
    velocity = sf::Vector2f(dis(gen), dis(gen));
    velocity = Vector2Util::scaleTo(velocity, speedDis(gen));
    acceleration = sf::Vector2f(0, 0);
}

void Boid::update() {
    sf::Vector2f borderForce = avoidBorders();
    applyForce(borderForce);

    velocity += acceleration;

    if (Vector2Util::calc_length(velocity) > MAX_SPEED) {
        velocity = Vector2Util::scaleTo(velocity, MAX_SPEED);
    }

    position += velocity;
    
    acceleration = sf::Vector2f(0, 0);
}

sf::Vector2f Boid::avoidBorders() const {
    sf::Vector2f steering(0, 0);
    
    if (position.x < MARGIN_SIZE) {
        steering.x += TURN_FORCE * (1.0f - position.x / MARGIN_SIZE);
    } else if (position.x > WIDTH - MARGIN_SIZE) {
        steering.x -= TURN_FORCE * (1.0f - (WIDTH - position.x) / MARGIN_SIZE);
    }
    
    if (position.y < MARGIN_SIZE) {
        steering.y += TURN_FORCE * (1.0f - position.y / MARGIN_SIZE);
    } else if (position.y > HEIGHT - MARGIN_SIZE) {
        steering.y -= TURN_FORCE * (1.0f - (HEIGHT - position.y) / MARGIN_SIZE);
    }
    
    steering = Vector2Util::calc_length(steering) > MAX_FORCE ? Vector2Util::scaleTo(steering, MAX_FORCE) : steering;
    
    return steering;
}

void Boid::applyForce(const sf::Vector2f& force) {
    acceleration += force;
}

void Boid::draw(sf::RenderWindow& window) const {
    // Calculate the angle for rotation
    float angle = std::atan2(velocity.y, velocity.x) * 180 / M_PI;
    // Draw the fish body (triangle)
    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    float rad1 = angle * M_PI / 180;
    float rad2 = (angle + 140) * M_PI / 180;
    float rad3 = (angle - 140) * M_PI / 180;
    triangle.setPoint(0, sf::Vector2f(position.x + size * std::cos(rad1), 
                                     position.y + size * std::sin(rad1)));
    triangle.setPoint(1, sf::Vector2f(position.x + size/2 * std::cos(rad2), 
                                     position.y + size/2 * std::sin(rad2)));
    triangle.setPoint(2, sf::Vector2f(position.x + size/2 * std::cos(rad3), 
                                     position.y + size/2 * std::sin(rad3)));

    triangle.setFillColor(color);
    window.draw(triangle);
}