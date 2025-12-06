#include "Character.h"

#include <cmath>
#include <iostream>
#include <random>

Character::Character() {}

sf::Vector2f Character::randWorldPos(const Map& map) {
    // find a random non-transparent starting position
    sf::Vector2u mapSize = map.img->getSize();
    std::cout << "Map size: " << mapSize.x << " , " << mapSize.y << std::endl;

    std::random_device rd;
    // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());
    // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<unsigned int> disX(0, mapSize.x - 1);
    std::uniform_int_distribution<unsigned int> disY(0, mapSize.y - 1);

    unsigned int randX, randY;
    bool validPositionFound = false;
    int attempts = 0;
    const int MAX_ATTEMPTS = 5000;  // Safety break to prevent infinite loops

    // Loop until we find a non-transparent pixel
    while (attempts < MAX_ATTEMPTS) {
        randX = disX(gen);
        randY = disY(gen);

        // Check alpha channel (0 is fully transparent)
        if (map.img->getPixel(randX, randY).a != 0) {
            validPositionFound = true;
            break;
        }
        attempts++;
    }

    if (validPositionFound) {
        return {static_cast<float>(randX), static_cast<float>(randY)};
        // Optional: Immediately sync sprite position to avoid 1-frame jump
        // sprite.setPosition(worldPos);
    } else {
        // Fallback if map is empty or something went wrong
        std::cerr
            << "Warning: Could not find valid spawn point. Defaulting to (0,0)."
            << std::endl;
        return {0.f, 0.f};
        // sprite.setPosition({0.f, 0.f});
    }
}

void Character::setTextures(sf::Texture* up, sf::Texture* down,
                            sf::Texture* left, sf::Texture* right) {
    texUp = up;
    texDown = down;
    texLeft = left;
    texRight = right;

    // 初始使用向下貼圖
    sprite.setTexture(*texDown);
}

void Character::handleInput(float dt, const Map& map) {
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
        float len = std::sqrt(dirMove.x * dirMove.x + dirMove.y * dirMove.y);
        dirMove /= len;

        sf::Vector2f newPos = worldPos + dirMove * speed * dt;
        // 檢查newPos是否在map非transparent區域

        // Check for negative coordinates FIRST
        // Casting a negative float to unsigned int results in a huge number
        // (overflow), causing a crash.
        if (newPos.x >= 0 && newPos.y >= 0) {
            sf::Vector2u mapSize = map.img->getSize();
            unsigned int x = static_cast<unsigned int>(newPos.x / map.mapScale);
            unsigned int y = static_cast<unsigned int>(newPos.y / map.mapScale);

            std::cout << "(" << x << ", " << y << ")" << std::endl;
            if (x < mapSize.x && y < mapSize.y) {
                if (map.img->getPixel(x, y).a != 0) {
                    worldPos += dirMove * speed * dt;
                }
            }
        }
    }
}
