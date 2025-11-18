#pragma once
#include "Object.h"

class Character : public Object {
public:
    Character();
    void handleInput(float dt);

    sf::Vector2f worldPos;
    float speed = 300.f;
};
