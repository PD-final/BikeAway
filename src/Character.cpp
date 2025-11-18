#include "Character.h"
#include <cmath>

Character::Character() {}

void Character::handleInput(float dt) {
    sf::Vector2f dir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dir.y -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dir.y += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dir.x -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dir.x += 1;

    if (dir.x != 0 || dir.y != 0) {
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        dir /= len;
        worldPos += dir * speed * dt;
    }
}
