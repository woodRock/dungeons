#pragma once
#include "Camera.h"
#include "ECS.h"
#include "GLRenderer.h"
#include <vector>
#include <string>

namespace PixelsEngine {

// Centralized settings struct for easy visual tweaking
struct MinimapSettings {
    SDL_Color bgColor = {20, 20, 30, 200};      // Dark semi-transparent background
    SDL_Color borderColor = {100, 100, 120, 255}; // Light border
    SDL_Color playerColor = {50, 255, 50, 255};   // Green player
    SDL_Color enemyColor = {220, 60, 60, 255};    // Red enemies
    SDL_Color wallColor = {140, 140, 160, 255};   // Grey walls
    SDL_Color doorColor = {200, 180, 50, 255};    // Gold/Yellow doors
    SDL_Color chestColor = {255, 215, 0, 255};    // Gold chests
    SDL_Color propColor = {120, 100, 80, 255};    // Brown props
    
    int borderThickness = 3;
    float baseScale = 8.0f; // Zoom baseline (increased from 4.0 for better detail)
};

class Minimap {
public:
    Minimap();
    ~Minimap();

    void Init(int width, int height);

    // Main render loop
    void Render(GLRenderer* renderer, Registry* registry, Entity playerEntity, 
                Camera* camera, int screenWidth, int screenHeight);
    
    // Configuration setters
    void SetPosition(int x, int y);
    void SetSize(int width, int height);
    void SetZoom(float zoom);
    void SetVisible(bool visible);
    void ToggleVisible() { m_Visible = !m_Visible; }

private:
    int m_PosX = 20;
    int m_PosY = 20;
    int m_Width = 200;
    int m_Height = 200;
    float m_Zoom = 1.0f;
    bool m_Visible = true;
    
    MinimapSettings m_Settings;

    // Helper functions
    void RenderBackground(GLRenderer* renderer);
    void RenderEntities(GLRenderer* renderer, Registry* registry, Entity playerEntity, Camera* camera);
    void RenderPlayerIcon(GLRenderer* renderer);
    
    // Coordinate conversion helper (Handles Rotation & Mirroring)
    bool WorldToMinimap(float worldX, float worldY, float playerX, float playerY, float playerAngle, float& outX, float& outY);
};

} // namespace PixelsEngine