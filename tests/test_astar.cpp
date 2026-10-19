#include <gtest/gtest.h>
#include "pathfinding/AStarPathfinder.h"
#include "pathfinding/Heuristic.h"
#include <memory>

TEST(AStarTest, HandlesNullInputs) {
    auto heuristic = std::make_unique<EuclideanHeuristic>();
    AStarPathfinder astar(std::move(heuristic));
    
    EXPECT_TRUE(astar.findPath(nullptr, nullptr, nullptr).empty());
}

// We will add a real graph test here when StarMap is ready!