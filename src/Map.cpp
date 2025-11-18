#include "Map.h"

Map::Map() {}

void Map::setTexture(const sf::Texture& tex) {
    mapSprite.setTexture(tex);
    mapSprite.setScale(1.1f, 1.1f);   // 完整放大兩倍
}

void Map::update(float dt) {
    for (auto& o : obstacles)
        o.update(dt);
}

void Map::draw(sf::RenderTarget& target) const {
    target.draw(mapSprite);

    for (auto& o : obstacles)
        o.draw(target);

    for (auto& p : powerups)
        p.draw(target);
}
