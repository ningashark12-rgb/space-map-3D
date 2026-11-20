#include "core/Graph.h"
#include "core/Star.h"
#include "core/StarData.h"
#include "core/StarMap.h"

#include <gtest/gtest.h>

#include <cmath>
#include <cstdio>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

TEST(StarTest, StoresConstructorValues) {
    Star star("Sol", glm::vec3(1.0f, 2.0f, 3.0f), 1.0f, StarType::MainSequence);

    EXPECT_EQ(star.getName(), "Sol");
    EXPECT_FLOAT_EQ(star.getPosition().x, 1.0f);
    EXPECT_FLOAT_EQ(star.getPosition().y, 2.0f);
    EXPECT_FLOAT_EQ(star.getPosition().z, 3.0f);
    EXPECT_FLOAT_EQ(star.getMass(), 1.0f);
    EXPECT_EQ(star.getType(), StarType::MainSequence);
}

TEST(StarMapTest, AddsAndFindsStars) {
    StarMap map;
    map.addStar(std::make_unique<Star>(
        "Sol", glm::vec3(0.0f), 1.0f, StarType::MainSequence));

    EXPECT_EQ(map.size(), 1u);
    const auto found = map.findByName("Sol");
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ((*found)->getName(), "Sol");
    EXPECT_FALSE(map.findByName("Missing").has_value());
}

TEST(StarMapTest, AddsUndirectedRouteAndAvoidsDuplicates) {
    StarMap map;
    map.addStar(std::make_unique<Star>(
        "A", glm::vec3(0.0f), 1.0f, StarType::MainSequence));
    map.addStar(std::make_unique<Star>(
        "B", glm::vec3(3.0f, 4.0f, 0.0f), 2.0f, StarType::RedGiant));

    const auto all = map.getAllStars();
    ASSERT_EQ(all.size(), 2u);

    map.addRoute(all[0], all[1]);
    map.addRoute(all[0], all[1]);

    EXPECT_EQ(map.getNeighbors(all[0]).size(), 1u);
    EXPECT_EQ(map.getNeighbors(all[1]).size(), 1u);
    EXPECT_EQ(map.getNeighbors(all[0])[0], all[1]);
    EXPECT_EQ(map.getNeighbors(all[1])[0], all[0]);
}

TEST(StarMapTest, CalculatesEuclideanDistance) {
    StarMap map;
    map.addStar(std::make_unique<Star>(
        "A", glm::vec3(0.0f), 1.0f, StarType::MainSequence));
    map.addStar(std::make_unique<Star>(
        "B", glm::vec3(3.0f, 4.0f, 0.0f), 1.0f, StarType::MainSequence));

    const auto stars = map.getAllStars();
    EXPECT_FLOAT_EQ(map.getDistance(stars[0], stars[1]), 5.0f);
}

TEST(StarMapTest, FindsNearestStar) {
    StarMap map;
    map.addStar(std::make_unique<Star>(
        "Far", glm::vec3(10.0f, 0.0f, 0.0f), 1.0f, StarType::MainSequence));
    map.addStar(std::make_unique<Star>(
        "Near", glm::vec3(2.0f, 0.0f, 0.0f), 1.0f, StarType::MainSequence));

    EXPECT_EQ(map.findNearest(glm::vec3(0.0f))->getName(), "Near");
    EXPECT_EQ(StarMap{}.findNearest(glm::vec3(0.0f)), nullptr);
}

TEST(StarMapTest, RejectsRouteToStarOutsideMap) {
    StarMap map;
    map.addStar(std::make_unique<Star>(
        "A", glm::vec3(0.0f), 1.0f, StarType::MainSequence));
    Star outside("Outside", glm::vec3(1.0f), 1.0f, StarType::MainSequence);

    EXPECT_THROW(map.addRoute(map.getAllStars()[0], &outside), std::invalid_argument);
}

TEST(GraphTest, StoresUndirectedEdges) {
    Graph<int> graph;
    graph.addNode(1);
    graph.addNode(2);
    graph.addEdge(1, 2);

    EXPECT_TRUE(graph.contains(1));
    EXPECT_EQ(graph.getNeighbors(1), std::vector<int>{2});
    EXPECT_EQ(graph.getNeighbors(2), std::vector<int>{1});
}

TEST(GraphTest, RejectsEdgeForMissingNode) {
    Graph<int> graph;
    graph.addNode(1);
    EXPECT_THROW(graph.addEdge(1, 2), std::invalid_argument);
}

TEST(StarDataTest, GeneratesRepeatableStars) {
    const auto first = StarData::generateRandom(5, 123);
    const auto second = StarData::generateRandom(5, 123);

    ASSERT_EQ(first.size(), 5u);
    ASSERT_EQ(second.size(), 5u);
    EXPECT_EQ(first[0]->getName(), second[0]->getName());
    EXPECT_FLOAT_EQ(first[0]->getPosition().x, second[0]->getPosition().x);
    EXPECT_FLOAT_EQ(first[0]->getMass(), second[0]->getMass());
}

TEST(StarDataTest, LoadsCsvFile) {
    const std::string path = "build/test_stars.csv";
    {
        std::ofstream output(path);
        ASSERT_TRUE(output.is_open());
        output << "name,x,y,z,mass,type\n";
        output << "Sol,0,0,0,1.0,MainSequence\n";
        output << "\"Alpha, Centauri\",1,2,3,2.0,RedGiant\n";
    }

    const auto stars = StarData::loadFromCSV(path);
    std::remove(path.c_str());

    ASSERT_EQ(stars.size(), 2u);
    EXPECT_EQ(stars[0]->getName(), "Sol");
    EXPECT_EQ(stars[1]->getName(), "Alpha, Centauri");
    EXPECT_EQ(stars[1]->getType(), StarType::RedGiant);
}
