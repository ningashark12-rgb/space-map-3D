// ============================================
// pathfinding/Pathfinder.h
// Abstract base class for all pathfinding algorithms
// ============================================
#pragma once
#include <vector>
#include <string>
#include "core/Star.h"

class Pathfinder {
public:
    virtual ~Pathfinder() = default;
    
    // Find a path from start to goal. Returns empty vector if no path.
    virtual std::vector<Star*> findPath(Star* start, Star* goal) = 0;
    
    // Name of the algorithm (for display/logging)
    virtual std::string name() const = 0;
};
