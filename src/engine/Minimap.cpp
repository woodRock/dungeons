#include "Minimap.h"
#include "Components.h"
#include <cmath>
#include <algorithm>

namespace PixelsEngine {

Minimap::Minimap() {}

Minimap::~Minimap() {}

void Minimap::Init(int width, int height) {
    m_Width = width;
    m_Height = height;
}

void Minimap::SetPosition(int x, int y) {
    m_PosX = x;
    m_PosY = y;
}

void Minimap::SetSize(int width, int height) {
    m_Width = width;
    m_Height = height;
}

void Minimap::SetZoom(float zoom) {
    // Clamp zoom to reasonable levels
    if (zoom < 0.5f) zoom = 0.5f;
    if (zoom > 5.0f) zoom = 5.0f;
    m_Zoom = zoom;
}

void Minimap::SetVisible(bool visible) {
    m_Visible = visible;
}

// Helper to convert world coordinates to minimap screen coordinates.
// Returns false if the point is outside the minimap bounds.
bool Minimap::WorldToMinimap(float worldX, float worldY, float playerX, float playerY, int& outX, int& outY) {
    int centerX = m_PosX + m_Width / 2;
    int centerY = m_PosY + m_Height / 2;

    // "North-Up" logic: we only scale relative to player position, we do NOT rotate the world.
    // The world stays fixed to cardinal directions, the player arrow rotates.
    float effectiveScale = m_Settings.baseScale * m_Zoom;

    float dx = worldX - playerX;
    float dy = worldY - playerY;

    // Apply scale
    float relX = dx * effectiveScale;
    float relY = dy * effectiveScale;

    // Bounds checking (Strict clipping)
    // We add a small margin so objects don't touch the border
    int margin = 2; 
    int halfW = (m_Width / 2) - margin;
    int halfH = (m_Height / 2) - margin;

    if (abs(relX) > halfW || abs(relY) > halfH) {
        return false;
    }

    outX = centerX + (int)relX;
    outY = centerY + (int)relY;
    return true;
}

void Minimap::RenderBackground(GLRenderer* renderer) {
    // 1. Draw Background
    renderer->DrawRect2D(m_PosX, m_PosY, m_Width, m_Height, m_Settings.bgColor);

    // 2. Draw Border (Constructed of 4 rects)
    int t = m_Settings.borderThickness;
    SDL_Color c = m_Settings.borderColor;

    // Top
    renderer->DrawRect2D(m_PosX, m_PosY, m_Width, t, c);
    // Bottom
    renderer->DrawRect2D(m_PosX, m_PosY + m_Height - t, m_Width, t, c);
    // Left
    renderer->DrawRect2D(m_PosX, m_PosY, t, m_Height, c);
    // Right
    renderer->DrawRect2D(m_PosX + m_Width - t, m_PosY, t, m_Height, c);
}

void Minimap::RenderPlayerIcon(GLRenderer* renderer, float rotation) {
    int centerX = m_PosX + m_Width / 2;
    int centerY = m_PosY + m_Height / 2;

    // Draw a direction indicator arrow
    // We calculate a triangle or arrow shape based on rotation
    float arrowLen = 10.0f;
    float wingLen = 6.0f;
    
    // Tip of the arrow
    float tipX = centerX + cos(rotation) * arrowLen;
    float tipY = centerY + sin(rotation) * arrowLen;

    // Back left wing
    float leftX = centerX + cos(rotation + 2.5f) * wingLen;
    float leftY = centerY + sin(rotation + 2.5f) * wingLen;

    // Back right wing
    float rightX = centerX + cos(rotation - 2.5f) * wingLen;
    float rightY = centerY + sin(rotation - 2.5f) * wingLen;

    // Since we only have DrawRect2D, we draw a small cluster of pixels 
    // or very thin rects to simulate lines/shape.
    
    // Center Dot
    renderer->DrawRect2D(centerX - 2, centerY - 2, 4, 4, m_Settings.playerColor);

    // Tip
    renderer->DrawRect2D((int)tipX - 1, (int)tipY - 1, 3, 3, m_Settings.playerColor);

    // Visualize the "Arrow" by drawing points interpolated between center and tip
    int steps = 5;
    for(int i = 1; i < steps; i++) {
        float t = (float)i / steps;
        
        // Main Shaft
        int sx = centerX + (int)((tipX - centerX) * t);
        int sy = centerY + (int)((tipY - centerY) * t);
        renderer->DrawRect2D(sx, sy, 2, 2, m_Settings.playerColor);
    }
}

void Minimap::RenderEntities(GLRenderer* renderer, Registry* registry, Entity playerEntity, Camera* camera) {
    if (!camera) return;

    // --- RENDER ENVIRONMENT (MESHES) ---
    auto& meshView = registry->View<MeshComponent>();
    
    // Optimization: Don't create string objects inside the loop if possible, 
    // or use a component tag system in the future.
    for (auto& pair : meshView) {
        Entity e = pair.first;
        if (e == playerEntity) continue;

        auto* t = registry->GetComponent<Transform3DComponent>(e);
        auto* mesh = registry->GetComponent<MeshComponent>(e);
        if (!t || !mesh) continue;

        // Determine Type
        std::string meshName = mesh->meshName;
        // Basic lowercasing
        std::transform(meshName.begin(), meshName.end(), meshName.begin(), ::tolower);

        SDL_Color color = m_Settings.wallColor;
        int size = 4;
        
        // Simple culling of floors to reduce overdraw
        if (meshName.find("floor") != std::string::npos || meshName.find("ceiling") != std::string::npos) {
            continue;
        }

        if (meshName.find("wall") != std::string::npos) {
            color = m_Settings.wallColor;
            size = 8; // Walls are chunky
        } 
        else if (meshName.find("door") != std::string::npos) {
            color = {200, 180, 50, 255}; 
            size = 6;
        }
        else if (meshName.find("chest") != std::string::npos) {
            color = m_Settings.chestColor;
            size = 6;
        }
        else {
            // General props
            color = m_Settings.propColor;
            size = 4;
        }

        int screenX, screenY;
        if (WorldToMinimap(t->x, t->y, camera->x, camera->y, screenX, screenY)) {
            renderer->DrawRect2D(screenX - size/2, screenY - size/2, size, size, color);
        }
    }

    // --- RENDER ENEMIES ---
    // Rendered after meshes so they appear on top
    auto& charView = registry->View<CharacterComponent>();
    for (auto& pair : charView) {
        Entity e = pair.first;
        if (e == playerEntity) continue;

        auto* t = registry->GetComponent<Transform3DComponent>(e);
        // Assuming CharacterComponent implies it's alive/movable
        if (!t) continue;

        int screenX, screenY;
        if (WorldToMinimap(t->x, t->y, camera->x, camera->y, screenX, screenY)) {
            int size = 6;
            renderer->DrawRect2D(screenX - size/2, screenY - size/2, size, size, m_Settings.enemyColor);
        }
    }
}

void Minimap::Render(GLRenderer* renderer, Registry* registry, Entity playerEntity,
                     Camera* camera, int screenWidth, int screenHeight) {
    if (!m_Visible || !renderer || !registry || !camera) return;

    // Safety check: Ensure minimap is within screen bounds (responsive layout)
    if (m_PosX + m_Width > screenWidth) m_PosX = screenWidth - m_Width - 10;
    if (m_PosY + m_Height > screenHeight) m_PosY = screenHeight - m_Height - 10;

    RenderBackground(renderer);
    
    // We pass the Camera position, not the Player Entity position, 
    // to ensure the map centers on what the user is seeing.
    RenderEntities(renderer, registry, playerEntity, camera);
    
    // Render Player Arrow on top of everything
    RenderPlayerIcon(renderer, camera->yaw);
}

} // namespace PixelsEngine