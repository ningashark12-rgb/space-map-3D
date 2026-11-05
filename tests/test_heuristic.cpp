#include <gtest/gtest.h>
#include "pathfinding/Heuristic.h"

TEST(HeuristicTest, EuclideanDistanceIsZeroForSameStar) {
    EuclideanHeuristic heuristic;
    Star* star = new Star();
    
    // Distance to itself should be 0
    EXPECT_FLOAT_EQ(heuristic.estimate(star, star), 0.0f);
    
    delete star;
}