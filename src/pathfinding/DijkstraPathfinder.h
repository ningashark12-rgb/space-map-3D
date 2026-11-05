#pragma once
#include "Pathfinder.h"

class DijkstraPathfinder : public Pathfinder {
public:
    std::vector<Star*> findPath(StarMap* map, Star* start, Star* goal) override;
    std::string name() const override { return "Dijkstra"; }
};