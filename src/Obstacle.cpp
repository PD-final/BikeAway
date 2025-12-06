#include "Obstacle.h"

#include <cmath>
#include <iostream>
#include <limits>

sf::Vector2i Obstacle::nextWhite(sf::Image* img_thin, int SEARCH_RADIUS,
                                 int posx, int posy) {
    sf::Vector2u imgSize = img_thin->getSize();
    sf::Vector2i nextPixel(-1, -1);

    // Right side implies i >= x.
    int startX = std::max(0, posx);
    int endX = std::min(static_cast<int>(imgSize.x) - 1, posx + SEARCH_RADIUS);

    int startY = std::max(0, posy - SEARCH_RADIUS);
    int endY = std::min(static_cast<int>(imgSize.y) - 1, posy + SEARCH_RADIUS);

    // 1. Setup initial position at the center
    int x = posx;
    int y = posy;

    // 2. Setup direction vectors (Starting direction: Right)
    // dx, dy pairs: (1,0) Right, (0,1) Down, (-1,0) Left, (0,-1) Up
    int dx = 1;
    int dy = 0;

    // 3. Setup spiral constraints
    int segmentLength = 1;  // How many steps to take in current direction
    int stepsTaken = 0;     // Steps taken in current segment
    int turns = 0;          // How many times we've turned
    // Define a max area to prevent infinite loops (e.g., width * height or a
    // fixed limit)
    int maxIterations = (endX - startX) * (endY - startY);

    // Iterate
    for (int k = 0; k < maxIterations; ++k) {
        // 4. BOUNDARY CHECK: Ensure we don't read outside the image
        // (Crucial because a spiral can walk out of the bounding box)
        if (x >= 0 && x < img_thin->getSize().x && y >= 0 &&
            y < img_thin->getSize().y) {
            // 5. Check color
            if (img_thin->getPixel(x, y) == sf::Color::White) {
                nextPixel = sf::Vector2i(x, y);
                // IMPORTANT: Break immediately.
                // In a spiral search, the first match is geometrically the
                // closest.
                break;
            }
        }

        // 6. Move to next pixel
        x += dx;
        y += dy;
        stepsTaken++;

        // 7. Check if we need to turn
        if (stepsTaken == segmentLength) {
            stepsTaken = 0;

            // Rotate direction 90 degrees clockwise: (1,0) -> (0,1) -> (-1,0)
            // -> (0,-1)
            int temp = dx;
            dx = -dy;
            dy = temp;

            turns++;

            // Increase segment length every 2 turns (e.g., 1, 1, 2, 2, 3, 3...)
            if (turns % 2 == 0) {
                segmentLength++;
            }
        }
    }

    return nextPixel;
}

void Obstacle::update(sf::Image* img_thin, float mapScale) {
    // std::cout << img_thin->getSize().x << ", " << img_thin->getSize().y <<
    // std::endl; sprite.move(velocity * dt);
    sf::Vector2f pixel_pos = sprite.getPosition() / mapScale;
    int x = static_cast<int>(pixel_pos.x) + 2;
    int y = static_cast<int>(pixel_pos.y);

    sf::Vector2f pos =
        static_cast<sf::Vector2f>(nextWhite(img_thin, 10, x, y)) * mapScale;
    sprite.setPosition(pos);
}