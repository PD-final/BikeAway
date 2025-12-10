#pragma once
#include "Object.h"
#include "Road.h"
#include <cmath>
#include <algorithm>

enum class ObstacleType { Bike, Car, Pedestrian };

class Obstacle : public Object {
public:
    ObstacleType type;
    sf::Vector2f velocity;
    int damage = 1;
    const Road* road = nullptr;
    float distanceAlong = 0.f;   // distance along the road centerline
    float speedAlong = 0.f;      // units per second along the centerline
    float lateralOffset = 0.f;   // signed offset from centerline, uses road width radius
    int directionSign = 1;       // 1: start->end, -1: end->start
    float spriteBaseAngle = 0.f; // orientation offset based on texture facing
    float spawnGraceSeconds = 0.f; // ignore collisions right after spawn

    void update(float dt) override {
        if (type == ObstacleType::Bike && road) {
            spawnGraceSeconds = std::max(0.f, spawnGraceSeconds - dt);
            sf::Vector2f dir = (directionSign >= 0) ? (road->end - road->start) : (road->start - road->end);
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (len > 0.0001f) {
                sf::Vector2f u = dir / len;
                sf::Vector2f n{-u.y, u.x};
                distanceAlong += speedAlong * dt;
                // wrap along the road length so bikes loop
                distanceAlong = std::fmod(distanceAlong, len);
                if (distanceAlong < 0.f) distanceAlong += len;
                sf::Vector2f pos = ((directionSign >= 0) ? road->start : road->end) + u * distanceAlong + n * lateralOffset;
                sprite.setPosition(pos);
                float headingDeg = std::atan2(u.y, u.x) * 180.f / 3.14159265f;
                sprite.setRotation(headingDeg - spriteBaseAngle);
            }
        } else {
            sprite.move(velocity * dt);
        }
    }
};
