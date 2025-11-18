#include "Game.h"

Game::Game()
: window(sf::VideoMode(1280, 720), "NTU Rush")
{
    window.setFramerateLimit(60);

    mapTexture.loadFromFile("assets/ntu_map.png");
    playerTexture.loadFromFile("assets/player.png");
    bikeTexture.loadFromFile("assets/bike.png");

    map.setTexture(mapTexture);

    // setup player
    player.sprite.setTexture(playerTexture);
    player.sprite.setScale(0.5f, 0.5f);   // 變成 50% 大小
    player.worldPos = {1000.f, 1000.f};
    sf::FloatRect bounds = player.sprite.getLocalBounds();
    player.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);


    // camera
    view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(player.worldPos);

    // sample obstacle
    Obstacle bike;
    bike.sprite.setTexture(bikeTexture);
    bike.type = ObstacleType::Bike;
    bike.setPosition({1000.f, 1000.f});
    bike.velocity = {10.f, 0.f};

    map.obstacles.push_back(bike);
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
}

void Game::render() {
    window.clear();
    window.setView(view);

    map.draw(window);
    player.setPosition(view.getCenter());
    player.draw(window);

    window.display();
}
