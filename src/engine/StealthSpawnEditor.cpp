#include "StealthSpawnEditor.h"
#include "Input.h"
#include "GLRenderer.h"
#include "TextRenderer.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PixelsEngine {

StealthSpawnEditor::StealthSpawnEditor() {
    // Load default spawns
    m_SpawnLocations = {
        {48.0f, -32.0f, 0.0f, {}, SpawnType::Enemy},
        {36.0f, -52.0f, 0.0f, {}, SpawnType::Enemy},
        {20.0f, -40.0f, 0.0f, {}, SpawnType::Enemy},
        {28.0f, 4.0f, 0.0f, {}, SpawnType::Enemy},
    };
}

StealthSpawnEditor::~StealthSpawnEditor() = default;

void StealthSpawnEditor::AddSpawn(float x, float y, float rotation, SpawnType type) {
    SpawnLocation loc;
    loc.x = x;
    loc.y = y;
    loc.rotation = rotation;
    loc.type = type;
    m_SpawnLocations.push_back(loc);
    m_SelectedIndex = m_SpawnLocations.size() - 1;
    m_StatusMessage = "Spawn added at (" + std::to_string((int)x) + ", " + std::to_string((int)y) + ")";
    m_StatusMessageTime = 2.0f;
}

void StealthSpawnEditor::RemoveSelectedSpawn() {
    if (m_SelectedIndex < m_SpawnLocations.size()) {
        m_SpawnLocations.erase(m_SpawnLocations.begin() + m_SelectedIndex);
        if (m_SelectedIndex >= m_SpawnLocations.size() && m_SelectedIndex > 0) {
            m_SelectedIndex--;
        }
        m_StatusMessage = "Spawn removed";
        m_StatusMessageTime = 2.0f;
    }
}

void StealthSpawnEditor::SaveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        m_StatusMessage = "Failed to save: " + filename;
        m_StatusMessageTime = 2.0f;
        return;
    }
    
    for (const auto& loc : m_SpawnLocations) {
        file << loc.x << " " << loc.y << " " << loc.rotation << " " << loc.waypoints.size();
        for (const auto& wp : loc.waypoints) {
            file << " " << wp.first << " " << wp.second;
        }
        file << " " << (int)loc.type << "\n";
    }
    file.close();
    
    m_StatusMessage = "Saved " + std::to_string(m_SpawnLocations.size()) + " spawns";
    m_StatusMessageTime = 2.0f;
}

void StealthSpawnEditor::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        m_StatusMessage = "Failed to load: " + filename;
        m_StatusMessageTime = 2.0f;
        return;
    }
    
    m_SpawnLocations.clear();
    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        
        SpawnLocation loc;
        loc.x = 0; loc.y = 0; loc.rotation = 0;
        loc.type = SpawnType::Enemy; // Default
        
        ss >> loc.x >> loc.y;
        
        // Try reading rotation
        if (ss >> loc.rotation) {
            // Try reading waypoints count
            size_t numWaypoints = 0;
            if (ss >> numWaypoints) {
                for (size_t i = 0; i < numWaypoints; ++i) {
                    float wx, wy;
                    if (ss >> wx >> wy) {
                        loc.waypoints.push_back({wx, wy});
                    }
                }
            }
            // Try reading type
            int typeVal = 0;
            if (ss >> typeVal) {
                loc.type = (SpawnType)typeVal;
            }
        }
        
        m_SpawnLocations.push_back(loc);
        count++;
    }
    file.close();
    
    m_SelectedIndex = 0;
    m_StatusMessage = "Loaded " + std::to_string(m_SpawnLocations.size()) + " spawns";
    m_StatusMessageTime = 2.0f;
}

