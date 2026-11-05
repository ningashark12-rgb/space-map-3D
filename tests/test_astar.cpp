#include <gtest/gtest.h>
#include "pathfinding/AStarPathfinder.h"
#include "pathfinding/Heuristic.h"
#include "core/StarMap.h"
#include <memory>

TEST(AStarTest, FindsPathInSimpleGraph) {
    StarMap map;
    
    Star* starA = new Star("A", glm::vec3(0,0,0), 1.0f, StarType::MainSequence);
    Star* starB = new Star("B", glm::vec3(1,0,0), 1.0f, StarType::MainSequence);
    Star* starC = new Star("C", glm::vec3(2,0,0), 1.0f, StarType::MainSequence);
    
    map.addStar(starA);
    map.addStar(starB);
    map.addStar(starC);
    
    map.addRoute(starA, starB);
    map.addRoute(starB, starC);
    
    // A* requires a heuristic
    auto heuristic = std::make_unique<EuclideanHeuristic>();
    AStarPathfinder astar(std::move(heuristic));
    
    std::vector<Star*> path = astar.findPath(&map, starA, starC);
    
    ASSERT_EQ(path.size(), 3);
    EXPECT_EQ(path[0], starA);
    EXPECT_EQ(path[1], starB);
    EXPECT_EQ(path[2], starC);
    
    delete starA;
    delete starB;
    delete starC;
}