#include "AStarPathFinder.h"
#include <queue>
#include <unordered_map>
#include <algorithm>

// We need a struct to hold nodes in the priority queue
struct AStarNode {
    Star* star;
    float fScore; // g(n) + h(n)
    float gScore; // Cost from start to this node
};

// Custom comparator for min-heap based on fScore
struct CompareFScore {
    bool operator()(const AStarNode& a, const AStarNode& b) {
        return a.fScore > b.fScore; 
    }
};

// Constructor: initialize the heuristic
AStarPathFinder::AStarPathFinder(std::unique_ptr<Heuristic> heuristic) 
    : heuristic_(std::move(heuristic)) {
}

std::vector<Star*> AStarPathFinder::findPath(StarMap* map, Star* start, Star* goal) {
    if (!map || !start || !goal) return {};
    if (start == goal) return { start };

    // Priority queue stores AStarNodes
    std::priority_queue<AStarNode, std::vector<AStarNode>, CompareFScore> frontier;
    
    // Initialize start node
    float h_start = heuristic_->estimate(start, goal);
    frontier.push({start, h_start, 0.0f});

    // Keep track of the cheapest cost to reach each star
    std::unordered_map<Star*, float> costSoFar;
    costSoFar[start] = 0.0f;

    std::unordered_map<Star*, Star*> cameFrom;
    bool found = false;

    while (!frontier.empty()) {
        AStarNode current = frontier.top();
        frontier.pop();

        if (current.star == goal) {
            found = true;
            break;
        }

        // Sometimes we find a better path to a node that is already in the queue.
        // If we pop a node that has a higher cost than what we already found, skip it.
        if (current.gScore > costSoFar[current.star]) {
            continue;
        }

        for (Star* next : map->getNeighbors(current.star)) {
            // Calculate the cost to travel to this neighbor
            float newCost = costSoFar[current.star] + map->getDistance(current.star, next);

            // If we haven't visited it, OR we found a cheaper way to get there
            if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
                costSoFar[next] = newCost;
                
                // f(n) = g(n) + h(n)
                float h = heuristic_->estimate(next, goal);
                float f = newCost + h;
                
                frontier.push({next, f, newCost});
                cameFrom[next] = current.star;
            }
        }
    }

    // Reconstruct path
    std::vector<Star*> path;
    if (found) {
        Star* curr = goal;
        while (curr != nullptr) {
            path.push_back(curr);
            curr = cameFrom[curr];
        }
        std::reverse(path.begin(), path.end());
    }
    return path;
}