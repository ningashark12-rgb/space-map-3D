#include "core/Star.h"

#include <utility>

Star::Star(std::string name, glm::vec3 position, float mass, StarType type)
    : name_(std::move(name)),
      position_(position),
      mass_(mass),
      type_(type) {}

glm::vec3 Star::getPosition() const {
    return position_;
}

std::string Star::getName() const {
    return name_;
}

float Star::getMass() const {
    return mass_;
}

StarType Star::getType() const {
    return type_;
}
