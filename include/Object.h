#pragma once
#include <SFML/Graphics.hpp>

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
    // std::array<sf::Vector2f, 4> hitbox;
    sf::Sprite sprite;
};

