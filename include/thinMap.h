#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

// #include "Game.h"
// #include "Road.h"

class thinMap {
   public:
    void create(std::string filename);
    sf::Image thinImage;
    // std::vector<Road> getRoads() const { return roads; }

    //    private:
    //     std::vector<Road> roads;
};
