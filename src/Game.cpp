#include "Game.h"

#include <cstring>
#include <iostream>
#include <random>

#include "thinMap.h"

namespace {
sf::String utf8(const char* text) {
    auto len = std::strlen(text);
    return sf::String::fromUtf8(text, text + len);
}
}  // namespace
Game::Game() : window(sf::VideoMode(1280, 720), "BikeAway") {
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    mapTexture.loadFromFile("assets/ntu_map.png");
    thinCreator.create("assets/ntu_map.png");
    // map.roads = roadsCreator.getRoads();

    playerTexture.loadFromFile("assets/player_front.png");
    bikeTexture.loadFromFile("assets/bike.png");
    playerTextureUp.loadFromFile("assets/player_back.png");
    playerTextureDown.loadFromFile("assets/player_front.png");
    playerTextureLeft.loadFromFile("assets/player_left.png");
    playerTextureRight.loadFromFile("assets/player_right.png");

    player.setTextures(&playerTextureUp, &playerTextureDown, &playerTextureLeft,
                       &playerTextureRight);

    worldMap.loadFromFile("assets/ntu_map.png");
    map.setImg(&worldMap);
    map.setThinImg(&thinCreator.thinImage);
    map.setTexture(mapTexture);

    // setup player
    player.sprite.setTexture(playerTexture);
    player.sprite.setScale(0.15f, 0.15f);  // 變成 30% 大小
    player.tmpWorldPos = player.randWorldPos(map) * map.mapScale;
    std::cout << "Player initial world pos: (" << player.tmpWorldPos.x << ", "
              << player.tmpWorldPos.y << ")\n";
    player.worldPos = player.tmpWorldPos;
    sf::FloatRect bounds = player.sprite.getLocalBounds();
    player.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // camera
    view.setSize(window.getSize().x, window.getSize().y);
    // view.setCenter(player.worldPos);
    // ====== 計時器 UI 初始化 ======
    if (!uiFont.loadFromFile("assets/NotoSansTC-Regular.ttf")) {
        std::cerr << "Failed to load font: assets/NotoSansTC-Regular.ttf\n";
    }

    timerText.setFont(uiFont);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setOutlineColor(sf::Color::Black);
    timerText.setOutlineThickness(2.f);
    timerText.setPosition(10.f, 10.f);  // 左上角

    timerText.setString("00:00");  // 初始顯示
    setupHomeUI();
    changeScreen(ScreenState::Home);

    // sample obstacle
    unsigned int seed = static_cast<unsigned int>(time(nullptr));
    srand(seed);

    sf::Vector2u mapSize = map.img_thin->getSize();
    std::random_device rd;
    // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());
    // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<unsigned int> disX(0, mapSize.x - 1);
    std::uniform_int_distribution<unsigned int> disY(0, mapSize.y - 1);

    for (int i = 0; i < 10; i++) {
        Obstacle obs;
        obs.sprite.setTexture(bikeTexture);
        obs.type = ObstacleType::Bike;

        sf::Vector2f pos = static_cast<sf::Vector2f>(obs.findRandomWhite(
                               map.img_thin, 200, disX(gen), disY(gen))) *
                           map.mapScale;
        obs.setPosition(pos);
        // obs.setPosition(
        //     player.tmpWorldPos +
        //     sf::Vector2f(float(rand() % 61 - 30), float(rand() % 61 -
        //     30)));
        // obs.setPosition(
        //     {1000.f + rand() % 601 - 300, 1000.f + rand() % 601 - 300});
        obs.sprite.setScale(0.3f, 0.3f);  // 變成 50% 大小
        obs.velocity = {0.0f, 0.0f};
        // obs.velocity = {float(rand() % 61 - 30), float(rand() % 61 -
        // 30)};
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
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::Resized) {
            sf::Vector2u newSize(event.size.width, event.size.height);
            view.setSize(static_cast<float>(newSize.x),
                         static_cast<float>(newSize.y));
            updateHomeLayout(newSize);
        }

        switch (screen) {
            case ScreenState::Home:
                handleHomeEvent(event);
                break;
            case ScreenState::Playing:
                handlePlayingEvent(event);
                break;
        }
    }
}

