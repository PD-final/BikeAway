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
    void chooseDestination();
    sf::Vector2f pickRandomSpawn() const;
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
    sf::Texture mapOverlayTexture;
    sf::Texture playerTexture;
    sf::Texture bikeTexture;
    sf::Texture bikeTextureFront;
    sf::Texture bikeTextureLeft;
    sf::Texture bikeTextureRight;
    sf::Texture bikeTextureBack;
    sf::Texture heartTexture;
    sf::Texture entryTexture;
    sf::Sprite entrySprite;
    sf::Texture failTexture;
    sf::Sprite failSprite;
    sf::Texture successTexture;
    sf::Sprite successSprite;
    sf::Texture playerTextureUp;
    sf::Texture playerTextureDown;      
    sf::Texture playerTextureLeft;
    sf::Texture playerTextureRight;

    Character player;
    Map map;
    Building startBuilding;
    Building destinationBuilding;
    
    sf::Clock gameClock;   // 從遊戲開始到現在經過多久
    const float timeLimitSeconds = 60.f;
    sf::Font uiFont;       // 顯示文字用
    sf::Text timerText;    // 左上角的時間文字
    sf::Text missionText;
    sf::Text winText;
    sf::Text failText;
    sf::Text invincibleText;
    sf::Text reachedText;
    float hitFlashSeconds = 0.f;
    float spawnInvincibleSeconds = 0.f;
    float heartInvincibleSeconds = 0.f;
    bool hasReachedDestination = false;

    ScreenState screen = ScreenState::Home;
    sf::RectangleShape homeBackground;
    sf::Text titleText;
    sf::Text subtitleText;
    sf::Text startPromptText;
};
