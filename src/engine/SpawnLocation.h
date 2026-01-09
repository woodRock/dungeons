#pragma once
#include <vector>
#include <utility>

namespace PixelsEngine {

struct SpawnLocation {
    float x;
    float y;
    float rotation; // Rotation in radians
    std::vector<std::pair<float, float>> waypoints;
};

} // namespace PixelsEngine
