#include "Object.h"
#include "Utils.h"
#include <cmath>

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
  return dist <= road.width ;
}
