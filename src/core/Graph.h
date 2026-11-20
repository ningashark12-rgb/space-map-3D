#pragma once

#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <vector>

// Generic undirected graph. T must be usable as an unordered_map key
// (for example, pointers, integers, or a type with std::hash specialization).
template <typename T, typename Hash = std::hash<T>>
class Graph {
public:
    void addNode(const T& node) {
        adjacency_.try_emplace(node);
    }

    void addEdge(const T& first, const T& second) {
        if (!contains(first) || !contains(second)) {
            throw std::invalid_argument("Both graph nodes must be added before an edge");
        }
        if (first == second) {
            return;
        }

        addUnique(adjacency_.at(first), second);
        addUnique(adjacency_.at(second), first);
    }

    bool contains(const T& node) const {
        return adjacency_.find(node) != adjacency_.end();
    }

    std::vector<T> getNeighbors(const T& node) const {
        const auto it = adjacency_.find(node);
        return it == adjacency_.end() ? std::vector<T>{} : it->second;
    }

    std::size_t nodeCount() const noexcept {
        return adjacency_.size();
    }

private:
    static void addUnique(std::vector<T>& values, const T& value) {
        if (std::find(values.begin(), values.end(), value) == values.end()) {
            values.push_back(value);
        }
    }

    std::unordered_map<T, std::vector<T>, Hash> adjacency_;
};
