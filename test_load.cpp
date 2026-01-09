#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

enum class SpawnType {
    Enemy,
    Player,
    Objective
};

struct SpawnLocation {
    float x;
    float y;
    float rotation;
    std::vector<std::pair<float, float>> waypoints;
    SpawnType type = SpawnType::Enemy;
};

int main() {
    std::vector<SpawnLocation> m_SpawnLocations;
    std::string filename = "assets/saves/spawn_config.txt";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open file" << std::endl;
        return 1;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        
        SpawnLocation loc;
        loc.x = 0; loc.y = 0; loc.rotation = 0;
        loc.type = SpawnType::Enemy;
        
        ss >> loc.x >> loc.y;
        
        if (ss >> loc.rotation) {
            size_t numWaypoints = 0;
            if (ss >> numWaypoints) {
                for (size_t i = 0; i < numWaypoints; ++i) {
                    float wx, wy;
                    if (ss >> wx >> wy) {
                        loc.waypoints.push_back({wx, wy});
                    }
                }
            }
            int typeVal = 0;
            if (ss >> typeVal) {
                loc.type = (SpawnType)typeVal;
            }
        }
        m_SpawnLocations.push_back(loc);
        std::cout << "Loaded: x=" << loc.x << " y=" << loc.y << " type=" << (int)loc.type << std::endl;
    }
    return 0;
}
