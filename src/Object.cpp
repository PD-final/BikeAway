#include "Object.h"
#include "Utils.h"
#include <cmath>
#include <array>
#include <iostream>

bool Object::on_road(Road &road) {
  sf::Vector2f p = getPosition();
  double dist=0;
  if (dot(road.end - road.start, p - road.start) >= 0 &&
      dot(road.start - road.end, p - road.end) >= 0)
    dist = (cross(road.end - road.start, p - road.start) /
                abs(road.start - road.end));
  else{
    dist = std::min(abs(p - road.start), abs(p - road.end));
  }
  return std::abs(dist) <= road.width ;
}


std::array<sf::Vector2f, 4> Object::getTransformedHitbox() const {
    std::array<sf::Vector2f, 4> transformed;
    sf::Transform t = sprite.getTransform();
    for (size_t i = 0; i < 4; ++i) {
        transformed[i] = t.transformPoint(hitbox[i]);
    }
    return transformed;
}

bool Object::collidesWith(const Object& other) const {
    auto box1 = getTransformedHitbox();
    auto box2 = other.getTransformedHitbox();

    // SAT
    auto getAxes = [](const std::array<sf::Vector2f, 4>& box) {
        return std::array<sf::Vector2f, 2>{
            sf::Vector2f(box[1].y - box[0].y, box[0].x - box[1].x),
            sf::Vector2f(box[3].y - box[0].y, box[0].x - box[3].x)
        };
    };
    auto project = [](const std::array<sf::Vector2f, 4>& box, const sf::Vector2f& axis) {
        float min = dot(box[0], axis);
        float max = min;
        for (size_t i = 1; i < 4; ++i) {
            float proj = dot(box[i], axis);
            if (proj < min) min = proj;
            if (proj > max) max = proj;
        }
        return std::make_pair(min, max);
    };
    auto axes1 = getAxes(box1);
    auto axes2 = getAxes(box2);
    for (const auto& axis : {axes1[0], axes1[1], axes2[0], axes2[1]}) {
        auto [min1, max1] = project(box1, axis);
        auto [min2, max2] = project(box2, axis);
        if (max1 < min2 || max2 < min1) {
            return false; // Found a separating axis
        }
    }
    return true; // No separating axis found, boxes collide
} 