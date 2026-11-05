#pragma once
#include <string>
#include <glm/glm.hpp>

enum class StarType { MainSequence, RedGiant, WhiteDwarf, NeutronStar };

class Star {
public:
    Star(std::string name, glm::vec3 position, float mass, StarType type)
        : name_(name), position_(position), mass_(mass), type_(type) {}

    glm::vec3 getPosition() const { return position_; }
    std::string getName() const { return name_; }
    float getMass() const { return mass_; }
    StarType getType() const { return type_; }

private:
    std::string name_;
    glm::vec3 position_;
    float mass_;
    StarType type_;
};