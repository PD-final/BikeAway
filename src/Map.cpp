#include "Map.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

Map::Map() {}

void Map::setTexture(const sf::Texture& tex) {
    mapSprite.setTexture(tex);
    mapSprite.setScale(5.f, 5.f);  
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

void Map::loadBuildingsFromJson(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open building data: " << path << "\n";
        return;
    }

    nlohmann::json data;
    try {
        file >> data;
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse building data: " << e.what() << "\n";
        return;
    }

    auto objectIt = data.find("object");
    if (objectIt == data.end() || !objectIt->is_object()) {
        std::cerr << "Building data missing 'object' field\n";
        return;
    }

    for (const auto& [name, entry] : objectIt->items()) {
        if (!entry.contains("type") || entry["type"] != "Building")
            continue;

        if (!entry.contains("hitbox") || !entry["hitbox"].is_array() || entry["hitbox"].size() != 4)
            continue;

        Building building(name);
        bool hitboxValid = true;
        for (size_t i = 0; i < 4; ++i) {
            const auto& corner = entry["hitbox"][i];
            if (!corner.is_array() || corner.size() != 2) {
                hitboxValid = false;
                break;
            }
            building.hitbox[i] = {
                corner[0].get<float>(),
                corner[1].get<float>()
            };
        }

        if (hitboxValid)
            buildings.push_back(building);
    }
}
