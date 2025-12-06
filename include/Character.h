#pragma once
#include "Map.h"
#include "Object.h"

enum class Direction { Up, Down, Left, Right };

class Character : public Object {
   public:
    Character();
    void handleInput(float dt, const Map& map);
    sf::Vector2f worldPos;
    float speed = 300.f;
    void setTextures(sf::Texture* up, sf::Texture* down, sf::Texture* left,
                     sf::Texture* right);
    sf::Vector2f randWorldPos(const Map& map);
    sf::Vector2f tmpWorldPos;

   private:
    std::string name;
    Direction dir = Direction::Down;

    sf::Texture* texUp = nullptr;
    sf::Texture* texDown = nullptr;
    sf::Texture* texLeft = nullptr;
    sf::Texture* texRight = nullptr;
};
