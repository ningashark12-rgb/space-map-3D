#pragma once

#include "CelestialObject.h"

#include <glm/glm.hpp>
#include <string>

enum class StarType {
    MainSequence,
    RedGiant,
    WhiteDwarf,
    NeutronStar
};

class Star : public CelestialObject {
public:
    Star(std::string name, glm::vec3 position, float mass, StarType type);

    glm::vec3 getPosition() const override;
    std::string getName() const override;
    float getMass() const;
    StarType getType() const;

private:
    std::string name_;
    glm::vec3 position_;
    float mass_;
    StarType type_;
};
