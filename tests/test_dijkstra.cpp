#include <gtest/gtest.h>
#include "pathfinding/DijkstraPathfinder.h"

TEST(DijkstraTest, HandlesNullInputs) {
    DijkstraPathfinder dijkstra;
    EXPECT_TRUE(dijkstra.findPath(nullptr, nullptr, nullptr).empty());
}

TEST(DijkstraTest, HandlesStartEqualsGoal) {
    DijkstraPathfinder dijkstra;
    Star* start = new Star(); // Fake star
    std::vector<Star*> path = dijkstra.findPath(nullptr, start, start);
    EXPECT_EQ(path.size(), 1);
    EXPECT_EQ(path[0], start);
    delete start;
}