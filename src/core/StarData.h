#pragma once

#include "Star.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class StarData {
public:
    // CSV columns: name,x,y,z,mass,type
    // The first non-empty line may be the header shown above.
    static std::vector<std::unique_ptr<Star>> loadFromCSV(const std::string& filePath);

    // Deterministic when the same seed is supplied.
    static std::vector<std::unique_ptr<Star>> generateRandom(
        std::size_t count,
        std::uint32_t seed = 42);
};
