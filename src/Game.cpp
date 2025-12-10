#include "Game.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <algorithm>

namespace {
sf::String utf8(const char* text) {
    auto len = std::strlen(text);
    return sf::String::fromUtf8(text, text + len);
}
}
Game::Game()
: window(sf::VideoMode(1920, 1080), "BikeAway")
{
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    mapTexture.loadFromFile("assets/ntu_map.png");
    mapOverlayTexture.loadFromFile("assets/ntu_map_buildings.png");
    playerTexture.loadFromFile("assets/player_front.png");
    bikeTexture.loadFromFile("assets/bike.png");
    bikeTextureFront.loadFromFile("assets/bike_front.png");
    bikeTextureLeft.loadFromFile("assets/bike_left.png");
    bikeTextureRight.loadFromFile("assets/bike_right.png");
    bikeTextureBack.loadFromFile("assets/bike_back.png");
    heartTexture.loadFromFile("assets/heart.png");
    entryTexture.loadFromFile("assets/entry.png");
    failTexture.loadFromFile("assets/fail.png");
    successTexture.loadFromFile("assets/success.png");
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
    map.setOverlayTexture(mapOverlayTexture);
    map.loadBuildingsFromJson("data/hitbox.json");
    map.loadRoadsFromJson("data/roads.json");
    spawnBikesOnRoads();

    // setup player
    player.sprite.setTexture(playerTexture);
    player.sprite.setScale(0.2f, 0.2f);   // 變成 30% 大小
    player.worldPos = {2000.f, 2000.f};
    sf::FloatRect bounds = player.sprite.getLocalBounds();
    player.sprite.setOrigin(bounds.width / 2.f, bounds.height);
    setBottomHitbox(player, 0.33f);


    // camera
    view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(player.worldPos);
    // ====== 計時器 UI 初始化 ======
    if (!uiFont.loadFromFile("assets/NotoSansTC-Regular.ttf")) {
        std::cerr << "Failed to load font: assets/NotoSansTC-Regular.ttf\n";
    }

    timerText.setFont(uiFont);
    timerText.setCharacterSize(40);
    timerText.setFillColor(sf::Color::White);
    timerText.setOutlineColor(sf::Color::Black);
    timerText.setOutlineThickness(2.f);
    timerText.setPosition(10.f, 10.f);  // 左上角

    timerText.setString("00:00");       // 初始顯示
    setupHomeUI();
    changeScreen(ScreenState::Home);

    missionText.setFont(uiFont);
    missionText.setCharacterSize(40);
    missionText.setFillColor(sf::Color::White);
    missionText.setOutlineColor(sf::Color::Black);
    missionText.setOutlineThickness(2.f);
    missionText.setPosition(10.f, 60.f);

    winText.setFont(uiFont);
    winText.setCharacterSize(48);
    winText.setFillColor(sf::Color::Yellow);
    winText.setOutlineColor(sf::Color::Black);
    winText.setOutlineThickness(3.f);

    failText.setFont(uiFont);
    failText.setCharacterSize(48);
    failText.setFillColor(sf::Color::Red);
    failText.setOutlineColor(sf::Color::Black);
    failText.setOutlineThickness(3.f);

    invincibleText.setFont(uiFont);
    invincibleText.setCharacterSize(24);
    invincibleText.setFillColor(sf::Color::Yellow);
    invincibleText.setOutlineColor(sf::Color::Black);
    invincibleText.setOutlineThickness(2.f);
    invincibleText.setString(utf8(u8"無敵中"));

    reachedText.setFont(uiFont);
    reachedText.setCharacterSize(40);
    reachedText.setFillColor(sf::Color::White);
    reachedText.setOutlineColor(sf::Color::Black);
    reachedText.setOutlineThickness(2.f);

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
            case ScreenState::Fail:
                handleFailEvent(event);
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
        case ScreenState::Fail:
            updateFail(dt);
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
        case ScreenState::Fail:
            renderFail();
            break;
    }

    window.display();
}

void Game::setupHomeUI() {

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

void Game::handleFailEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyReleased &&
        (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Space)) {
        changeScreen(ScreenState::Home);
    }
}

