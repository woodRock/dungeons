#include "Minimap.h"
#include "Components.h"
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
    if (zoom < 0.5f) zoom = 0.5f;
    if (zoom > 5.0f) zoom = 5.0f;
    m_Zoom = zoom;
}

void Minimap::SetVisible(bool visible) {
    m_Visible = visible;
}

// Coordinate Conversion: World Space -> Minimap Screen Space
// Applies: Translation -> Rotation -> Mirroring -> Scaling -> Bounds Check
bool Minimap::WorldToMinimap(float worldX, float worldY, float playerX, float playerY, float playerAngle, float& outX, float& outY) {
    float centerX = (float)m_PosX + m_Width / 2.0f;
    float centerY = (float)m_PosY + m_Height / 2.0f;
    
    float scale = m_Settings.baseScale * m_Zoom;

    // 1. Get position relative to player
    float dx = worldX - playerX;
    float dy = worldY - playerY;

    // 2. Rotation Logic (Forward-Up View)
    // Rotate world opposite to player rotation.
    // Offset by -90 degrees (-PI/2) because screen "Up" is -Y in SDL.
    float theta = -playerAngle - (M_PI / 2.0f); 

    float cosA = cos(theta);
    float sinA = sin(theta);

    // Standard 2D rotation matrix
    float rotX = dx * cosA - dy * sinA;
    float rotY = dx * sinA + dy * cosA;

    // 3. Mirror & Scale
    // Negate X to mirror horizontally (fix for coordinate system mismatch)
    float screenRelX = -(rotX * scale); 
    float screenRelY = rotY * scale;

    // 4. Bounds Checking (Clipping)
    float margin = 2.0f; 
    float halfW = (m_Width / 2.0f) - margin;
    float halfH = (m_Height / 2.0f) - margin;

    if (abs(screenRelX) > halfW || abs(screenRelY) > halfH) {
        return false;
    }

    outX = centerX + screenRelX;
    outY = centerY + screenRelY;
    return true;
}

void Minimap::RenderBackground(GLRenderer* renderer) {
    // Outer glow/shadow
    SDL_Color glow = m_Settings.borderColor;
    glow.a = 60;
    renderer->DrawRect2D((float)m_PosX - 4.0f, (float)m_PosY - 4.0f, (float)m_Width + 8.0f, (float)m_Height + 8.0f, glow);

    // Background fill
    renderer->DrawRect2D((float)m_PosX, (float)m_PosY, (float)m_Width, (float)m_Height, m_Settings.bgColor);

    // Border
    float t = (float)m_Settings.borderThickness;
    SDL_Color c = m_Settings.borderColor;

    renderer->DrawRect2D((float)m_PosX, (float)m_PosY, (float)m_Width, t, c); // Top
    renderer->DrawRect2D((float)m_PosX, (float)m_PosY + m_Height - t, (float)m_Width, t, c); // Bottom
    renderer->DrawRect2D((float)m_PosX, (float)m_PosY, t, (float)m_Height, c); // Left
    renderer->DrawRect2D((float)m_PosX + m_Width - t, (float)m_PosY, t, (float)m_Height, c); // Right
}

void Minimap::RenderPlayerIcon(GLRenderer* renderer) {
    float centerX = (float)m_PosX + m_Width / 2.0f;
    float centerY = (float)m_PosY + m_Height / 2.0f;

    SDL_Color color = m_Settings.playerColor;

    // A more modern triangular arrow icon facing UP
    // We use rotated rectangles to build it
    
    // Main body
    renderer->DrawRotatedRect2D(centerX, centerY + 2.0f, 6.0f, 14.0f, 0.0f, color);
    
    // Left wing
    renderer->DrawRotatedRect2D(centerX - 4.0f, centerY + 6.0f, 4.0f, 10.0f, 0.6f, color);
    
    // Right wing
    renderer->DrawRotatedRect2D(centerX + 4.0f, centerY + 6.0f, 4.0f, 10.0f, -0.6f, color);
    
    // Tip glow/point
    SDL_Color tipColor = {255, 255, 255, 200};
    renderer->DrawRect2D(centerX - 1.0f, centerY - 8.0f, 2.0f, 4.0f, tipColor);
}

