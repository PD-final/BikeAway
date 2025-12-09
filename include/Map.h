#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "PowerUp.h"
#include "Building.h"
#include "Road.h"

class Map {
public:
    Map();
    void setTexture(const sf::Texture& tex);
    void setOverlayTexture(const sf::Texture& tex);

    void update(float dt);
    void draw(sf::RenderTarget& target) const;
    void drawOverlay(sf::RenderTarget& target) const;
    void loadBuildingsFromJson(const std::string& path);
    void loadRoadsFromJson(const std::string& path);

    sf::Sprite mapSprite;
    sf::Sprite overlaySprite;

    std::vector<Obstacle> obstacles;
    std::vector<PowerUp> powerups;
    std::vector<Building> buildings;
    std::vector<Road> roads;
};
