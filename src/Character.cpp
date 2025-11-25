#include "Character.h"
#include <cmath>

Character::Character() {}

void Character::setTextures(
    sf::Texture* up,
    sf::Texture* down,
    sf::Texture* left,
    sf::Texture* right
) {
    texUp = up;
    texDown = down;
    texLeft = left;
    texRight = right;

    // 初始使用向下貼圖
    sprite.setTexture(*texDown);
}

void Character::handleInput(float dt) {
    sf::Vector2f dirMove(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        dir = Direction::Up;
        sprite.setTexture(*texUp);
        dirMove.y -= 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        dir = Direction::Down;
        sprite.setTexture(*texDown);
        dirMove.y += 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        dir = Direction::Left;
        sprite.setTexture(*texLeft);
        dirMove.x -= 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        dir = Direction::Right;
        sprite.setTexture(*texRight);
        dirMove.x += 1;
    }

    // 正規化移動
    if (dirMove.x != 0 || dirMove.y != 0) {
        float len = std::sqrt(dirMove.x*dirMove.x + dirMove.y*dirMove.y);
        dirMove /= len;
        worldPos += dirMove * speed * dt;
    }
}
