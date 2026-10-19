// ============================================
// pathfinding/AStarPathfinder.h
// A* Search Algorithm
// ============================================
#pragma once
#include "Pathfinder.h"
#include "Heuristic.h"
#include <memory>

class AStarPathfinder : public Pathfinder {
public:
    AStarPathfinder(std::unique_ptr<Heuristic> heuristic);
    
    // Overrides Pathfinder
    std::vector<Star*> findPath(Star* start, Star* goal) override;
    std::string name() const override { return "A*"; }

private:
    std::unique_ptr<Heuristic> heuristic_;
};
