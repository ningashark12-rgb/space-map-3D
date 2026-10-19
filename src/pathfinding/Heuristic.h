// ============================================
// pathfinding/Heuristic.h
// Abstract base for heuristics used by A*
// ============================================
#pragma once
#include "core/Star.h"
#include <cmath> // for std::sqrt

class Heuristic {
public:
    virtual ~Heuristic() = default;
    
    // Estimate the distance from a to b.
    // Must be admissible (never overestimate the actual cost).
    virtual float estimate(Star* a, Star* b) const = 0;
};

// Euclidean distance (straight line in 3D space)
class EuclideanHeuristic : public Heuristic {
public:
    float estimate(Star* a, Star* b) const override {
        // Get positions
        auto posA = a->getPosition();
        auto posB = b->getPosition();
        
        // Calculate straight-line distance
        float dx = posA.x - posB.x;
        float dy = posA.y - posB.y;
        float dz = posA.z - posB.z;
        
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }
};
