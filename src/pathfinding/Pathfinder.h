#pragma once
#include <vector>
#include <string>
#include "../core/Star.h"
#include "../core/StarMap.h"

class Pathfinder {
public:
    virtual ~Pathfinder() = default;
    
    // MUST HAVE StarMap* map HERE
    virtual std::vector<Star*> findPath(StarMap* map, Star* start, Star* goal) = 0;
    
    virtual std::string name() const = 0;
};