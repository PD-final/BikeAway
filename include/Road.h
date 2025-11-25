#pragma once

#include <SFML/Graphics.hpp>

class Road {
public:
    sf::Vector2f start, end;
    int width;
    Road(sf::Vector2f _start, sf::Vector2f _end, int width) : start(_start), end(_end), width(width) {}
};