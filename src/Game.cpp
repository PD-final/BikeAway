#include "Game.h"
#include <iostream>
#include <cstring>

namespace {
sf::String utf8(const char* text) {
    auto len = std::strlen(text);
    return sf::String::fromUtf8(text, text + len);
}
}
Game::Game()
: window(sf::VideoMode(1280, 720), "BikeAway")
{
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    mapTexture.loadFromFile("assets/ntu_map.png");
    playerTexture.loadFromFile("assets/player_front.png");
    bikeTexture.loadFromFile("assets/bike.png");
    playerTextureUp.loadFromFile("assets/player_back.png");
    playerTextureDown.loadFromFile("assets/player_front.png");
    playerTextureLeft.loadFromFile("assets/player_left.png");
    playerTextureRight.loadFromFile("assets/player_right.png");

    player.setTextures(
        &playerTextureUp,
        &playerTextureDown,
        &playerTextureLeft,
        &playerTextureRight
    );

    map.setTexture(mapTexture);
    map.loadBuildingsFromJson("data/hitbox.json");

    // setup player
    player.sprite.setTexture(playerTexture);
    player.sprite.setScale(0.15f, 0.15f);   // 變成 30% 大小
    player.worldPos = {2000.f, 2000.f};
    sf::FloatRect bounds = player.sprite.getLocalBounds();
    player.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    player.hitbox = {{
        {0.f, 0.f},
        {bounds.width, 0.f},
        {bounds.width, bounds.height},
        {0.f, bounds.height}
    }};


    // camera
    view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(player.worldPos);
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

    timerText.setString("00:00");       // 初始顯示
    setupHomeUI();
    changeScreen(ScreenState::Home);

    missionText.setFont(uiFont);
    missionText.setCharacterSize(24);
    missionText.setFillColor(sf::Color::White);
    missionText.setOutlineColor(sf::Color::Black);
    missionText.setOutlineThickness(2.f);
    missionText.setPosition(10.f, 40.f);

    winText.setFont(uiFont);
    winText.setCharacterSize(48);
    winText.setFillColor(sf::Color::Yellow);
    winText.setOutlineColor(sf::Color::Black);
    winText.setOutlineThickness(3.f);

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
        if (event.type == sf::Event::Resized) {
            sf::Vector2u newSize(event.size.width, event.size.height);
            view.setSize(static_cast<float>(newSize.x), static_cast<float>(newSize.y));
            updateHomeLayout(newSize);
        }

        switch (screen) {
            case ScreenState::Home:
                handleHomeEvent(event);
                break;
            case ScreenState::Playing:
                handlePlayingEvent(event);
                break;
            case ScreenState::Win:
                handleWinEvent(event);
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
        case ScreenState::Win:
            updateWin(dt);
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
        case ScreenState::Win:
            renderWin();
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
    startPromptText.setString(utf8(u8"按 Enter / Space 開始遊戲，Esc 返回首頁"));
    startPromptText.setCharacterSize(24);
    startPromptText.setFillColor(sf::Color(180, 200, 255));

    updateHomeLayout(window.getSize());
}

void Game::handleHomeEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyReleased &&
        (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space)) {
        changeScreen(ScreenState::Playing);
    }
}

void Game::handlePlayingEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyReleased &&
        event.key.code == sf::Keyboard::Escape) {
        changeScreen(ScreenState::Home);
    }
}

void Game::handleWinEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyReleased &&
        (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Space)) {
        changeScreen(ScreenState::Home);
    }
}

void Game::updateHomeLayout(sf::Vector2u size) {
    sf::Vector2f fSize(static_cast<float>(size.x), static_cast<float>(size.y));
    homeBackground.setSize(fSize);

    auto centerText = [](sf::Text& text, float y) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
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
        case ScreenState::Win:
            onEnterWin();
            break;
    }
}

