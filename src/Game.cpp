#include "Game.h"
#include <iostream>
Game::Game()
: window(sf::VideoMode(1280, 720), "BikeAway")
{
    window.setFramerateLimit(60);

    mapTexture.loadFromFile("assets/ntu_map.png");
    playerTexture.loadFromFile("assets/player.png");
    bikeTexture.loadFromFile("assets/bike.png");
    playerTextureUp.loadFromFile("assets/player.png");
    playerTextureDown.loadFromFile("assets/player.png");
    playerTextureLeft.loadFromFile("assets/player_left.png");
    playerTextureRight.loadFromFile("assets/player_right.png");

    player.setTextures(
        &playerTextureUp,
        &playerTextureDown,
        &playerTextureLeft,
        &playerTextureRight
    );

    map.setTexture(mapTexture);

    // setup player
    player.sprite.setTexture(playerTexture);
    player.sprite.setScale(0.15f, 0.15f);   // 變成 30% 大小
    player.worldPos = {2000.f, 2000.f};
    sf::FloatRect bounds = player.sprite.getLocalBounds();
    player.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);


    // camera
    view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(player.worldPos);
    // ====== 計時器 UI 初始化 ======
    uiFont.loadFromFile("assets/NotoSans-Regular.ttf");

    timerText.setFont(uiFont);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setOutlineColor(sf::Color::Black);
    timerText.setOutlineThickness(2.f);
    timerText.setPosition(10.f, 10.f);  // 左上角

    timerText.setString("00:00");       // 初始顯示
    // sample obstacle
    unsigned int seed = static_cast<unsigned int>(time(nullptr));
    srand(seed);

    for(int i=0;  i<5; i++){
        Obstacle obs;
        obs.sprite.setTexture(bikeTexture);
        obs.type = ObstacleType::Bike;
        obs.setPosition({1000.f + rand()%601 - 300, 1000.f+ rand()%601 - 300});
        obs.sprite.setScale(0.3f, 0.3f);   // 變成 50% 大小
        obs.velocity = {float(rand() % 61 - 30), float(rand() % 61 - 30)    };
        map.obstacles.push_back(obs);
    }
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        processEvents();
        sf::Time dt = clock.restart();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update(sf::Time dt) {
    float dtSec = dt.asSeconds();

    player.handleInput(dtSec);

    // update view center to player's world position
    view.setCenter(player.worldPos);

    map.update(dtSec);
        // ====== 更新計時器文字 ======
    sf::Time elapsed = gameClock.getElapsedTime();
    int totalSec = static_cast<int>(elapsed.asSeconds());

    int minutes = totalSec / 60;
    int seconds = totalSec % 60;

    char buffer[16];
    std::snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);
    timerText.setString(buffer);
}

void Game::render() {
    window.clear();
    window.setView(view);

    map.draw(window);
    player.setPosition(view.getCenter());
    player.draw(window);
    
    // ==== 2. 畫 UI（固定螢幕座標）====
    window.setView(window.getDefaultView());
    window.draw(timerText);

    window.display();
}
