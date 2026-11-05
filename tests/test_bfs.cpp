#include <gtest/gtest.h>
#include "pathfinding/BFSPathfinder.h"
#include "core/StarMap.h"

TEST(BFSTest, FindsPathInSimpleGraph) {
    // 1. Setup the map
    StarMap map;
    
    // We use the new Star mock
    Star* starA = new Star("A", glm::vec3(0,0,0), 1.0f, StarType::MainSequence);
    Star* starB = new Star("B", glm::vec3(1,0,0), 1.0f, StarType::MainSequence);
    Star* starC = new Star("C", glm::vec3(2,0,0), 1.0f, StarType::MainSequence);
    
    // Add them to the map
    map.addStar(starA);
    map.addStar(starB);
    map.addStar(starC);
    
    // Connect them (A -> B -> C)
    map.addRoute(starA, starB);
    map.addRoute(starB, starC);
    
    // 2. Run the BFS
    BFSPathfinder bfs;
    std::vector<Star*> path = bfs.findPath(&map, starA, starC);
    
    // 3. Check the results
    ASSERT_EQ(path.size(), 3); // Should be A, B, C
    EXPECT_EQ(path[0], starA);
    EXPECT_EQ(path[1], starB);
    EXPECT_EQ(path[2], starC);
    
    // Cleanup
    delete starA;
    delete starB;
    delete starC;
}