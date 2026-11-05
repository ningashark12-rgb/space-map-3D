#include <gtest/gtest.h>
#include "pathfinding/Heuristic.h"
#include "core/StarMap.h"

TEST(HeuristicTest, CalculatesEuclideanDistanceCorrectly) {
    // Create two stars 3 units apart on the X axis
    Star* starA = new Star("A", glm::vec3(0,0,0), 1.0f, StarType::MainSequence);
    Star* starB = new Star("B", glm::vec3(3,0,0), 1.0f, StarType::MainSequence);
    
    EuclideanHeuristic heuristic;
    
    // The distance should be exactly 3.0
    EXPECT_FLOAT_EQ(heuristic.estimate(starA, starB), 3.0f);
    
    delete starA;
    delete starB;
}

TEST(HeuristicTest, ReturnsZeroForSameStar) {
    Star* star = new Star("A", glm::vec3(5,5,5), 1.0f, StarType::MainSequence);
    EuclideanHeuristic heuristic;
    
    EXPECT_FLOAT_EQ(heuristic.estimate(star, star), 0.0f);
    
    delete star;
}