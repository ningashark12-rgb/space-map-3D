#include "DijkstraPathFinder.h"
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <limits>

// Custom comparator for priority queue (min-heap based on distance)
struct CompareDistance {
    bool operator()(const std::pair<Star*, float>& a, const std::pair<Star*, float>& b) {
        return a.second > b.second; // Greater means lower priority in a min-heap
    }
};

std::vector<Star*> DijkstraPathFinder::findPath(StarMap* map, Star* start, Star* goal) {
    if (!map || !start || !goal) return {};
    if (start == goal) return { start };

    // Priority queue stores pairs of (Star*, cost_so_far)
    std::priority_queue<std::pair<Star*, float>, 
                        std::vector<std::pair<Star*, float>>, 
                        CompareDistance> frontier;
    
    frontier.push({start, 0.0f});

    // Keep track of the cheapest cost to reach each star
    std::unordered_map<Star*, float> costSoFar;
    costSoFar[start] = 0.0f;

    std::unordered_map<Star*, Star*> cameFrom;
    bool found = false;

    while (!frontier.empty()) {
        Star* current = frontier.top().first;
        frontier.pop();

        if (current == goal) {
            found = true;
            break;
        }

        for (Star* next : map->getNeighbors(current)) {
            // Calculate the cost to travel to this neighbor
            float newCost = costSoFar[current] + map->getDistance(current, next);

            // If we haven't visited it, OR we found a cheaper way to get there
            if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
                costSoFar[next] = newCost;
                frontier.push({next, newCost});
                cameFrom[next] = current;
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