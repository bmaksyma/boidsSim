#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace Vector2Util {
    inline float calc_length(const sf::Vector2f& v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }
    
    inline sf::Vector2f normalize(const sf::Vector2f& v) {
        float len = calc_length(v);
        if (len != 0)
            return sf::Vector2f(v.x / len, v.y / len);
        return v;
    }

    inline sf::Vector2f scaleTo(const sf::Vector2f& v, float newLength) {
        sf::Vector2f normalized = normalize(v);
        return normalized * newLength;
    }
    
    inline float distance(const sf::Vector2f& v1, const sf::Vector2f& v2) {
        sf::Vector2f diff = v1 - v2;
        return calc_length(diff);
        // return diff.x * diff.x + diff.y * diff.y;
    }
}