void StealthSpawnEditor::ProcessInput() {
    if (!m_IsOpen) return;
    
    // Arrow keys to navigate
    if (Input::IsKeyPressed(SDL_SCANCODE_UP)) {
        if (m_SelectedIndex > 0) m_SelectedIndex--;
        m_EditingX = false;
        m_EditingY = false;
        m_EditingRot = false;
    }
    if (Input::IsKeyPressed(SDL_SCANCODE_DOWN)) {
        if (m_SelectedIndex < m_SpawnLocations.size() - 1) m_SelectedIndex++;
        m_EditingX = false;
        m_EditingY = false;
        m_EditingRot = false;
    }
    
    // X key to edit X coordinate
    if (Input::IsKeyPressed(SDL_SCANCODE_X) && !m_EditingX && !m_EditingY && !m_EditingRot) {
        m_EditingX = true;
        m_EditBuffer = std::to_string((int)m_SpawnLocations[m_SelectedIndex].x);
        Input::StartTextInput();
    }
    
    // Y key to edit Y coordinate
    if (Input::IsKeyPressed(SDL_SCANCODE_Y) && !m_EditingX && !m_EditingY && !m_EditingRot) {
        m_EditingY = true;
        m_EditBuffer = std::to_string((int)m_SpawnLocations[m_SelectedIndex].y);
        Input::StartTextInput();
    }
    
    // R key to edit Rotation
    if (Input::IsKeyPressed(SDL_SCANCODE_R) && !m_EditingX && !m_EditingY && !m_EditingRot) {
        m_EditingRot = true;
        m_EditBuffer = std::to_string((int)(m_SpawnLocations[m_SelectedIndex].rotation * 180.0f / 3.14159f)); // Display in degrees
        Input::StartTextInput();
    }
    
    // T key to cycle Type
    static bool tWasDown = false;
    bool tIsDown = Input::IsKeyDown(SDL_SCANCODE_T);
    if (tIsDown && !tWasDown && !m_EditingX && !m_EditingY && !m_EditingRot) {
        int type = (int)m_SpawnLocations[m_SelectedIndex].type;
        type = (type + 1) % 3; // Cycle 0, 1, 2
        m_SpawnLocations[m_SelectedIndex].type = (SpawnType)type;
        m_StatusMessage = "Changed Type";
        m_StatusMessageTime = 2.0f;
    }
    tWasDown = tIsDown;
    
    // Process text input while editing
    if (m_EditingX || m_EditingY || m_EditingRot) {
        std::string textInput = Input::GetTextInput();
        if (!textInput.empty()) {
            m_EditBuffer += textInput;
            Input::ClearTextInput();
        }
        
        if (Input::IsKeyPressed(SDL_SCANCODE_BACKSPACE) && !m_EditBuffer.empty()) {
            m_EditBuffer.pop_back();
        }
        
        if (Input::IsKeyPressed(SDL_SCANCODE_RETURN)) {
            try {
                float value = std::stof(m_EditBuffer);
                if (m_EditingX) {
                    m_SpawnLocations[m_SelectedIndex].x = value;
                } else if (m_EditingY) {
                    m_SpawnLocations[m_SelectedIndex].y = value;
                } else if (m_EditingRot) {
                    m_SpawnLocations[m_SelectedIndex].rotation = value * 3.14159f / 180.0f; // Convert back to radians
                }
                m_StatusMessage = "Updated spawn";
                m_StatusMessageTime = 2.0f;
            } catch (...) {
                m_StatusMessage = "Invalid input";
                m_StatusMessageTime = 2.0f;
            }
            m_EditingX = false;
            m_EditingY = false;
            m_EditingRot = false;
            Input::StopTextInput();
            m_EditBuffer.clear();
        }
    }
    
    // Delete key to remove spawn
    if (Input::IsKeyPressed(SDL_SCANCODE_DELETE)) {
        RemoveSelectedSpawn();
    }
    
    // A key to add new spawn at player location (0,0 by default)
    if (Input::IsKeyPressed(SDL_SCANCODE_A)) {
        AddSpawn(0.0f, 0.0f, 0.0f, SpawnType::Enemy);
    }
    
    // S key to save
    if (Input::IsKeyPressed(SDL_SCANCODE_S)) {
        SaveToFile("assets/saves/spawn_config.txt");
    }
    
    // L key to load
    if (Input::IsKeyPressed(SDL_SCANCODE_L)) {
        LoadFromFile("assets/saves/spawn_config.txt");
    }
}

