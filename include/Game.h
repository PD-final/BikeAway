#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Map.h"

enum class ScreenState { Home, Playing };

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time dt);
    void render();
    void changeScreen(ScreenState next);
    void onEnterHome();
    void onEnterPlaying();

    void handleHomeEvent(const sf::Event& event);
    void handlePlayingEvent(const sf::Event& event);

    void updateHome(sf::Time dt);
    void updatePlaying(sf::Time dt);

    void renderHome();
    void renderPlaying();

    void setupHomeUI();
    void updateHomeLayout(sf::Vector2u size);

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

    ScreenState screen = ScreenState::Home;
    sf::RectangleShape homeBackground;
    sf::Text titleText;
    sf::Text subtitleText;
    sf::Text startPromptText;
};
