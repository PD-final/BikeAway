#include "Map.h"
#include <fstream>
#include <iostream>
#include <cmath>
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

    // draw roads overlay (semi-transparent blue). Road coordinates are already in world space.
    sf::RectangleShape roadShape;
    roadShape.setFillColor(sf::Color(80, 140, 255, 90));
    for (const auto& r : roads) {
        sf::Vector2f delta = r.end - r.start;
        float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        float diameter = static_cast<float>(r.width * 2); // width now represents radius
        roadShape.setSize({length, diameter});
        roadShape.setOrigin(0.f, diameter / 2.f);
        roadShape.setPosition(r.start);
        roadShape.setRotation(std::atan2(delta.y, delta.x) * 180.f / 3.14159265f);
        target.draw(roadShape);
    }

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
                corner[0].get<float>()*5,
                corner[1].get<float>()*5
            };
        }

        if (hitboxValid)
            buildings.push_back(building);
    }
}

void Map::loadRoadsFromJson(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open road data: " << path << "\n";
        return;
    }

    nlohmann::json data;
    try {
        file >> data;
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse road data: " << e.what() << "\n";
        return;
    }

    auto roadsIt = data.find("roads");
    if (roadsIt == data.end() || !roadsIt->is_array()) {
        std::cerr << "Road data missing 'roads' array\n";
        return;
    }

    for (const auto& entry : *roadsIt) {
        if (!entry.contains("start") || !entry.contains("end") || !entry.contains("width"))
            continue;
        const auto& s = entry["start"];
        const auto& e = entry["end"];
        if (!s.is_array() || s.size() != 2 || !e.is_array() || e.size() != 2)
            continue;
        Road r(
            {s[0].get<float>()*5, s[1].get<float>()*5},
            {e[0].get<float>()*5, e[1].get<float>()*5},
            entry["width"].get<int>()
        );
        roads.push_back(r);
    }
}
