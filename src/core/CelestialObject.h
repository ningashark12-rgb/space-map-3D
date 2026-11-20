#pragma once

#include <glm/glm.hpp>
#include <string>

// Abstract base class for objects that can exist in the 3D space map.
class CelestialObject {
public:
    virtual ~CelestialObject() = default;

    virtual glm::vec3 getPosition() const = 0;
    virtual std::string getName() const = 0;
};
