#include <gtest/gtest.h>
#include "pathFinding/DijkstraPathFinder.h"

TEST(DijkstraTest, HandlesNullInputs) {
    DijkstraPathFinder dijkstra;
    EXPECT_TRUE(dijkstra.findPath(nullptr, nullptr, nullptr).empty());
}

TEST(DijkstraTest, HandlesStartEqualsGoal) {
    DijkstraPathFinder dijkstra;
    Star* start = new Star(); // Fake star
    std::vector<Star*> path = dijkstra.findPath(nullptr, start, start);
    EXPECT_EQ(path.size(), 1);
    EXPECT_EQ(path[0], start);
    delete start;
}
