#pragma once
#include "Camera.h"
#include "ECS.h"
#include "GLRenderer.h"
#include <vector>
#include <string>

namespace PixelsEngine {

// Simple container for minimap styling to avoid magic numbers
struct MinimapSettings {
    SDL_Color bgColor = {20, 20, 30, 200};
    SDL_Color borderColor = {100, 100, 120, 255};
    SDL_Color playerColor = {50, 255, 50, 255};
    SDL_Color enemyColor = {220, 60, 60, 255};
    SDL_Color wallColor = {140, 140, 160, 255};
    SDL_Color chestColor = {255, 215, 0, 255};
    SDL_Color propColor = {120, 100, 80, 255};
    
    int borderThickness = 3;
    float baseScale = 4.0f; // Pixels per world unit
};

class Minimap {
public:
    Minimap();
    ~Minimap();

    void Init(int width, int height);
    
    // Main render loop
    void Render(GLRenderer* renderer, Registry* registry, Entity playerEntity, 
                Camera* camera, int screenWidth, int screenHeight);
    
    // Configuration
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

    // Helpers
    void RenderBackground(GLRenderer* renderer);
    void RenderEntities(GLRenderer* renderer, Registry* registry, Entity playerEntity, Camera* camera);
    void RenderPlayerIcon(GLRenderer* renderer, float rotation);
    
    // Coordinate conversion helper
    bool WorldToMinimap(float worldX, float worldY, float playerX, float playerY, int& outX, int& outY);
};

} // namespace PixelsEngine