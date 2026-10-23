#pragma once
#include "PathFinder.h"

class DijkstraPathFinder : public PathFinder {
public:
    std::vector<Star*> findPath(StarMap* map, Star* start, Star* goal) override;
    std::string name() const override { return "Dijkstra"; }
};