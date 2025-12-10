#pragma once
#include <SFML/Graphics.hpp>
#include "Road.h"
#include <array>

class Object {
public:
    virtual ~Object() = default;

    virtual void update(float) {}
    virtual void draw(sf::RenderTarget& target) const {
        target.draw(sprite);
    }

    void setPosition(const sf::Vector2f& p) { sprite.setPosition(p); }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Sprite sprite;
    bool on_road(Road &road);
    std::array<sf::Vector2f, 4> hitbox;
    std::array<sf::Vector2f, 4> getTransformedHitbox() const ;
    bool collidesWith(const Object& other) const ;
};