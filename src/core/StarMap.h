#pragma once

#include "Star.h"

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

class StarMap {
public:
    // Pathfinding interface used by Member B.
    std::vector<Star*> getNeighbors(Star* star) const;
    float getDistance(Star* a, Star* b) const;
    std::vector<Star*> getAllStars() const;

    // Rendering / selection interface.
    std::optional<Star*> findByName(const std::string& name) const;
    Star* findNearest(const glm::vec3& position) const;

    // Setup interface.
    void addStar(std::unique_ptr<Star> star);
    void addRoute(Star* a, Star* b);

    // Convenience query; does not transfer ownership.
    bool contains(const Star* star) const;
    std::size_t size() const noexcept;

private:
    std::vector<std::unique_ptr<Star>> stars_;
    std::unordered_map<Star*, std::vector<Star*>> adjacency_;
};
