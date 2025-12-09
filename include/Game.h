#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Map.h"

enum class ScreenState { Home, Playing, Win, Fail };

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
    void onEnterWin();
    void onEnterFail();

    void handleHomeEvent(const sf::Event& event);
    void handlePlayingEvent(const sf::Event& event);
    void handleWinEvent(const sf::Event& event);
    void handleFailEvent(const sf::Event& event);

    void updateHome(sf::Time dt);
    void updatePlaying(sf::Time dt);
    void updateWin(sf::Time dt);
    void updateFail(sf::Time dt);

    void renderHome();
    void renderPlaying();
    void renderWin();
    void renderFail();

    void setupHomeUI();
    void updateHomeLayout(sf::Vector2u size);
    void chooseStartAndDestination();
    sf::Vector2f buildingCenter(const Building& b) const;
    void drawBuildingMarkers(sf::RenderTarget& target) const;
    void drawBuildingOutlines(sf::RenderTarget& target) const;
    bool playerOnAnyRoad();
    void spawnBikesOnRoads();
    float setBikeTextureForDirection(Obstacle& bike, const sf::Vector2f& dir);
    void setBottomHitbox(Object& obj, float fraction = 0.33f);

    sf::RenderWindow window;
    sf::View view;

    sf::Texture mapTexture;
    sf::Texture playerTexture;
    sf::Texture bikeTexture;
    sf::Texture bikeTextureFront;
    sf::Texture bikeTextureLeft;
    sf::Texture bikeTextureRight;
    sf::Texture playerTextureUp;
    sf::Texture playerTextureDown;      
    sf::Texture playerTextureLeft;
    sf::Texture playerTextureRight;

    Character player;
    Map map;
    Building startBuilding;
    Building destinationBuilding;
    
    sf::Clock gameClock;   // 從遊戲開始到現在經過多久
    sf::Font uiFont;       // 顯示文字用
    sf::Text timerText;    // 左上角的時間文字
    sf::Text missionText;
    sf::Text winText;
    sf::Text failText;

    ScreenState screen = ScreenState::Home;
    sf::RectangleShape homeBackground;
    sf::Text titleText;
    sf::Text subtitleText;
    sf::Text startPromptText;
};
