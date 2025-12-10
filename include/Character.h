#pragma once
#include "Object.h"

enum class Direction { Up, Down, Left, Right };

class Character : public Object {
public:
    Character();
    void handleInput(float dt);
    sf::Vector2f worldPos;
    float speed = 300.f;
    int hearts = 0;
    float shieldTimer = 0.f; // unused timer placeholder
    void setTextures(
        sf::Texture* up,
        sf::Texture* down,
        sf::Texture* left,
        sf::Texture* right
    );
private:
    std::string name;
    Direction dir = Direction::Down;

    sf::Texture* texUp   = nullptr;
    sf::Texture* texDown = nullptr;
    sf::Texture* texLeft = nullptr;
    sf::Texture* texRight = nullptr;
};
