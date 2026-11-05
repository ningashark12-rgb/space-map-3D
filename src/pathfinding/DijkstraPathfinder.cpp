#include "DijkstraPathfinder.h"
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <stdexcept> // Exception handling

struct CompareDistance {
    bool operator()(const std::pair<Star*, float>& a, const std::pair<Star*, float>& b) {
        return a.second > b.second; 
    }
};

std::vector<Star*> DijkstraPathfinder::findPath(StarMap* map, Star* start, Star* goal) {
    // EXCEPTION HANDLING
    if (!map) throw std::invalid_argument("Dijkstra: StarMap pointer is null.");
    if (!start) throw std::invalid_argument("Dijkstra: Start star pointer is null.");
    if (!goal) throw std::invalid_argument("Dijkstra: Goal star pointer is null.");

    if (start == goal) return { start };

    std::priority_queue<std::pair<Star*, float>, 
                        std::vector<std::pair<Star*, float>>, 
                        CompareDistance> frontier;
    
    frontier.push({start, 0.0f});

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
            float newCost = costSoFar[current] + map->getDistance(current, next);

            if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
                costSoFar[next] = newCost;
                frontier.push({next, newCost});
                cameFrom[next] = current;
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