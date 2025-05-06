#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <cmath>
#include <iostream>
#include "Vector2Util.hpp"
#include "Fish.hpp"

extern const int WIDTH = 1280;
extern const int HEIGHT = 720;
extern const int FISH_COUNT = 100;
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
extern const float FISH_SIZE = 10.0f;

std::mt19937 gen;
std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
std::uniform_real_distribution<float> speedDis(1.0f, MAX_SPEED);

int main() {
    std::random_device rd;
    gen = std::mt19937(rd());
    std::uniform_int_distribution<> posDis_x(0, WIDTH);
    std::uniform_int_distribution<> posDis_y(0, HEIGHT);
    
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fish Swarm Simulation");
    window.setFramerateLimit(60);
    
    std::vector<Fish> fishes;
    fishes.reserve(FISH_COUNT);
    for (int i = 0; i < FISH_COUNT - 1; i++) { // -1 to avoid the red testing fish
        fishes.emplace_back(posDis_x(gen), posDis_y(gen));
    }
    fishes.emplace_back(WIDTH / 2, HEIGHT / 2, sf::Color(255, 0, 0)); // Red fish for testing
    
    // sf::Vector2f* foodPosition = nullptr;

    while (window.isOpen()) {        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        // Simulation logic later...
        
        window.clear(sf::Color(25, 25, 40));
        
        for (auto& fish : fishes) {
            fish.update();
            fish.flock(fishes);
            fish.draw(window);
        }

        window.display();
    }
    return 0;
}