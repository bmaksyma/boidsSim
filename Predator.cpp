#include "Predator.hpp"
#include <limits>
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
extern const float PREDATOR_SIZE;
extern const int PREDATORS_COUNT;
extern const float KILL_DISTANCE;

Predator::Predator(float x, float y) : Boid(x, y, PREDATOR_SIZE) {
    color = 0xF800;
}

void Predator::hunt(const std::vector<Prey>& preys) {
    if (preys.empty()) {
        return;
    }

    sf::Vector2f closestPrey;
    float closestDistance = std::numeric_limits<float>::max();

    for (const auto& prey : preys) {
        float distance = Vector2Util::distance(position, prey.position);
        if (distance < closestDistance) {
            closestDistance = distance;
            closestPrey = prey.position;
        }
    }

    if (closestDistance < PERCEPTION_RADIUS) {
        sf::Vector2f desired = closestPrey - position;
        desired = Vector2Util::normalize(desired);
        desired *= MAX_SPEED;

        sf::Vector2f steer = desired - velocity;
        if (Vector2Util::calc_length(steer) > MAX_FORCE) {
            steer = Vector2Util::scaleTo(steer, MAX_FORCE);
        }
        applyForce(steer);
    }
}

bool Predator::tryToKill(std::vector<Prey>& preys) {
    for (auto it = preys.begin(); it != preys.end(); ++it) {
        float distance = Vector2Util::distance(position, it->position);
        if (distance < KILL_DISTANCE) {
            preys.erase(it);
            return true;
        }
    }
    return false;
}

void Predator::applyForce(const sf::Vector2f& force) {
    acceleration += force;
}

sf::Vector2f Predator::avoidBorders() const {
    return Boid::avoidBorders();
}