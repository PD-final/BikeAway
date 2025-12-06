#include "thinMap.h"

#include <iostream>

// #include <opencv2/opencv.hpp>

struct Point {
    int x;
    int y;
};

class ZhangSuenThinning {
   private:
    int width;
    int height;
    std::vector<std::vector<int>> grid;

    // Get pixel value safely (return 0 if out of bounds)
    int get(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return 0;
        }
        return grid[y][x];
    }

    /**
     * Calculates the number of 0->1 transitions in the ordered sequence of
     * neighbors: P2, P3, P4, P5, P6, P7, P8, P9, P2
     *
     * Neighborhood mapping:
     * P9 P2 P3
     * P8 P1 P4
     * P7 P6 P5
     */
    int connectivity(int x, int y) const {
        // Neighbors in clockwise order starting from Top (P2)
        int p2 = get(x, y - 1);
        int p3 = get(x + 1, y - 1);
        int p4 = get(x + 1, y);
        int p5 = get(x + 1, y + 1);
        int p6 = get(x, y + 1);
        int p7 = get(x - 1, y + 1);
        int p8 = get(x - 1, y);
        int p9 = get(x - 1, y - 1);

        int transitions = 0;
        if (p2 == 0 && p3 == 1) transitions++;
        if (p3 == 0 && p4 == 1) transitions++;
        if (p4 == 0 && p5 == 1) transitions++;
        if (p5 == 0 && p6 == 1) transitions++;
        if (p6 == 0 && p7 == 1) transitions++;
        if (p7 == 0 && p8 == 1) transitions++;
        if (p8 == 0 && p9 == 1) transitions++;
        if (p9 == 0 && p2 == 1) transitions++;

        return transitions;
    }

    // Count non-zero neighbors
    int numNeighbors(int x, int y) const {
        return get(x, y - 1) + get(x + 1, y - 1) + get(x + 1, y) +
               get(x + 1, y + 1) + get(x, y + 1) + get(x - 1, y + 1) +
               get(x - 1, y) + get(x - 1, y - 1);
    }

   public:
    void apply(sf::Image& image) {
        sf::Vector2u size = image.getSize();
        width = static_cast<int>(size.x);
        height = static_cast<int>(size.y);

        // 1. Initialize binary grid from SFML Image
        // We assume White (or non-black) is foreground (1), Black is background
        // (0)
        grid.assign(height, std::vector<int>(width));
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                sf::Color c = image.getPixel(x, y);
                // Simple threshold: if R channel > 127, it's a 1.
                grid[y][x] = (c.r > 127) ? 1 : 0;
            }
        }

        bool isModified = true;
        std::vector<Point> pixelsToDelete;

        // 2. Main Loop
        while (isModified) {
            isModified = false;

            // --- Iteration 1 ---
            pixelsToDelete.clear();
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    if (grid[y][x] == 0) continue;

                    int A = connectivity(x, y);
                    int B = numNeighbors(x, y);

                    int p2 = get(x, y - 1);
                    int p4 = get(x + 1, y);
                    int p6 = get(x, y + 1);
                    int p8 = get(x - 1, y);

                    // Condition 1: 2 <= B(P1) <= 6
                    // Condition 2: A(P1) == 1
                    // Condition 3: P2 * P4 * P6 == 0
                    // Condition 4: P4 * P6 * P8 == 0
                    if (B >= 2 && B <= 6 && A == 1 && (p2 * p4 * p6 == 0) &&
                        (p4 * p6 * p8 == 0)) {
                        pixelsToDelete.push_back({x, y});
                    }
                }
            }

            if (!pixelsToDelete.empty()) {
                isModified = true;
                for (const auto& p : pixelsToDelete) {
                    grid[p.y][p.x] = 0;
                }
            }

            // --- Iteration 2 ---
            pixelsToDelete.clear();
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    if (grid[y][x] == 0) continue;

                    int A = connectivity(x, y);
                    int B = numNeighbors(x, y);

                    int p2 = get(x, y - 1);
                    int p4 = get(x + 1, y);
                    int p6 = get(x, y + 1);
                    int p8 = get(x - 1, y);

                    // Condition 1: 2 <= B(P1) <= 6
                    // Condition 2: A(P1) == 1
                    // Condition 3: P2 * P4 * P8 == 0  <-- Changed from
                    // Iteration 1 Condition 4: P2 * P6 * P8 == 0  <-- Changed
                    // from Iteration 1
                    if (B >= 2 && B <= 6 && A == 1 && (p2 * p4 * p8 == 0) &&
                        (p2 * p6 * p8 == 0)) {
                        pixelsToDelete.push_back({x, y});
                    }
                }
            }

            if (!pixelsToDelete.empty()) {
                isModified = true;
                for (const auto& p : pixelsToDelete) {
                    grid[p.y][p.x] = 0;
                }
            }
        }

        // 3. Write back to SFML Image
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (grid[y][x] == 1) {
                    image.setPixel(x, y, sf::Color::White);
                } else {
                    image.setPixel(x, y, sf::Color::Black);
                }
            }
        }
    }
};

// Helper to calculate perceived brightness
// Uses standard NTSC coefficients: 0.299R + 0.587G + 0.114B
int getLuminance(const sf::Color& c) {
    return static_cast<int>(0.299 * c.r + 0.587 * c.g + 0.114 * c.b);
}

void preprocessImage(sf::Image& img, int threshold = 128) {
    sf::Vector2u size = img.getSize();

    for (unsigned int y = 0; y < size.y; ++y) {
        for (unsigned int x = 0; x < size.x; ++x) {
            sf::Color c = img.getPixel(x, y);

            // 1. Check Transparency
            // If alpha is low (e.g., < 128), treat it as background (Black)
            if (c.a < 128) {
                img.setPixel(x, y, sf::Color::Black);
                continue;
            }

            // 2. Check Brightness
            int luminance = getLuminance(c);

            if (luminance < threshold) {
                // DARK pixel -> Make it WHITE (Foreground for thinning)
                img.setPixel(x, y, sf::Color::White);
            } else {
                // LIGHT pixel -> Make it BLACK (Background)
                img.setPixel(x, y, sf::Color::Black);
            }
        }
    }
}

void thinMap::create(std::string filename) {
    sf::Image img;
    if (!img.loadFromFile(filename)) {
        std::cerr << "Error: Could not load image." << std::endl;
        return;
    }

    // Convert dark ink to white pixels, remove background
    preprocessImage(img);
    // sf::Image preprocessed = img;

    // Apply thinning
    ZhangSuenThinning thinner;
    thinner.apply(img);
    thinImage = img;
    img.saveToFile("ntu_map_thin.png");

    return;
}