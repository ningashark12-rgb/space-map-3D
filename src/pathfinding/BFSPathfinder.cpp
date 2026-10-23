#include "BFSPathFinder.h"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

std::vector<Star*> BFSPathFinder::findPath(StarMap* map, Star* start, Star* goal) {
    if (!map || !start || !goal) return {};
    if (start == goal) return { start };

    std::queue<Star*> frontier;
    frontier.push(start);

    std::unordered_set<Star*> visited;
    visited.insert(start);

    std::unordered_map<Star*, Star*> cameFrom;
    bool found = false;

    while (!frontier.empty()) {
        Star* current = frontier.front();
        frontier.pop();

        if (current == goal) {
            found = true;
            break;
        }

        for (Star* next : map->getNeighbors(current)) {
            if (visited.find(next) == visited.end()) {
                visited.insert(next);
                frontier.push(next);
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