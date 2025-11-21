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
    sf::Texture playerTextureUp;
    sf::Texture playerTextureDown;      
    sf::Texture playerTextureLeft;
    sf::Texture playerTextureRight;

    Character player;
    Map map;
    
    sf::Clock gameClock;   // 從遊戲開始到現在經過多久
    sf::Font uiFont;       // 顯示文字用
    sf::Text timerText;    // 左上角的時間文字
};
