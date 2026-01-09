#pragma once
#include <vector>
#include <utility>
#include <string>

namespace PixelsEngine {

class GLRenderer;
class TextRenderer;

class StealthSpawnEditor {
public:
    StealthSpawnEditor();
    ~StealthSpawnEditor();

    void Update();
    void Render(GLRenderer* renderer, TextRenderer* textRenderer, int screenWidth, int screenHeight);
    
    bool IsOpen() const { return m_IsOpen; }
    void Open() { m_IsOpen = true; }
    void Close() { m_IsOpen = false; }
    void Toggle() { m_IsOpen = !m_IsOpen; }
    
    // Get the current spawn locations
    std::vector<std::pair<float, float>> GetSpawnLocations() const { return m_SpawnLocations; }
    
    // Set spawn locations
    void SetSpawnLocations(const std::vector<std::pair<float, float>>& locations) {
        m_SpawnLocations = locations;
        m_SelectedIndex = 0;
    }
    
    // Add a spawn location
    void AddSpawn(float x, float y);
    
    // Remove selected spawn location
    void RemoveSelectedSpawn();
    
    // Save to file
    void SaveToFile(const std::string& filename);
    
    // Load from file
    void LoadFromFile(const std::string& filename);

private:
    bool m_IsOpen = false;
    std::vector<std::pair<float, float>> m_SpawnLocations;
    int m_SelectedIndex = 0;
    std::string m_StatusMessage;
    float m_StatusMessageTime = 0.0f;
    
    // Editing mode
    bool m_EditingX = false;
    bool m_EditingY = false;
    std::string m_EditBuffer;
    
    void ProcessInput();
};

} // namespace PixelsEngine
