#pragma once
#include <vector>
#include <unordered_map>
#include <optional>
#include "Star.h"

class StarMap {
public:
    void addStar(Star* star) { stars_.push_back(star); }
    
    void addRoute(Star* a, Star* b) {
        adjacency_[a].push_back(b);
        adjacency_[b].push_back(a); 
    }

    std::vector<Star*> getNeighbors(Star* star) const {
        auto it = adjacency_.find(star);
        if (it != adjacency_.end()) return it->second;
        return {};
    }

    float getDistance(Star* a, Star* b) const {
        glm::vec3 diff = a->getPosition() - b->getPosition();
        return glm::length(diff); 
    }

    std::vector<Star*> getAllStars() const { return stars_; }

    std::optional<Star*> findByName(const std::string& name) const {
        for (auto* s : stars_) if (s->getName() == name) return s;
        return std::nullopt;
    }

private:
    std::vector<Star*> stars_;
    std::unordered_map<Star*, std::vector<Star*>> adjacency_;
};