void Game::updateHomeLayout(sf::Vector2u size) {
    sf::Vector2f fSize(static_cast<float>(size.x), static_cast<float>(size.y));
    if (entryTexture.getSize().x > 0 && entryTexture.getSize().y > 0) {
        float scaleX = fSize.x / static_cast<float>(entryTexture.getSize().x);
        float scaleY = fSize.y / static_cast<float>(entryTexture.getSize().y);
        float scale = std::max(scaleX, scaleY);
        entrySprite.setTexture(entryTexture);
        entrySprite.setScale(scale, scale);
        float posX = (fSize.x - entryTexture.getSize().x * scale) * 0.5f;
        float posY = (fSize.y - entryTexture.getSize().y * scale) * 0.5f;
        entrySprite.setPosition(posX, posY);
    }

    sf::FloatRect promptBounds = startPromptText.getLocalBounds();
    startPromptText.setOrigin(promptBounds.left + promptBounds.width / 2.f, promptBounds.top + promptBounds.height / 2.f);
    startPromptText.setPosition(fSize.x / 2.f, fSize.y * 0.9f);

    if (failTexture.getSize().x > 0 && failTexture.getSize().y > 0) {
        float fScaleX = fSize.x / static_cast<float>(failTexture.getSize().x);
        float fScaleY = fSize.y / static_cast<float>(failTexture.getSize().y);
        float fScale = std::max(fScaleX, fScaleY);
        failSprite.setTexture(failTexture);
        failSprite.setScale(fScale, fScale);
        float fPosX = (fSize.x - failTexture.getSize().x * fScale) * 0.5f;
        float fPosY = (fSize.y - failTexture.getSize().y * fScale) * 0.5f;
        failSprite.setPosition(fPosX, fPosY);
    }
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
        case ScreenState::Fail:
            onEnterFail();
            break;
    }
}

void Game::onEnterPlaying() {
    chooseDestination();
    player.worldPos = pickRandomSpawn();
    view.setCenter(player.worldPos);
    map.mapSprite.setPosition(0.f, 0.f);
    gameClock.restart();
    spawnInvincibleSeconds = 5.f;
    heartInvincibleSeconds = 0.f;
    player.hearts = 3; // initial hearts
    hasReachedDestination = false;
    // convert to char* and use utf8()
    std::string missionStr = "Destination: " + destinationBuilding.name;
    const char* missionCStr = missionStr.c_str();
    missionText.setString(utf8(missionCStr));
}

void Game::onEnterHome() {
    window.setView(window.getDefaultView());
}

void Game::onEnterWin() {
    window.setView(window.getDefaultView());
    // Prepare success sprite for current window size
    sf::Vector2f fSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    if (successTexture.getSize().x > 0 && successTexture.getSize().y > 0) {
        float sScaleX = fSize.x / static_cast<float>(successTexture.getSize().x);
        float sScaleY = fSize.y / static_cast<float>(successTexture.getSize().y);
        float sScale = std::max(sScaleX, sScaleY);
        successSprite.setTexture(successTexture);
        successSprite.setScale(sScale, sScale);
        float sPosX = (fSize.x - successTexture.getSize().x * sScale) * 0.5f;
        float sPosY = (fSize.y - successTexture.getSize().y * sScale) * 0.5f;
        successSprite.setPosition(sPosX, sPosY);
    }
    hasReachedDestination = true;
    std::string reachedStr = "你已抵達 " + destinationBuilding.name;
    reachedText.setString(utf8(reachedStr.c_str()));
    sf::FloatRect reachBounds = reachedText.getLocalBounds();
    reachedText.setOrigin(reachBounds.left + reachBounds.width, reachBounds.top);
    reachedText.setPosition(fSize.x - 10.f, 10.f);
}

void Game::onEnterFail() {
    window.setView(window.getDefaultView());
    sf::Vector2f fSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    if (failTexture.getSize().x > 0 && failTexture.getSize().y > 0) {
        float fScaleX = fSize.x / static_cast<float>(failTexture.getSize().x);
        float fScaleY = fSize.y / static_cast<float>(failTexture.getSize().y);
        float fScale = std::max(fScaleX, fScaleY);
        failSprite.setTexture(failTexture);
        failSprite.setScale(fScale, fScale);
        float fPosX = (fSize.x - failTexture.getSize().x * fScale) * 0.5f;
        float fPosY = (fSize.y - failTexture.getSize().y * fScale) * 0.5f;
        failSprite.setPosition(fPosX, fPosY);
    }
}

void Game::updateHome(sf::Time) {
    // No-op for now; placeholder for future home animations or timers.
}