void StealthSpawnEditor::Update() {
    ProcessInput();
    
    // Update status message timer
    if (m_StatusMessageTime > 0.0f) {
        m_StatusMessageTime -= 0.016f;  // Assuming ~60 FPS
    }
}

void StealthSpawnEditor::Render(GLRenderer* renderer, TextRenderer* textRenderer, int screenWidth, int screenHeight) {
    if (!m_IsOpen) return;
    
    // Draw editor background panel
    renderer->DrawRect2D(screenWidth - 400, 10, 390, 300, {20, 20, 40, 220});
    renderer->DrawRect2D(screenWidth - 400, 10, 390, 2, {100, 200, 255, 255});
    
    // Title
    textRenderer->RenderText(renderer, "SPAWN EDITOR", screenWidth - 390, 15, {100, 200, 255, 255});
    
    // Draw spawn list
    int y = 35;
    for (size_t i = 0; i < m_SpawnLocations.size(); ++i) {
        SDL_Color color = (i == m_SelectedIndex) ? SDL_Color{0, 255, 100, 255} : SDL_Color{200, 200, 200, 255};
        
        std::string text = "[" + std::to_string(i) + "] ";
        text += "(" + std::to_string((int)m_SpawnLocations[i].x) + ", ";
        text += std::to_string((int)m_SpawnLocations[i].y) + ", ";
        text += std::to_string((int)(m_SpawnLocations[i].rotation * 180.0f / 3.14159f)) + "deg)";
        
        // Show Type
        if (m_SpawnLocations[i].type == SpawnType::Player) text += " [P]";
        else if (m_SpawnLocations[i].type == SpawnType::Objective) text += " [O]";
        else text += " [E]";
        
        if (!m_SpawnLocations[i].waypoints.empty()) {
            text += " WP:" + std::to_string(m_SpawnLocations[i].waypoints.size());
        }
        
        textRenderer->RenderText(renderer, text, screenWidth - 390, y, color);
        y += 22;
    }
    
    // Draw controls
    int controlY = 35 + m_SpawnLocations.size() * 22 + 10;
    textRenderer->RenderText(renderer, "UP/DOWN: Select", screenWidth - 390, controlY, {150, 150, 150, 255});
    controlY += 18;
    textRenderer->RenderText(renderer, "X: Edit X | Y: Edit Y | R: Edit Rot", screenWidth - 390, controlY, {150, 150, 150, 255});
    controlY += 18;
    textRenderer->RenderText(renderer, "T: Cycle Type | DEL: Remove | A: Add", screenWidth - 390, controlY, {150, 150, 150, 255});
    controlY += 18;
    textRenderer->RenderText(renderer, "S: Save | L: Load", screenWidth - 390, controlY, {150, 150, 150, 255});
    
    // Draw status message
    if (m_StatusMessageTime > 0.0f) {
        controlY += 20;
        textRenderer->RenderText(renderer, m_StatusMessage, screenWidth - 390, controlY, {255, 255, 100, 255});
    }
    
    // Draw editing prompt if in edit mode
    if (m_EditingX || m_EditingY || m_EditingRot) {
        std::string prompt;
        if (m_EditingX) prompt = "Edit X: ";
        else if (m_EditingY) prompt = "Edit Y: ";
        else prompt = "Edit Rotation (deg): ";
        textRenderer->RenderText(renderer, prompt + m_EditBuffer + "_", screenWidth - 390, 10, {0, 255, 100, 255});
    }
}

} // namespace PixelsEngine