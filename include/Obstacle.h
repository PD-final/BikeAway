#pragma once
#include "Object.h"

enum class ObstacleType { Bike, Car, Pedestrian };

class Obstacle : public Object {
   public:
    ObstacleType type;
    sf::Vector2f velocity;
    int damage = 1;

    void update(sf::Image* img_thin, float mapScale);
    sf::Vector2i nextWhite(sf::Image* img_thin, int SEARCH_RADIUS, int posx,
                           int posy);
    // override { sprite.move(velocity * dt); }
};
