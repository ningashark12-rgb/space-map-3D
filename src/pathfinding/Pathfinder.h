#pragma once
#include <vector>
#include <string>
#include "../core/StarMap.h"

class Pathfinder {
public:
    virtual ~Pathfinder() = default;
    virtual std::vector<Star*> findPath(StarMap* map, Star* start, Star* goal) = 0;
    virtual std::string name() const = 0;
};