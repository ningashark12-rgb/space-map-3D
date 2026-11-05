#include <gtest/gtest.h>
#include "pathfinding/DijkstraPathfinder.h"
#include "core/StarMap.h"

TEST(DijkstraTest, FindsPathInSimpleGraph) {
    StarMap map;
    
    Star* starA = new Star("A", glm::vec3(0,0,0), 1.0f, StarType::MainSequence);
    Star* starB = new Star("B", glm::vec3(1,0,0), 1.0f, StarType::MainSequence);
    Star* starC = new Star("C", glm::vec3(2,0,0), 1.0f, StarType::MainSequence);
    
    map.addStar(starA);
    map.addStar(starB);
    map.addStar(starC);
    
    map.addRoute(starA, starB);
    map.addRoute(starB, starC);
    
    DijkstraPathfinder dijkstra;
    std::vector<Star*> path = dijkstra.findPath(&map, starA, starC);
    
    ASSERT_EQ(path.size(), 3);
    EXPECT_EQ(path[0], starA);
    EXPECT_EQ(path[1], starB);
    EXPECT_EQ(path[2], starC);
    
    delete starA;
    delete starB;
    delete starC;
}