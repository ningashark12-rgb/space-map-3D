#include "core/StarData.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
std::string trim(const std::string& value) {
    const auto first = std::find_if_not(value.begin(), value.end(),
        [](unsigned char ch) { return std::isspace(ch) != 0; });
    const auto last = std::find_if_not(value.rbegin(), value.rend(),
        [](unsigned char ch) { return std::isspace(ch) != 0; }).base();

    if (first >= last) {
        return {};
    }
    return std::string(first, last);
}

std::vector<std::string> parseCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        const char ch = line[i];

        if (ch == '"') {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                field += '"';
                ++i;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (ch == ',' && !inQuotes) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += ch;
        }
    }

    if (inQuotes) {
        throw std::runtime_error("CSV line contains an unmatched quote");
    }

    fields.push_back(trim(field));
    return fields;
}

StarType parseStarType(const std::string& value) {
    if (value == "MainSequence") return StarType::MainSequence;
    if (value == "RedGiant") return StarType::RedGiant;
    if (value == "WhiteDwarf") return StarType::WhiteDwarf;
    if (value == "NeutronStar") return StarType::NeutronStar;

    throw std::runtime_error("Unknown star type: " + value);
}

float parseFloat(const std::string& value, const std::string& fieldName) {
    try {
        std::size_t parsed = 0;
        const float result = std::stof(value, &parsed);
        if (parsed != value.size()) {
            throw std::runtime_error("Invalid number");
        }
        return result;
    } catch (const std::exception&) {
        throw std::runtime_error("Invalid numeric value for " + fieldName + ": " + value);
    }
}
} // namespace

std::vector<std::unique_ptr<Star>> StarData::loadFromCSV(const std::string& filePath) {
    std::ifstream input(filePath);
    if (!input.is_open()) {
        throw std::runtime_error("Could not open CSV file: " + filePath);
    }

    std::vector<std::unique_ptr<Star>> stars;
    std::string line;
    std::size_t lineNumber = 0;

    while (std::getline(input, line)) {
        ++lineNumber;
        if (trim(line).empty()) {
            continue;
        }

        std::vector<std::string> fields;
        try {
            fields = parseCsvLine(line);
        } catch (const std::exception& error) {
            throw std::runtime_error(
                "CSV error on line " + std::to_string(lineNumber) + ": " + error.what());
        }

        if (fields.size() != 6) {
            throw std::runtime_error(
                "CSV line " + std::to_string(lineNumber) +
                " must contain exactly 6 columns");
        }

        if (lineNumber == 1 && fields[0] == "name") {
            continue;
        }

        try {
            const std::string name = fields[0];
            const glm::vec3 position(
                parseFloat(fields[1], "x"),
                parseFloat(fields[2], "y"),
                parseFloat(fields[3], "z"));
            const float mass = parseFloat(fields[4], "mass");
            const StarType type = parseStarType(fields[5]);

            stars.push_back(std::make_unique<Star>(name, position, mass, type));
        } catch (const std::exception& error) {
            throw std::runtime_error(
                "CSV error on line " + std::to_string(lineNumber) + ": " + error.what());
        }
    }

    return stars;
}

std::vector<std::unique_ptr<Star>> StarData::generateRandom(
    std::size_t count, std::uint32_t seed) {

    std::mt19937 generator(seed);
    std::uniform_real_distribution<float> coordinate(-100.0f, 100.0f);
    std::uniform_real_distribution<float> mass(0.1f, 50.0f);
    std::uniform_int_distribution<int> typeIndex(0, 3);

    const StarType types[] = {
        StarType::MainSequence,
        StarType::RedGiant,
        StarType::WhiteDwarf,
        StarType::NeutronStar
    };

    std::vector<std::unique_ptr<Star>> stars;
    stars.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        const std::string name = "RandomStar_" + std::to_string(i + 1);
        const glm::vec3 position(
            coordinate(generator),
            coordinate(generator),
            coordinate(generator));

        stars.push_back(std::make_unique<Star>(
            name, position, mass(generator), types[typeIndex(generator)]));
    }

    return stars;
}
