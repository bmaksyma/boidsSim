// #include "Prey.hpp"

// extern const int WIDTH;
// extern const int HEIGHT;
// extern const float MAX_SPEED;
// extern const float MAX_FORCE;
// extern const float PERCEPTION_RADIUS;
// extern const float SEPARATION_RADIUS;
// extern const float FOOD_ATTRACTION;
// extern const float ALIGNMENT_WEIGHT;
// extern const float COHESION_WEIGHT;
// extern const float SEPARATION_WEIGHT;
// extern const float MARGIN_SIZE;
// extern const float TURN_FORCE;
// extern const float PREY_SIZE;
// extern const int PREYS_COUNT;

// Prey::Prey(float x, float y) : Boid(x, y, PREY_SIZE) {
//     color = 0x001F;
// }

// void Prey::applyForce(const sf::Vector2f& force) {
//     acceleration += force;
// }

// void Prey::flock(const std::vector<Prey*>& preys) {
//     sf::Vector2f alignment = align(preys) * ALIGNMENT_WEIGHT;
//     sf::Vector2f cohesionForce = cohesion(preys) * COHESION_WEIGHT;
//     sf::Vector2f separationForce = separation(preys) * SEPARATION_WEIGHT;
    
//     applyForce(alignment);
//     applyForce(cohesionForce);
//     applyForce(separationForce);
// }

// // void Prey::flock(const std::vector<Prey*>& preys) {
// //     sf::Vector2f alignmentForce(0, 0);
// //     sf::Vector2f cohesionForce(0, 0);
// //     sf::Vector2f separationForce(0, 0);
    
// //     int alignCount = 0;
// //     int cohesionCount = 0;
// //     int separationCount = 0;
    
// //     for (Prey* other : preys) {
// //         float d = Vector2Util::distance(position, other->position);
        
// //         // Alignment
// //         if (d < PERCEPTION_RADIUS) {
// //             alignmentForce += other->velocity;
// //             alignCount++;
// //         }
        
// //         // Cohesion
// //         if (d < PERCEPTION_RADIUS) {
// //             cohesionForce += other->position;
// //             cohesionCount++;
// //         }
        
// //         // Separation
// //         if (d < SEPARATION_RADIUS) {
// //             sf::Vector2f diff = position - other->position;
// //             diff = Vector2Util::normalize(diff);
// //             diff /= d;  // Closer objects have more influence
// //             separationForce += diff;
// //             separationCount++;
// //         }
// //     }
    
// //     applyForce(alignmentForce);
// //     applyForce(cohesionForce);
// //     applyForce(separationForce);
// // }

// sf::Vector2f Prey::align(const std::vector<Prey*>& preys) const {
//     sf::Vector2f steering(0, 0);
//     int total = 0;
    
//     for (Prey* other : preys) {
//         float distance = Vector2Util::distance(position, other->position);
//         if (other != this && distance < PERCEPTION_RADIUS) {
//             steering += other->velocity;
//             total++;
//         }
//     }
    
//     if (total > 0) {
//         steering /= static_cast<float>(total);
//         steering = Vector2Util::normalize(steering);
//         steering *= MAX_SPEED;
        
//         sf::Vector2f steer = steering - velocity;
//         if (Vector2Util::calc_length(steer) > MAX_FORCE) {
//             steer = Vector2Util::scaleTo(steer, MAX_FORCE);
//         }
        
//         return steer;
//     }
    
//     return sf::Vector2f(0, 0);
// }

// sf::Vector2f Prey::cohesion(const std::vector<Prey*>& preys) const {
//     sf::Vector2f steering(0, 0);
//     int total = 0;
    
//     for (const auto& other : preys) {
//         float distance = Vector2Util::distance(position, other->position);
//         if (other != this && distance < PERCEPTION_RADIUS) {
//             steering += other->position;
//             total++;
//         }
//     }
    
//     if (total > 0) {
//         steering /= static_cast<float>(total);
//         steering -= position;
//         steering = Vector2Util::normalize(steering);
//         steering *= MAX_SPEED;
        
//         sf::Vector2f steer = steering - velocity;
//         if (Vector2Util::calc_length(steer) > MAX_FORCE) {
//             steer = Vector2Util::scaleTo(steer, MAX_FORCE);
//         }
        
//         return steer;
//     }
    
//     return sf::Vector2f(0, 0);
// }

// sf::Vector2f Prey::separation(const std::vector<Prey*>& preys) const {
//     sf::Vector2f steering(0, 0);
//     int total = 0;
    
//     for (const auto& other : preys) {
//         float distance = Vector2Util::distance(position, other->position);
//         if (other != this && distance < SEPARATION_RADIUS) {
//             sf::Vector2f diff = position - other->position;
//             diff /= distance;
//             steering += diff;
//             total++;
//         }
//     }
    
//     if (total > 0) {
//         steering /= static_cast<float>(total);
//         steering = Vector2Util::normalize(steering);
//         steering *= MAX_SPEED;
        
//         sf::Vector2f steer = steering - velocity;
//         if (Vector2Util::calc_length(steer) > MAX_FORCE) {
//             steer = Vector2Util::scaleTo(steer, MAX_FORCE);
//         }
        
//         return steer;
//     }
    
//     return sf::Vector2f(0, 0);
// }


// sf::Vector2f Prey::avoidBorders() const {
//     return Boid::avoidBorders();
// }
