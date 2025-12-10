#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include <string>
#include <array>


class Building : public Object {
public:
    std::string name;
    Building() = default;
    Building(const std::string& building_name) : name(building_name) {}
};
