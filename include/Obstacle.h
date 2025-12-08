#pragma once
#include <random>

#include "Object.h"

enum class ObstacleType { Bike, Car, Pedestrian };

class Obstacle : public Object {
   public:
    Obstacle();
    ObstacleType type;
    sf::Vector2f velocity;
    int damage = 1;

    void update(sf::Image* img_thin, float mapScale);
    sf::Vector2i findRandomWhite(sf::Image* img_thin, int SEARCH_RADIUS,
                                 int posx, int posy);
    // override { sprite.move(velocity * dt); }

   private:
    sf::Vector2f currentPos;
    std::mt19937 m_gen;
};
