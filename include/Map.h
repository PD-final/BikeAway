#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "Obstacle.h"
#include "PowerUp.h"

class Map {
   public:
    Map();
    void setTexture(const sf::Texture& tex);
    void setImg(sf::Image* gameMap);
    void setThinImg(sf::Image* gameMap);

    void update();
    void draw(sf::RenderTarget& target) const;

    sf::Sprite mapSprite;

    // std::vector<Road> roads;
    std::vector<Obstacle> obstacles;
    std::vector<PowerUp> powerups;
    sf::Image* img = nullptr;
    sf::Image* img_thin = nullptr;

    const float mapScale = 3.f;
};
