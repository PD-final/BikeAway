#include "Obstacle.h"

#include <algorithm>  // For std::reverse
#include <cmath>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

Obstacle::Obstacle() : m_gen(std::random_device{}()) {
    // The body of your constructor
    // sprite.setTexture(...);
}

std::vector<sf::Vector2i> getPossibleMoves(sf::Image* img_thin,
                                           int SEARCH_RADIUS, int posx,
                                           int posy) {
    sf::Vector2u imgSize = img_thin->getSize();
    int width = static_cast<int>(imgSize.x);
    int height = static_cast<int>(imgSize.y);

    std::vector<sf::Vector2i> candidates;

    // Helper lambda
    auto isWhite = [&](int x, int y) -> bool {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return img_thin->getPixel(x, y) == sf::Color::White;
        }
        return false;
    };

    // 1. Top Edge
    for (int x = posx - SEARCH_RADIUS; x <= posx + SEARCH_RADIUS; ++x) {
        int y = posy - SEARCH_RADIUS;
        if (isWhite(x, y)) candidates.push_back(sf::Vector2i(x, y));
    }

    // 2. Right Edge
    for (int y = posy - SEARCH_RADIUS + 1; y <= posy + SEARCH_RADIUS; ++y) {
        int x = posx + SEARCH_RADIUS;
        if (isWhite(x, y)) candidates.push_back(sf::Vector2i(x, y));
    }

    // 3. Bottom Edge
    for (int x = posx + SEARCH_RADIUS - 1; x >= posx - SEARCH_RADIUS; --x) {
        int y = posy + SEARCH_RADIUS;
        if (isWhite(x, y)) candidates.push_back(sf::Vector2i(x, y));
    }

    // 4. Left Edge
    for (int y = posy + SEARCH_RADIUS - 1; y > posy - SEARCH_RADIUS; --y) {
        int x = posx - SEARCH_RADIUS;
        if (isWhite(x, y)) candidates.push_back(sf::Vector2i(x, y));
    }

    return candidates;
}

sf::Vector2i Obstacle::findRandomWhite(sf::Image* img_thin, int SEARCH_RADIUS,
                                       int posx, int posy) {
    std::vector<sf::Vector2i> candidates =
        getPossibleMoves(img_thin, SEARCH_RADIUS, posx, posy);

    if (!candidates.empty()) {
        std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
        return candidates[dist(m_gen)];
    } else {
        return sf::Vector2i(posx, posy);
    }
}

void Obstacle::update(sf::Image* img_thin, float mapScale) {
    sf::Vector2i prevPixel = static_cast<sf::Vector2i>(currentPos / mapScale);

    currentPos = sprite.getPosition();
    sf::Vector2i startNode = static_cast<sf::Vector2i>(currentPos / mapScale);

    // Clamp startNode to be inside image bounds ---
    int w = img_thin->getSize().x;
    int h = img_thin->getSize().y;

    // Ensure x is between 0 and w-1
    if (startNode.x < 0) startNode.x = 0;
    if (startNode.x >= w) startNode.x = w - 1;

    // Ensure y is between 0 and h-1
    if (startNode.y < 0) startNode.y = 0;
    if (startNode.y >= h) startNode.y = h - 1;

    sf::Vector2i nextPixel;
    std::uniform_int_distribution<size_t> dist(0, 99);

    if (dist(m_gen) > 0) {
        std::vector<sf::Vector2i> candidates =
            getPossibleMoves(img_thin, 5, startNode.x, startNode.y);

        // set nextPixel to the pixel in candidates that is farthest from
        // prevPixel
        if (!candidates.empty()) {
            float maxDistSq = -1.0f;
            sf::Vector2i bestMove = candidates[0];

            for (const auto& cand : candidates) {
                // Calculate Euclidean distance squared (faster than sqrt)
                float dx = static_cast<float>(cand.x - prevPixel.x);
                float dy = static_cast<float>(cand.y - prevPixel.y);
                float distSq = dx * dx + dy * dy;

                if (distSq > maxDistSq) {
                    maxDistSq = distSq;
                    bestMove = cand;
                }
            }
            nextPixel = bestMove;
        } else {
            // No moves found, stay put
            nextPixel = startNode;
        }
    } else {
        nextPixel = findRandomWhite(img_thin, 5, startNode.x, startNode.y);
    }

    // Convert to World Coordinates
    sprite.setPosition(static_cast<float>(nextPixel.x) * mapScale,
                       static_cast<float>(nextPixel.y) * mapScale);
}