void Game::update(sf::Time dt) {
    switch (screen) {
        case ScreenState::Home:
            updateHome(dt);
            break;
        case ScreenState::Playing:
            updatePlaying(dt);
            break;
    }
}

void Game::render() {
    window.clear();

    switch (screen) {
        case ScreenState::Home:
            renderHome();
            break;
        case ScreenState::Playing:
            renderPlaying();
            break;
    }

    window.display();
}

void Game::setupHomeUI() {
    homeBackground.setFillColor(sf::Color(18, 26, 60));
    updateHomeLayout(window.getSize());

    titleText.setFont(uiFont);
    titleText.setString("BikeAway");
    titleText.setCharacterSize(72);
    titleText.setFillColor(sf::Color::White);

    subtitleText.setFont(uiFont);
    subtitleText.setString(utf8(u8"騎上單車，閃避障礙物"));
    subtitleText.setCharacterSize(32);
    subtitleText.setFillColor(sf::Color(200, 220, 255));

    startPromptText.setFont(uiFont);
    startPromptText.setString(
        utf8(u8"按 Enter / Space 開始遊戲，Esc 返回首頁"));
    startPromptText.setCharacterSize(24);
    startPromptText.setFillColor(sf::Color(180, 200, 255));

    updateHomeLayout(window.getSize());
}

void Game::handleHomeEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyReleased &&
        (event.key.code == sf::Keyboard::Enter ||
         event.key.code == sf::Keyboard::Space)) {
        changeScreen(ScreenState::Playing);
    }
}

void Game::handlePlayingEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyReleased &&
        event.key.code == sf::Keyboard::Escape) {
        changeScreen(ScreenState::Home);
    }
}

void Game::updateHomeLayout(sf::Vector2u size) {
    sf::Vector2f fSize(static_cast<float>(size.x), static_cast<float>(size.y));
    homeBackground.setSize(fSize);

    auto centerText = [](sf::Text& text, float y) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.f,
                       bounds.top + bounds.height / 2.f);
        text.setPosition(sf::Vector2f(0.f, y));
    };

    centerText(titleText, fSize.y * 0.3f);
    titleText.move(fSize.x / 2.f, 0.f);

    centerText(subtitleText, fSize.y * 0.45f);
    subtitleText.move(fSize.x / 2.f, 0.f);

    centerText(startPromptText, fSize.y * 0.65f);
    startPromptText.move(fSize.x / 2.f, 0.f);
}

void Game::changeScreen(ScreenState next) {
    screen = next;
    switch (screen) {
        case ScreenState::Home:
            onEnterHome();
            break;
        case ScreenState::Playing:
            onEnterPlaying();
            break;
    }
}

void Game::onEnterPlaying() {
    // player.worldPos = {2000.f, 2000.f};
    player.worldPos = player.tmpWorldPos;
    view.setCenter(player.worldPos);
    map.mapSprite.setPosition(0.f, 0.f);
    gameClock.restart();
}

void Game::onEnterHome() { window.setView(window.getDefaultView()); }

void Game::updateHome(sf::Time) {
    // No-op for now; placeholder for future home animations or timers.
}

void Game::updatePlaying(sf::Time dt) {
    float dtSec = dt.asSeconds();

    player.handleInput(dtSec, map);

    // update view center to player's world position
    view.setCenter(player.worldPos);

    // map.update(dtSec);
    map.update();
    // ====== 更新計時器文字 ======
    sf::Time elapsed = gameClock.getElapsedTime();
    int totalSec = static_cast<int>(elapsed.asSeconds());

    int minutes = totalSec / 60;
    int seconds = totalSec % 60;

    char buffer[16];
    std::snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);
    timerText.setString(buffer);
}

void Game::renderHome() {
    window.setView(window.getDefaultView());
    window.draw(homeBackground);
    window.draw(titleText);
    window.draw(subtitleText);
    window.draw(startPromptText);
}

void Game::renderPlaying() {
    window.setView(view);

    map.draw(window);
    player.setPosition(view.getCenter());
    player.draw(window);

    // ==== 2. 畫 UI（固定螢幕座標）====
    window.setView(window.getDefaultView());
    window.draw(timerText);
}
