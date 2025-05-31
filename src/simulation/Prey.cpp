#include "simulation/Prey.hpp"
#include <cstdint>

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
extern uint16_t PREY_COLOR;

Prey::Prey(float x, float y) : Boid(x, y, PREY_SIZE) {
    color = PREY_COLOR;
}

void Prey::applyForce(const sf::Vector2f& force) {
    acceleration += force;
}

void Prey::flock(const std::vector<Prey*>& preys) {
    sf::Vector2f alignment = align(preys) * ALIGNMENT_WEIGHT;
    sf::Vector2f cohesionForce = cohesion(preys) * COHESION_WEIGHT;
    sf::Vector2f separationForce = separation(preys) * SEPARATION_WEIGHT;
    
    applyForce(alignment);
    applyForce(cohesionForce);
    applyForce(separationForce);
}


sf::Vector2f Prey::align(const std::vector<Prey*>& preys) const {
    sf::Vector2f steering(0, 0);
    int total = 0;
    
    for (Prey* other : preys) {
        float distance = Vector2Util::distance(position, other->position);
        if (other != this && distance < PERCEPTION_RADIUS) {
            steering += other->velocity;
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

sf::Vector2f Prey::cohesion(const std::vector<Prey*>& preys) const {
    sf::Vector2f steering(0, 0);
    int total = 0;
    
    for (const auto& other : preys) {
        float distance = Vector2Util::distance(position, other->position);
        if (other != this && distance < PERCEPTION_RADIUS) {
            steering += other->position;
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

sf::Vector2f Prey::separation(const std::vector<Prey*>& preys) const {
    sf::Vector2f steering(0, 0);
    int total = 0;
    
    for (const auto& other : preys) {
        float distance = Vector2Util::distance(position, other->position);
        if (other != this && distance < SEPARATION_RADIUS) {
            sf::Vector2f diff = position - other->position;
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


sf::Vector2f Prey::avoidBorders() const {
    return Boid::avoidBorders();
}