void Game::onEnterPlaying() {
    chooseStartAndDestination();
    if (!map.buildings.empty()) {
        player.worldPos = buildingCenter(startBuilding);
    } else {
        player.worldPos = {2000.f, 2000.f};
    }
    view.setCenter(player.worldPos);
    map.mapSprite.setPosition(0.f, 0.f);
    gameClock.restart();
    // convert to char* and use utf8()
    std::string missionStr = "Start: " + startBuilding.name + " | Destination: " + destinationBuilding.name;
    const char* missionCStr = missionStr.c_str();
    missionText.setString(utf8(missionCStr));
}

void Game::onEnterHome() {
    window.setView(window.getDefaultView());
}

void Game::onEnterWin() {
    window.setView(window.getDefaultView());
    winText.setString(utf8(("You reached " + destinationBuilding.name + "! Press Enter/Esc.").c_str()));
}

void Game::updateHome(sf::Time) {
    // No-op for now; placeholder for future home animations or timers.
}

void Game::updatePlaying(sf::Time dt) {
    float dtSec = dt.asSeconds();

    player.handleInput(dtSec);
    player.setPosition(player.worldPos);

    // update view center to player's world position
    view.setCenter(player.worldPos);

    map.update(dtSec);
    // check destination collision
    if (map.buildings.size() >= 2) {
        destinationBuilding.sprite.setPosition(0.f, 0.f); // keep world-aligned hitbox
        if (player.collidesWith(destinationBuilding)) {
            changeScreen(ScreenState::Win);
            return;
        }
    }
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
    drawBuildingOutlines(window);
    drawBuildingMarkers(window);
    player.setPosition(view.getCenter());
    player.draw(window);
    
    // ==== 2. 畫 UI（固定螢幕座標）====
    window.setView(window.getDefaultView());
    window.draw(timerText);
    window.draw(missionText);
}

void Game::updateWin(sf::Time) {
    // idle state; nothing to update yet
}

void Game::renderWin() {
    window.setView(window.getDefaultView());
    window.clear(sf::Color(18, 26, 60));
    window.draw(winText);
}

void Game::chooseStartAndDestination() {
    if (map.buildings.size() < 2) {
        startBuilding = Building("Unknown");
        destinationBuilding = Building("Unknown");
        return;
    }
    size_t a = static_cast<size_t>(rand() % map.buildings.size());
    size_t b = a;
    while (b == a) {
        b = static_cast<size_t>(rand() % map.buildings.size());
    }
    startBuilding = map.buildings[a];
    destinationBuilding = map.buildings[b];
}

sf::Vector2f Game::buildingCenter(const Building& b) const {
    sf::Vector2f sum{0.f, 0.f};
    for (const auto& p : b.hitbox) {
        sum += p;
    }
    sum.x /= 4.f;
    sum.y /= 4.f;
    return sum;
}

void Game::drawBuildingMarkers(sf::RenderTarget& target) const {
    if (map.buildings.empty()) return;
    sf::CircleShape marker(8.f);
    marker.setOrigin(8.f, 8.f);
    for (const auto& b : map.buildings) {
        sf::Color color(80, 180, 255);
        if (b.name == startBuilding.name) color = sf::Color(50, 200, 120);
        if (b.name == destinationBuilding.name) color = sf::Color(220, 80, 80);
        marker.setFillColor(color);
        marker.setPosition(buildingCenter(b));
        target.draw(marker);
    }
}

void Game::drawBuildingOutlines(sf::RenderTarget& target) const {
    if (map.buildings.empty()) return;
    sf::ConvexShape poly;
    poly.setPointCount(4);
    poly.setOutlineThickness(3.f);
    for (const auto& b : map.buildings) {
        sf::Color color(80, 180, 255, 200);
        if (b.name == startBuilding.name) color = sf::Color(50, 200, 120, 220);
        if (b.name == destinationBuilding.name) color = sf::Color(220, 80, 80, 220);
        for (size_t i = 0; i < 4; ++i) {
            poly.setPoint(i, b.hitbox[i]);
        }
        poly.setOutlineColor(color);
        poly.setFillColor(sf::Color(color.r, color.g, color.b, 40));
        target.draw(poly);
    }
}
