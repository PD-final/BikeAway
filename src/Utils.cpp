#include "Utils.h"
#include <cmath>

float dot(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
}

float cross(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x * b.y - a.y * b.x;
}

float abs(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}