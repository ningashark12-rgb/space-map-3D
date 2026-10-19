#pragma once
#include "Pathfinder.h"

class BFSPathfinder : public Pathfinder {
public:
    std::vector<Star*> findPath(StarMap* map, Star* start, Star* goal) override;
    std::string name() const override { return "BFS"; }
};