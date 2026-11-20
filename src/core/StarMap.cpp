#include "core/StarMap.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

std::vector<Star*> StarMap::getNeighbors(Star* star) const {
    const auto it = adjacency_.find(star);
    if (it == adjacency_.end()) {
        return {};
    }
    return it->second;
}

float StarMap::getDistance(Star* a, Star* b) const {
    if (a == nullptr || b == nullptr) {
        return std::numeric_limits<float>::infinity();
    }

    const glm::vec3 difference = a->getPosition() - b->getPosition();
    return glm::length(difference);
}

std::vector<Star*> StarMap::getAllStars() const {
    std::vector<Star*> result;
    result.reserve(stars_.size());

    for (const auto& star : stars_) {
        result.push_back(star.get());
    }
    return result;
}

std::optional<Star*> StarMap::findByName(const std::string& name) const {
    const auto it = std::find_if(
        stars_.begin(), stars_.end(),
        [&name](const std::unique_ptr<Star>& star) {
            return star->getName() == name;
        });

    if (it == stars_.end()) {
        return std::nullopt;
    }
    return it->get();
}

Star* StarMap::findNearest(const glm::vec3& position) const {
    if (stars_.empty()) {
        return nullptr;
    }

    Star* nearest = nullptr;
    float bestDistanceSquared = std::numeric_limits<float>::infinity();

    for (const auto& star : stars_) {
        const glm::vec3 difference = star->getPosition() - position;
        const float distanceSquared = glm::dot(difference, difference);

        if (distanceSquared < bestDistanceSquared) {
            bestDistanceSquared = distanceSquared;
            nearest = star.get();
        }
    }
    return nearest;
}

void StarMap::addStar(std::unique_ptr<Star> star) {
    if (!star) {
        throw std::invalid_argument("StarMap::addStar received a null star");
    }

    Star* rawStar = star.get();
    if (contains(rawStar)) {
        throw std::invalid_argument("Star is already in this StarMap");
    }

    adjacency_.try_emplace(rawStar);
    stars_.push_back(std::move(star));
}

void StarMap::addRoute(Star* a, Star* b) {
    if (a == nullptr || b == nullptr) {
        throw std::invalid_argument("StarMap::addRoute requires non-null stars");
    }
    if (!contains(a) || !contains(b)) {
        throw std::invalid_argument("Both stars must belong to this StarMap");
    }
    if (a == b) {
        return;
    }

    auto addUnique = [](std::vector<Star*>& neighbors, Star* target) {
        if (std::find(neighbors.begin(), neighbors.end(), target) == neighbors.end()) {
            neighbors.push_back(target);
        }
    };

    addUnique(adjacency_.at(a), b);
    addUnique(adjacency_.at(b), a);
}

bool StarMap::contains(const Star* star) const {
    if (star == nullptr) {
        return false;
    }

    return std::any_of(
        stars_.begin(), stars_.end(),
        [star](const std::unique_ptr<Star>& owned) {
            return owned.get() == star;
        });
}

std::size_t StarMap::size() const noexcept {
    return stars_.size();
}
