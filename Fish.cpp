#include "Fish.hpp"
#include <random>
#include <cmath>

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
extern const float FISH_SIZE;

extern std::mt19937 gen;
extern std::uniform_real_distribution<float> dis;
extern std::uniform_real_distribution<float> speedDis;

Fish::Fish(float x, float y) : position(x, y), size(FISH_SIZE), color(sf::Color(0, 128, 255)) {
    velocity = sf::Vector2f(dis(gen), dis(gen));
    velocity = Vector2Util::scaleTo(velocity, speedDis(gen));
    acceleration = sf::Vector2f(0, 0);
}

Fish::Fish(float x, float y, const sf::Color& customColor) : position(x, y), size(FISH_SIZE), color(customColor) {
    velocity = sf::Vector2f(dis(gen), dis(gen));
    velocity = Vector2Util::scaleTo(velocity, speedDis(gen));
    acceleration = sf::Vector2f(0, 0);
}

void Fish::update() {
    sf::Vector2f borderForce = avoidBorders();
    applyForce(borderForce);

    velocity += acceleration;

    if (Vector2Util::calc_length(velocity) > MAX_SPEED) {
        velocity = Vector2Util::scaleTo(velocity, MAX_SPEED);
    }
    // velocity = Vector2Util::calc_length(velocity) > MAX_SPEED ? Vector2Util::scaleTo(velocity, MAX_SPEED) : velocity;

    position += velocity;
    
    acceleration = sf::Vector2f(0, 0);
}

sf::Vector2f Fish::avoidBorders() const {
    sf::Vector2f steering(0, 0);
    
    // Left edge
    if (position.x < MARGIN_SIZE) {
        steering.x += TURN_FORCE * (1.0f - position.x/MARGIN_SIZE);
    }
    // Right edge
    else if (position.x > WIDTH - MARGIN_SIZE) {
        steering.x -= TURN_FORCE * (1.0f - (WIDTH - position.x)/MARGIN_SIZE);
    }
    
    // Top edge
    if (position.y < MARGIN_SIZE) {
        steering.y += TURN_FORCE * (1.0f - position.y/MARGIN_SIZE);
    }
    // Bottom edge
    else if (position.y > HEIGHT - MARGIN_SIZE) {
        steering.y -= TURN_FORCE * (1.0f - (HEIGHT - position.y)/MARGIN_SIZE);
    }
    
    // Limit the steering force
    // if (Vector2Util::calc_length(steering) > MAX_FORCE) {
    //     steering = Vector2Util::scaleTo(steering, MAX_FORCE);
    // }
    steering = Vector2Util::calc_length(steering) > MAX_FORCE ? Vector2Util::scaleTo(steering, MAX_FORCE) : steering;
    
    return steering;
}

void Fish::applyForce(const sf::Vector2f& force) {
    acceleration += force;
}

sf::Vector2f Fish::align(const std::vector<Fish>& fishes) const {
    sf::Vector2f steering(0, 0);
    int total = 0;
    
    for (const auto& other : fishes) {
        float distance = Vector2Util::distance(position, other.position);
        if (&other != this && distance < PERCEPTION_RADIUS) {
            steering += other.velocity;
            total++;
        }
    }
    
    if (total > 0) {
        steering /= static_cast<float>(total);
        steering = Vector2Util::normalize(steering);
        steering *= MAX_SPEED;
        
        sf::Vector2f steer = steering - velocity;
        if (Vector2Util::calc_length(steer) > MAX_FORCE) {
            steer = Vector2Util::scaleTo(steer, MAX_FORCE);
        }
        
        return steer;
    }
    
    return sf::Vector2f(0, 0);
}

sf::Vector2f Fish::cohesion(const std::vector<Fish>& fishes) const {
    sf::Vector2f steering(0, 0);
    int total = 0;
    
    for (const auto& other : fishes) {
        float distance = Vector2Util::distance(position, other.position);
        if (&other != this && distance < PERCEPTION_RADIUS) {
            steering += other.position;
            total++;
        }
    }
    
    if (total > 0) {
        steering /= static_cast<float>(total);
        steering -= position;
        steering = Vector2Util::normalize(steering);
        steering *= MAX_SPEED;
        
        sf::Vector2f steer = steering - velocity;
        if (Vector2Util::calc_length(steer) > MAX_FORCE) {
            steer = Vector2Util::scaleTo(steer, MAX_FORCE);
        }
        
        return steer;
    }
    
    return sf::Vector2f(0, 0);
}

sf::Vector2f Fish::separation(const std::vector<Fish>& fishes) const {
    sf::Vector2f steering(0, 0);
    int total = 0;
    
    for (const auto& other : fishes) {
        float distance = Vector2Util::distance(position, other.position);
        if (&other != this && distance < SEPARATION_RADIUS) {
            sf::Vector2f diff = position - other.position;
            diff /= distance;
            steering += diff;
            total++;
        }
    }
    
    if (total > 0) {
        steering /= static_cast<float>(total);
        steering = Vector2Util::normalize(steering);
        steering *= MAX_SPEED;
        
        sf::Vector2f steer = steering - velocity;
        if (Vector2Util::calc_length(steer) > MAX_FORCE) {
            steer = Vector2Util::scaleTo(steer, MAX_FORCE);
        }
        
        return steer;
    }
    
    return sf::Vector2f(0, 0);
}

void Fish::flock(const std::vector<Fish>& fishes) {
    sf::Vector2f alignment = this->align(fishes) * ALIGNMENT_WEIGHT;
    sf::Vector2f cohesion = this->cohesion(fishes) * COHESION_WEIGHT;
    sf::Vector2f separation = this->separation(fishes) * SEPARATION_WEIGHT;
    
    applyForce(alignment);
    applyForce(cohesion);
    applyForce(separation);
}

void Fish::draw(sf::RenderWindow& window) const {
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