void Game::updatePlaying(sf::Time dt) {
    float dtSec = dt.asSeconds();
    spawnInvincibleSeconds = std::max(0.f, spawnInvincibleSeconds - dtSec);
    heartInvincibleSeconds = std::max(0.f, heartInvincibleSeconds - dtSec);
    hitFlashSeconds = std::max(0.f, hitFlashSeconds - dtSec);

    sf::Vector2f prevPos = player.worldPos;
    player.handleInput(dtSec);
    player.setPosition(player.worldPos);

    // prevent leaving roads
    if (!playerOnAnyRoad()) {
        player.worldPos = prevPos;
        player.setPosition(player.worldPos);
    }

    // update view center to player's world position
    view.setCenter(player.worldPos);

    map.update(dtSec);
    // check collision with bikes
    if (spawnInvincibleSeconds <= 0.f && heartInvincibleSeconds <= 0.f) {
        for (const auto& obs : map.obstacles) {
            if (obs.type == ObstacleType::Bike && obs.spawnGraceSeconds <= 0.f && player.collidesWith(obs)) {
                hitFlashSeconds = 0.3f;
                if (player.hearts > 0) {
                    player.hearts -= 1;
                    if (player.hearts <= 0) {
                        changeScreen(ScreenState::Fail);
                        return;
                    }
                    heartInvincibleSeconds = 3.f;
                } else {
                    changeScreen(ScreenState::Fail);
                    return;
                }
            }
        }
    }
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

    // update UI positions
    sf::Vector2f viewSize(window.getDefaultView().getSize());

    // invincible indicator top-center
    if (spawnInvincibleSeconds > 0.f || heartInvincibleSeconds > 0.f) {
        sf::FloatRect invBounds = invincibleText.getLocalBounds();
        invincibleText.setOrigin(invBounds.left + invBounds.width / 2.f, invBounds.top);
        invincibleText.setPosition(viewSize.x / 2.f, 10.f);
    }

    // destination text top-right (only after reached)
    if (hasReachedDestination) {
        std::string reachedStr = "你已抵達 " + destinationBuilding.name;
        reachedText.setString(utf8(reachedStr.c_str()));
        sf::FloatRect reachBounds = reachedText.getLocalBounds();
        reachedText.setOrigin(reachBounds.left + reachBounds.width, reachBounds.top);
        reachedText.setPosition(viewSize.x - 10.f, 10.f);
    }
}

void Game::renderHome() {
    window.setView(window.getDefaultView());
    window.draw(entrySprite);
    window.draw(startPromptText);
}

void Game::renderPlaying() {
    window.setView(view);

    map.draw(window);
    player.setPosition(view.getCenter());
    player.draw(window);
    map.drawOverlay(window); // overlay on top of bikes/player to hide them

    // ==== 2. 畫 UI（固定螢幕座標）====
    window.setView(window.getDefaultView());
    window.draw(timerText);
    window.draw(missionText);
    // draw hearts (top-right)
    if (heartTexture.getSize().x > 0 && heartTexture.getSize().y > 0) {
        sf::Sprite heart(heartTexture);
        float scale = 1.f;
        heart.setScale(scale, scale);
        sf::FloatRect hb = heart.getLocalBounds();
        heart.setOrigin(hb.width / 2.f, hb.height / 2.f);
        sf::Vector2f viewSize(window.getDefaultView().getSize());
        float spacing = hb.width * scale + 6.f;
        float startX = viewSize.x - (hb.width * scale) * 0.5f - 10.f;
        float y = 10.f + (hb.height * scale) * 0.5f;
        for (int i = 0; i < player.hearts; ++i) {
            heart.setPosition(startX - i * spacing, y);
            window.draw(heart);
        }
    }
    if (hasReachedDestination) {
        window.draw(reachedText);
    }
    if (spawnInvincibleSeconds > 0.f || heartInvincibleSeconds > 0.f) {
        window.draw(invincibleText);
    }
    if (hitFlashSeconds > 0.f) {
        float ratio = hitFlashSeconds / 0.3f;
        sf::RectangleShape flash(window.getView().getSize());
        flash.setFillColor(sf::Color(255, 0, 0, static_cast<sf::Uint8>(150 * ratio)));
        flash.setPosition(0.f, 0.f);
        window.draw(flash);
    }
}

void Game::updateWin(sf::Time) {
    // idle state; nothing to update yet
}

void Game::updateFail(sf::Time) {
    // idle
}

void Game::renderWin() {
    window.setView(window.getDefaultView());
    window.clear(sf::Color::Black);
    window.draw(successSprite);
    window.draw(reachedText);
}

void Game::renderFail() {
    window.setView(window.getDefaultView());
    window.clear(sf::Color::Black);
    window.draw(failSprite);
}

