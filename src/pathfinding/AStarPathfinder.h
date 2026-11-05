// ============================================
// pathfinding/AStarPathFinder.h
// A* Search Algorithm
// ============================================
#pragma once
#include <vector>
#include <memory>
#include "Pathfinder.h"
#include "Heuristic.h"
#include "PathResult.h"
#include "../core/StarMap.h"

class AStarPathfinder : public Pathfinder {
public:
    AStarPathfinder(std::unique_ptr<Heuristic> heuristic);
        // Using our C++ Template to return the result safely!
    PathResult<std::vector<Star*>> findPathResult(StarMap* map, Star* start, Star* goal);
    
    // Overrides PathFinder
    std::vector<Star*> findPath(StarMap* map, Star* start, Star* goal) override;
    std::string name() const override { return "A*"; }

private:
    std::unique_ptr<Heuristic> heuristic_;
};
