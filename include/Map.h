#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "PowerUp.h"

class Map {
public:
    Map();
    void setTexture(const sf::Texture& tex);

    void update(float dt);
    void draw(sf::RenderTarget& target) const;

    sf::Sprite mapSprite;

    std::vector<Obstacle> obstacles;
    std::vector<PowerUp> powerups;
};