void Game::spawnBikesOnRoads() {
    unsigned int seed = static_cast<unsigned int>(time(nullptr));
    srand(seed);
    map.obstacles.clear();
    for (size_t i = 0; i < map.roads.size(); ++i) {
        const Road& r = map.roads[i];
        sf::Vector2f dir = r.end - r.start;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len < 1.f) continue;
        sf::Vector2f u = dir / len;
        sf::Vector2f n{-u.y, u.x};

        int count = std::max(2, static_cast<int>(len / 300.f)); // more bikes on longer roads
        for (int k = 0; k < count; ++k) {
            Obstacle bike;
            bike.type = ObstacleType::Bike;
            bike.road = &r;
            bike.directionSign = (rand() % 2 == 0) ? 1 : -1;
            bike.speedAlong = 100.f + static_cast<float>(rand() % 101); // 150-250
            float maxOffset = static_cast<float>(r.width) * 0.7f;
            bike.lateralOffset = maxOffset == 0.f ? 0.f : (static_cast<float>(rand()) / RAND_MAX * 2.f - 1.f) * maxOffset;
            float startDist = (static_cast<float>(rand()) / RAND_MAX) * len;
            bike.distanceAlong = startDist;
            sf::Vector2f pos = ((bike.directionSign >= 0) ? r.start : r.end) + u * startDist + n * bike.lateralOffset;
            bike.spawnGraceSeconds = 5.f;
            bike.sprite.setTexture(bikeTextureFront);
            bike.sprite.setScale(0.25f, 0.25f);
            sf::FloatRect b = bike.sprite.getLocalBounds();
            bike.sprite.setOrigin(b.width / 2.f, b.height );
            bike.sprite.setPosition(pos);
            sf::Vector2f heading = u * static_cast<float>(bike.directionSign);
            bike.spriteBaseAngle = setBikeTextureForDirection(bike, heading);
            float headingDeg = std::atan2(heading.y, heading.x) * 180.f / 3.14159265f;
            bike.sprite.setRotation(headingDeg - bike.spriteBaseAngle);
            setBottomHitbox(bike, 0.33f);
            map.obstacles.push_back(bike);
        }
    }
}

float Game::setBikeTextureForDirection(Obstacle& bike, const sf::Vector2f& dir) {
    const sf::Texture* tex = &bikeTextureFront;
    float ax = std::abs(dir.x);
    float ay = std::abs(dir.y);
    float baseAngle = 90.f; // front/down baseline
    if (ax >= ay) {
        if (dir.x >= 0) {
            tex = &bikeTextureRight;
            baseAngle = 0.f;
        } else {
            tex = &bikeTextureLeft;
            baseAngle = 180.f;
        }
    } else {
        if (dir.y < 0) {
            tex = &bikeTextureBack;
            baseAngle = -90.f; // back faces up (-Y)
        } else {
            tex = &bikeTextureFront; // down-facing baseline
            baseAngle = 90.f;
        }
    }
    bike.sprite.setTexture(*tex, true); // reset texture rect to avoid cropping
    sf::FloatRect bounds = bike.sprite.getLocalBounds();
    bike.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    return baseAngle;
}

void Game::setBottomHitbox(Object& obj, float fraction) {
    sf::FloatRect b = obj.sprite.getLocalBounds();
    float top = b.height * (1.f - fraction);
    obj.hitbox = {{
        {0.f, top},
        {b.width, top},
        {b.width, b.height},
        {0.f, b.height}
    }};
}

void Game::chooseDestination() {
    if (map.buildings.empty()) {
        destinationBuilding = Building("Unknown");
        return;
    }
    size_t idx = static_cast<size_t>(rand() % map.buildings.size());
    destinationBuilding = map.buildings[idx];
}

sf::Vector2f Game::pickRandomSpawn() const {
    if (!map.roads.empty()) {
        const Road& r = map.roads[static_cast<size_t>(rand() % map.roads.size())];
        sf::Vector2f dir = r.end - r.start;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 1.f) {
            sf::Vector2f u = dir / len;
            sf::Vector2f n{-u.y, u.x};
            float t = static_cast<float>(rand()) / RAND_MAX * len;
            float offset = static_cast<float>(rand()) / RAND_MAX * r.width * 0.6f * ((rand()%2)==0?1.f:-1.f);
            return r.start + u * t + n * offset;
        }
    }
    // fallback to map center
    return {view.getCenter().x, view.getCenter().y};
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

bool Game::playerOnAnyRoad() {
    for (const auto& r : map.roads) {
        if (player.on_road(const_cast<Road&>(r))) { // on_road requires non-const
            return true;
        }
    }
    return map.roads.empty(); // if no roads, don't block movement
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
