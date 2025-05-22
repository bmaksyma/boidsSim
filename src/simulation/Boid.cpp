// #include "simulation/Boid.hpp"
// #include <random>
// #include "utils/draw_utils.h"
// #include <cmath>

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

// extern unsigned short *fb;

// extern std::mt19937 gen;
// extern std::uniform_real_distribution<float> dis;
// extern std::uniform_real_distribution<float> speedDis;

// Boid::Boid(float x, float y, float size) : position(x, y), size(size), color(0x981e) {
//     velocity = sf::Vector2f(dis(gen), dis(gen));
//     velocity = Vector2Util::scaleTo(velocity, speedDis(gen));
//     acceleration = sf::Vector2f(0, 0);
// }

// void Boid::update() {
//     sf::Vector2f borderForce = avoidBorders();
//     applyForce(borderForce);

//     velocity += acceleration;

//     if (Vector2Util::calc_length(velocity) > MAX_SPEED) {
//         velocity = Vector2Util::scaleTo(velocity, MAX_SPEED);
//     }

//     position += velocity;
    
//     acceleration = sf::Vector2f(0, 0);
// }

// sf::Vector2f Boid::avoidBorders() const {
//     sf::Vector2f steering(0, 0);
    
//     if (position.x < MARGIN_SIZE) {
//         steering.x += TURN_FORCE * (1.0f - position.x / MARGIN_SIZE);
//     } else if (position.x > WIDTH - MARGIN_SIZE) {
//         steering.x -= TURN_FORCE * (1.0f - (WIDTH - position.x) / MARGIN_SIZE);
//     }
    
//     if (position.y < MARGIN_SIZE) {
//         steering.y += TURN_FORCE * (1.0f - position.y / MARGIN_SIZE);
//     } else if (position.y > HEIGHT - MARGIN_SIZE) {
//         steering.y -= TURN_FORCE * (1.0f - (HEIGHT - position.y) / MARGIN_SIZE);
//     }
    
//     steering = Vector2Util::calc_length(steering) > MAX_FORCE ? Vector2Util::scaleTo(steering, MAX_FORCE) : steering;
    
//     return steering;
// }

// void Boid::applyForce(const sf::Vector2f& force) {
//     acceleration += force;
// }

// void Boid::draw() const {
//     float angle = std::atan2(velocity.y, velocity.x);

//     float rad1 = angle;
//     float rad2 = angle + 140 * M_PI / 180;
//     float rad3 = angle - 140 * M_PI / 180;

//     int x1 = static_cast<int>(position.x + size * std::cos(rad1));
//     int y1 = static_cast<int>(position.y + size * std::sin(rad1));
//     int x2 = static_cast<int>(position.x + size / 2 * std::cos(rad2));
//     int y2 = static_cast<int>(position.y + size / 2 * std::sin(rad2));
//     int x3 = static_cast<int>(position.x + size / 2 * std::cos(rad3));
//     int y3 = static_cast<int>(position.y + size / 2 * std::sin(rad3));

//     auto sortVertices = [](int& x1, int& y1, int& x2, int& y2, int& x3, int& y3) {
//         if (y1 > y2) {
//             std::swap(x1, x2);
//             std::swap(y1, y2);
//         }
//         if (y1 > y3) {
//             std::swap(x1, x3);
//             std::swap(y1, y3);
//         }
//         if (y2 > y3) {
//             std::swap(x2, x3);
//             std::swap(y2, y3);
//         }
//     };

//     sortVertices(x1, y1, x2, y2, x3, y3);

//     auto drawHorizontalLine = [&](int x0, int x1, int y) {
//         if (y >= 0 && y < 320) {
//             if (x0 > x1) std::swap(x0, x1);
//             for (int x = std::max(0, x0); x <= std::min(479, x1); x++) {
//                 draw_pixel(x, y, color);
//             }
//         }
//     };

//     auto interpolate = [](int x0, int y0, int x1, int y1, int y) -> int {
//         if (y1 == y0) return x0;
//         return x0 + (x1 - x0) * (y - y0) / (y1 - y0);
//     };

//     for (int y = y1; y <= y3; y++) {
//         if (y < y2) {
//             int xA = interpolate(x1, y1, x3, y3, y);
//             int xB = interpolate(x1, y1, x2, y2, y);
//             drawHorizontalLine(xA, xB, y);
//         } else {
//             int xA = interpolate(x1, y1, x3, y3, y);
//             int xB = interpolate(x2, y2, x3, y3, y);
//             drawHorizontalLine(xA, xB, y);
//         }
//     }
// }