void Minimap::RenderEntities(GLRenderer* renderer, Registry* registry, Entity playerEntity, Camera* camera) {
    if (!camera) return;

    float scale = m_Settings.baseScale * m_Zoom;
    float mapRotation = -camera->yaw - (M_PI / 2.0f);

    // 1. Render Environment (Meshes)
    auto& meshView = registry->View<MeshComponent>();
    
    for (auto& pair : meshView) {
        Entity e = pair.first;
        if (e == playerEntity) continue;

        auto* t = registry->GetComponent<Transform3DComponent>(e);
        auto* mesh = registry->GetComponent<MeshComponent>(e);
        
        if (!t || !mesh) continue;

        // Categorize mesh based on name
        std::string meshName = mesh->meshName;
        std::transform(meshName.begin(), meshName.end(), meshName.begin(), ::tolower);

        SDL_Color color = m_Settings.propColor;
        
        // Skip floors/ceilings to avoid clutter
        if (meshName.find("floor") != std::string::npos || meshName.find("ceiling") != std::string::npos) {
            continue;
        }

        float screenX, screenY;
        if (!WorldToMinimap(t->x, t->y, camera->x, camera->y, camera->yaw, screenX, screenY)) {
            continue;
        }

        // Structural elements
        if (meshName.find("door") != std::string::npos) {
            color = m_Settings.doorColor; 
            renderer->DrawRotatedRect2D(screenX, screenY, 4.2f * scale, 1.0f * scale, -t->rot - mapRotation, color);
        } else if (meshName.find("corner") != std::string::npos) {
            color = m_Settings.wallColor;
            float legLen = 6.0f; // Extra long to ensure overlap at all junction types
            float thickness = 1.2f;
            float angle = -t->rot - mapRotation;
            
            // Draw two legs to form a proper L-corner
            // Leg 1: horizontal (relative to asset rotation)
            float offX1 = cos(angle) * (legLen * 0.25f * scale);
            float offY1 = sin(angle) * (legLen * 0.25f * scale);
            renderer->DrawRotatedRect2D(screenX + offX1, screenY + offY1, 
                                        legLen * 0.5f * scale, thickness * scale, angle, color);
            
            // Leg 2: vertical (relative to asset rotation)
            float angle2 = angle + M_PI * 0.5f;
            float offX2 = cos(angle2) * (legLen * 0.25f * scale);
            float offY2 = sin(angle2) * (legLen * 0.25f * scale);
            renderer->DrawRotatedRect2D(screenX + offX2, screenY + offY2, 
                                        thickness * scale, legLen * 0.5f * scale, angle, color);
            continue; 
        } else if (meshName.find("wall") != std::string::npos) {
            color = m_Settings.wallColor;
            renderer->DrawRotatedRect2D(screenX, screenY, 4.2f * scale, 1.2f * scale, -t->rot - mapRotation, color);
        } else if (meshName.find("chest") != std::string::npos) {
            color = m_Settings.chestColor;
            float size = 1.5f * scale;
            renderer->DrawRect2D(screenX - size/2.0f, screenY - size/2.0f, size, size, color);
        } else {
            // Props
            float size = 0.5f * scale;
            renderer->DrawRect2D(screenX - size/2.0f, screenY - size/2.0f, size, size, color);
        }
    }

    // 2. Render Enemies (On top of environment)
    auto& charView = registry->View<CharacterComponent>();
    for (auto& pair : charView) {
        Entity e = pair.first;
        if (e == playerEntity) continue;
        
        auto* t = registry->GetComponent<Transform3DComponent>(e);
        if (!t) continue;

        float screenX, screenY;
        if (WorldToMinimap(t->x, t->y, camera->x, camera->y, camera->yaw, screenX, screenY)) {
            float size = 8.0f;
            renderer->DrawRect2D(screenX - size/2.0f, screenY - size/2.0f, size, size, m_Settings.enemyColor);
        }
    }
}

void Minimap::Render(GLRenderer* renderer, Registry* registry, Entity playerEntity,
                     Camera* camera, int screenWidth, int screenHeight) {
    if (!m_Visible || !renderer || !registry || !camera) return;

    // Responsive positioning: Ensure map stays within window bounds if resized
    if (m_PosX + m_Width > screenWidth) m_PosX = screenWidth - m_Width - 10;
    if (m_PosY + m_Height > screenHeight) m_PosY = screenHeight - m_Height - 10;

    RenderBackground(renderer);
    RenderEntities(renderer, registry, playerEntity, camera);
    RenderPlayerIcon(renderer);
}

} // namespace PixelsEngine