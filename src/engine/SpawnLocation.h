#pragma once
#include <vector>
#include <utility>
#include <string>

namespace PixelsEngine {

enum class SpawnType {
    Enemy,
    Player,
    Objective
};

struct SpawnLocation {
    float x;
    float y;
    float rotation; // Rotation in radians
    std::vector<std::pair<float, float>> waypoints;
    SpawnType type = SpawnType::Enemy;
    std::string subtype = ""; // e.g., "Knight", "Skeleton_Mage"
};

} // namespace PixelsEngine