#include "Map.h"
// #include <iostream>
Map::Map() {}

void Map::setImg(sf::Image* gameMap) { img = gameMap; }
void Map::setThinImg(sf::Image* gameMap) { img_thin = gameMap; }

void Map::setTexture(const sf::Texture& tex) {
    mapSprite.setTexture(tex);
    mapSprite.setScale(mapScale, mapScale);
}

void Map::update() {
    // std::cout << "Map update called\n";
    for (auto& o : obstacles) o.update(img_thin, mapScale);
}

void Map::draw(sf::RenderTarget& target) const {
    target.draw(mapSprite);

    for (auto& o : obstacles) o.draw(target);

    for (auto& p : powerups) p.draw(target);
}
