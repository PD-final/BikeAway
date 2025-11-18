#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Map.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time dt);
    void render();

    sf::RenderWindow window;
    sf::View view;

    sf::Texture mapTexture;
    sf::Texture playerTexture;
    sf::Texture bikeTexture;

    Character player;
    Map map;
};
