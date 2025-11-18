#pragma once
#include "Object.h"

enum class ObstacleType { Bike, Car, Pedestrian };

class Obstacle : public Object {
public:
    ObstacleType type;
    sf::Vector2f velocity;
    int damage = 1;

    void update(float dt) override {
        sprite.move(velocity * dt);
    }
};
