#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

/**
 * @brief Utility functions for working with 2D vectors
 */
namespace Vector2Util {

    /**
     * @brief Calculates the magnitude length of a vector
     * 
     * @param v Input vector
     * @return The length of the vector
     */
    inline float calc_length(const sf::Vector2f& v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    /**
     * @brief Normalizes a vector to unit length
     * 
     * @param v Input vector
     * @return Normalized vector, if length is 1, or original vector if length is 0
     */
    inline sf::Vector2f normalize(const sf::Vector2f& v) {
        float len = calc_length(v);
        if (len != 0)
            return sf::Vector2f(v.x / len, v.y / len);
        return v;
    }

    /**
     * @brief Scales a vector to a specific length
     * 
     * @param v Input vector
     * @param newLength Desired new length
     * @return Vector scaled to the new length
     */
    inline sf::Vector2f scaleTo(const sf::Vector2f& v, float newLength) {
        sf::Vector2f normalized = normalize(v);
        return normalized * newLength;
    }

    /**
     * @brief Calculates the distance between two vectors
     * 
     * @param v1 First vector
     * @param v2 Second vector
     * @return Distance between the two points
     */
    inline float distance(const sf::Vector2f& v1, const sf::Vector2f& v2) {
        sf::Vector2f diff = v1 - v2;
        return calc_length(diff);
    }
}
