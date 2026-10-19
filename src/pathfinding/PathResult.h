// ============================================
// pathfinding/PathResult.h
// Templated result type for pathfinding
// ============================================
#pragma once
#include <vector>
#include <string>
#include "core/Star.h"

template <typename T>
struct PathResult {
    bool found;
    T path;
    float cost;
    std::string error;

    // Helper to create a successful result
    static PathResult success(T p, float c) {
        return { true, p, c, "" };
    }

    // Helper to create a failed result
    static PathResult failure(std::string err) {
        return { false, T{}, 0.0f, err };
    }
};
