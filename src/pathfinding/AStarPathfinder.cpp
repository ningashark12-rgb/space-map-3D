#include "AStarPathfinder.h"
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

struct AStarNode {
    Star* star;
    float fScore;
    float gScore;
};

struct CompareFScore {
    bool operator()(const AStarNode& a, const AStarNode& b) {
        return a.fScore > b.fScore; 
    }
};

AStarPathfinder::AStarPathfinder(std::unique_ptr<Heuristic> heuristic) 
    : heuristic_(std::move(heuristic)) {
}

std::vector<Star*> AStarPathfinder::findPath(StarMap* map, Star* start, Star* goal) {
    if (!map) throw std::invalid_argument("AStarPathfinder: StarMap pointer is null.");
    if (!start) throw std::invalid_argument("AStarPathfinder: Start star pointer is null.");
    if (!goal) throw std::invalid_argument("AStarPathfinder: Goal star pointer is null.");
    if (!heuristic_) throw std::runtime_error("AStarPathfinder: Heuristic is not initialized.");

    if (start == goal) return { start };

    std::priority_queue<AStarNode, std::vector<AStarNode>, CompareFScore> frontier;
    
    float h_start = heuristic_->estimate(start, goal);
    frontier.push({start, h_start, 0.0f});

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

        if (current.gScore > costSoFar[current.star]) {
            continue;
        }

        for (Star* next : map->getNeighbors(current.star)) {
            float newCost = costSoFar[current.star] + map->getDistance(current.star, next);

            if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
                costSoFar[next] = newCost;
                
                float h = heuristic_->estimate(next, goal);
                float f = newCost + h;
                
                frontier.push({next, f, newCost});
                cameFrom[next] = current.star;
            }
        }
    }

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

PathResult<std::vector<Star*>> AStarPathfinder::findPathResult(StarMap* map, Star* start, Star* goal) {
    try {
        std::vector<Star*> path = findPath(map, start, goal);
        
        if (path.empty()) {
            return PathResult<std::vector<Star*>>::failure("No path found between the stars.");
        }
        
        float totalCost = 0.0f;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            totalCost += map->getDistance(path[i], path[i+1]);
        }
        
        return PathResult<std::vector<Star*>>::success(path, totalCost);
    }
    catch (const std::exception& e) {
        return PathResult<std::vector<Star*>>::failure(e.what());
    }
}