#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <cmath>
#include <iostream>
#include "Vector2Util.hpp"
#include "Boid.hpp"
#include "Predator.hpp"
#include "Prey.hpp"

extern const int WIDTH = 480;
extern const int HEIGHT = 320;
extern const float MAX_SPEED = 10.0f;
extern const float MAX_FORCE = 1.3f;
extern const float PERCEPTION_RADIUS = 25.0f;
extern const float SEPARATION_RADIUS = 10.0f;
extern const float FOOD_ATTRACTION = 0.8f;
extern const float ALIGNMENT_WEIGHT = 1.0f;
extern const float COHESION_WEIGHT = 1.0f;
extern const float SEPARATION_WEIGHT = 1.5f;
extern const float MARGIN_SIZE = 40.0f;
extern const float TURN_FORCE = 5.0f;
extern const float PREY_SIZE = 10.0f;
extern const float PREDATOR_SIZE = 20.0f;
extern const int PREYS_COUNT = 30;
extern const int PREDATORS_COUNT = 5;
extern const float KILL_DISTANCE = 5.0f;

std::mt19937 gen;
std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
std::uniform_real_distribution<float> speedDis(1.0f, MAX_SPEED);

int main() {
    std::random_device rd;
    gen = std::mt19937(rd());
    std::uniform_int_distribution<> posDis_x(0, WIDTH);
    std::uniform_int_distribution<> posDis_y(0, HEIGHT);
    
    std::vector<Prey> preys;
    preys.reserve(PREYS_COUNT);
    for (int i = 0; i < PREYS_COUNT; i++) {
        preys.emplace_back(posDis_x(gen), posDis_y(gen));
    }

    std::vector<Predator> predators;
    predators.reserve(PREDATORS_COUNT);
    for (int i = 0; i < PREDATORS_COUNT; i++) {
        predators.emplace_back(posDis_x(gen), posDis_y(gen));
    }

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fish Swarm Simulation");
    window.setFramerateLimit(60);
    
    
    // sf::Vector2f* foodPosition = nullptr;

    while (window.isOpen()) {        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        // Simulation logic later...
        
        window.clear(sf::Color(255,255,255)); // White background
        
        for (auto& prey : preys) {
            prey.update();
            prey.flock(preys);
            prey.draw(window);
        }

        for (auto& predator : predators) {
        predator.update();
        predator.hunt(preys);
        
        bool killed = predator.tryToKill(preys);
        
        if (killed) {
            if (preys.size() < PREYS_COUNT) {
            preys.emplace_back(posDis_x(gen), posDis_y(gen));
            }
        }
        predator.draw(window);
    }

        window.display();
    }
    return 0;
}