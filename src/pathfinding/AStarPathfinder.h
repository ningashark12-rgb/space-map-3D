// ============================================
// pathfinding/AStarPathFinder.h
// A* Search Algorithm
// ============================================
#pragma once
#include "PathFinder.h"
#include "Heuristic.h"
#include <memory>

class AStarPathFinder : public PathFinder {
public:
    AStarPathFinder(std::unique_ptr<Heuristic> heuristic);
    
    // Overrides PathFinder
    std::vector<Star*> findPath(Star* start, Star* goal) override;
    std::string name() const override { return "A*"; }

private:
    std::unique_ptr<Heuristic> heuristic_;
};
