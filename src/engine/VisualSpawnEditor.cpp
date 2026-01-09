#include "VisualSpawnEditor.h"
#include "Input.h"
#include "Components.h"
#include "CharacterFactory.h"
#include "TextRenderer.h"
#include "UIHelper.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>

namespace PixelsEngine {

VisualSpawnEditor::VisualSpawnEditor(Registry* registry, GLRenderer* renderer)
    : m_Registry(registry), m_Renderer(renderer) {
}

VisualSpawnEditor::~VisualSpawnEditor() {
    // Clean up spawn entities
    for (Entity e : m_SpawnEntities) {
        if (e != INVALID_ENTITY) {
            m_Registry->DestroyEntity(e);
        }
    }
    m_SpawnEntities.clear();
}

void VisualSpawnEditor::SetSpawnLocations(const std::vector<SpawnLocation>& locations) {
    m_SpawnLocations = locations;
    CreateSpawnEntities();
}

std::vector<SpawnLocation> VisualSpawnEditor::GetSpawnLocations() const {
    // Update from current entity positions
    std::vector<SpawnLocation> result;
    for (size_t i = 0; i < m_SpawnEntities.size(); ++i) {
        Entity e = m_SpawnEntities[i];
        auto* transform = m_Registry->GetComponent<Transform3DComponent>(e);
        if (transform) {
            SpawnLocation loc;
            loc.x = transform->x;
            loc.y = transform->y;
            loc.rotation = transform->rot;
            // Preserve waypoints from the internal state since they aren't on the entity in editor mode
            if (i < m_SpawnLocations.size()) {
                loc.waypoints = m_SpawnLocations[i].waypoints;
            }
            result.push_back(loc);
        }
    }
    return result;
}

void VisualSpawnEditor::ClearAllSpawns() {
    // Clean up old entities
    for (Entity e : m_SpawnEntities) {
        if (e != INVALID_ENTITY) {
            m_Registry->DestroyEntity(e);
        }
    }
    m_SpawnEntities.clear();
    m_SpawnLocations.clear();
    m_SelectedSpawnIndex = -1;
    m_PatrolEditMode = false;
    
    // Clean up preview if active
    if (m_PreviewSkeleton != INVALID_ENTITY) {
        m_Registry->DestroyEntity(m_PreviewSkeleton);
        m_PreviewSkeleton = INVALID_ENTITY;
        m_PreviewIsTransparent = false;
    }
}

void VisualSpawnEditor::CreateSpawnEntities() {
    // Clean up old entities
    for (Entity e : m_SpawnEntities) {
        if (e != INVALID_ENTITY) {
            m_Registry->DestroyEntity(e);
        }
    }
    m_SpawnEntities.clear();
    
    // Create skeleton entities at spawn locations
    for (const auto& loc : m_SpawnLocations) {
        Entity e = CharacterFactory::CreateSkeleton(m_Registry, m_Renderer, 
                                                     "skel_warrior", loc.x, loc.y, 0.5f);
        if (e != INVALID_ENTITY) {
            auto* transform = m_Registry->GetComponent<Transform3DComponent>(e);
            if (transform) {
                transform->rot = loc.rotation;
            }
            m_SpawnEntities.push_back(e);
        }
    }
    // Don't reset selection here if we are just refreshing
    // m_SelectedSpawnIndex = -1; 
}

void VisualSpawnEditor::UpdateSpawnEntities() {
    for (size_t i = 0; i < m_SpawnEntities.size(); ++i) {
        auto* transform = m_Registry->GetComponent<Transform3DComponent>(m_SpawnEntities[i]);
        if (transform) {
            transform->x = m_SpawnLocations[i].x;
            transform->y = m_SpawnLocations[i].y;
            // We don't update rotation here because dragging only affects position.
            // Rotation is updated via R key directly on entity.
        }
    }
}

bool VisualSpawnEditor::RaycastToGround(Camera* camera, int mouseX, int mouseY, int screenWidth, int screenHeight, float& outX, float& outY) {
    // Screen-space raycast: trace from camera through screen point to ground
    if (!camera) {
        return false;
    }
    
    // Normalize mouse to -1 to 1 range
    float normalizedX = (2.0f * mouseX) / screenWidth - 1.0f;
    float normalizedY = 1.0f - (2.0f * mouseY) / screenHeight;
    
    // Use camera direction vectors to calculate ray
    float dirX = cos(camera->yaw) * cos(camera->pitch);
    float dirY = sin(camera->yaw) * cos(camera->pitch);
    float dirZ = sin(camera->pitch);
    
    // Get camera right direction
    float rightX = cos(camera->yaw - M_PI / 2.0f);
    float rightY = sin(camera->yaw - M_PI / 2.0f);
    
    // Get camera up direction
    float upX = -sin(camera->pitch) * cos(camera->yaw);
    float upY = -sin(camera->pitch) * sin(camera->yaw);
    float upZ = cos(camera->pitch);
    
    // Calculate ray direction with perspective
    float rayX = dirX + (rightX * normalizedX * 0.5f) + (upX * normalizedY * 0.5f);
    float rayY = dirY + (rightY * normalizedX * 0.5f) + (upY * normalizedY * 0.5f);
    float rayZ = dirZ + (upZ * normalizedY * 0.5f);
    
    // Normalize ray
    float rayLen = sqrt(rayX*rayX + rayY*rayY + rayZ*rayZ);
    if (rayLen > 0.0001f) {
        rayX /= rayLen;
        rayY /= rayLen;
        rayZ /= rayLen;
    }
    
    // Raycast to ground plane (z=0) - simple plane intersection
    // Ray: P = camera + t*ray
    // Plane: z = 0
    // Solve: camera.z + t*rayZ = 0
    if (rayZ < -0.01f) {  // Ray is pointing downward
        float t = -camera->z / rayZ;
        if (t > 0) {
            outX = camera->x + t * rayX;
            outY = camera->y + t * rayY;
            return true;
        }
    }
    
    // Fallback: project ahead using camera forward direction
    float distance = 15.0f;
    outX = camera->x + dirX * distance;
    outY = camera->y + dirY * distance;
    return true;
}

void VisualSpawnEditor::UpdatePreviewSkeleton(Camera* camera, int screenWidth, int screenHeight) {
    if (!m_SpawnModeActive) {
        // Clean up preview if not in spawn mode
        if (m_PreviewSkeleton != INVALID_ENTITY) {
            m_Registry->DestroyEntity(m_PreviewSkeleton);
            m_PreviewSkeleton = INVALID_ENTITY;
            m_PreviewIsTransparent = false;
        }
        return;
    }
    
    // Get mouse world position (not screen center - use actual mouse position)
    float worldX, worldY;
    int mouseX, mouseY;
    Input::GetMousePosition(mouseX, mouseY);
    
    if (!RaycastToGround(camera, mouseX, mouseY, screenWidth, screenHeight, worldX, worldY)) {
        return;
    }
    
    m_PreviewX = worldX;
    m_PreviewY = worldY;
    
    // Create preview skeleton if needed
    if (m_PreviewSkeleton == INVALID_ENTITY) {
        m_PreviewSkeleton = CharacterFactory::CreateSkeleton(m_Registry, m_Renderer, 
                                                              "skel_warrior", worldX, worldY, 0.5f);
        m_PreviewIsTransparent = true;
        // Mark this as a preview for rendering purposes
    } else {
        // Update preview position
        auto* transform = m_Registry->GetComponent<Transform3DComponent>(m_PreviewSkeleton);
        if (transform) {
            transform->x = worldX;
            transform->y = worldY;
        }
    }
}

void VisualSpawnEditor::HandleMouseInput(Camera* camera, int screenWidth, int screenHeight) {
    int deltaX, deltaY;
    Input::GetMouseDelta(deltaX, deltaY);
    
    // Mouse look: update camera yaw/pitch (like Creative Mode)
    float mouseSensitivity = 0.003f;
    m_CameraYaw -= deltaX * mouseSensitivity;
    m_CameraPitch -= deltaY * mouseSensitivity;
    
    // Clamp pitch to prevent flipping
    if (m_CameraPitch > 1.5f) m_CameraPitch = 1.5f;
    if (m_CameraPitch < -1.5f) m_CameraPitch = -1.5f;
    
    // Get current mouse position for click-based actions
    int currMouseX, currMouseY;
    Input::GetMousePosition(currMouseX, currMouseY);
    Uint32 buttonState = SDL_GetMouseState(nullptr, nullptr);
    
    // Check if clicking on a spawn point (left click)
    if (Input::IsMouseButtonPressed(1)) {  // Left click
        // If in spawn mode (E held), place the new spawn
        if (m_SpawnModeActive) {
            // Place at preview position
            SpawnLocation loc;
            loc.x = m_PreviewX;
            loc.y = m_PreviewY;
            loc.rotation = 0.0f;
            m_SpawnLocations.push_back(loc);
            
            CreateSpawnEntities();
            m_SelectedSpawnIndex = m_SpawnLocations.size() - 1;
            m_SpawnModeActive = false;
            
            // Clean up preview skeleton
            if (m_PreviewSkeleton != INVALID_ENTITY) {
                m_Registry->DestroyEntity(m_PreviewSkeleton);
                m_PreviewSkeleton = INVALID_ENTITY;
                m_PreviewIsTransparent = false;
            }
            return;
        }
        
        float worldX, worldY;
        if (RaycastToGround(camera, currMouseX, currMouseY, screenWidth, screenHeight, worldX, worldY)) {
            
            // Handle Patrol Point Addition
            if (m_PatrolEditMode && m_SelectedSpawnIndex >= 0 && m_SelectedSpawnIndex < (int)m_SpawnLocations.size()) {
                // Add current mouse position as a waypoint
                m_SpawnLocations[m_SelectedSpawnIndex].waypoints.push_back({worldX, worldY});
                return; // Consume click
            }
            
            // Find nearest spawn to click with selection radius
            float closestDist = 3.5f;  // Increased selection radius
            int closestIdx = -1;
            
            for (size_t i = 0; i < m_SpawnLocations.size(); ++i) {
                float dx = m_SpawnLocations[i].x - worldX;
                float dy = m_SpawnLocations[i].y - worldY;
                float dist = sqrt(dx * dx + dy * dy);
                
                if (dist < closestDist) {
                    closestDist = dist;
                    closestIdx = i;
                }
            }
            
            if (closestIdx >= 0) {
                m_SelectedSpawnIndex = closestIdx;
                m_IsDragging = true;
                m_DragOffsetX = m_SpawnLocations[closestIdx].x - worldX;
                m_DragOffsetY = m_SpawnLocations[closestIdx].y - worldY;
            } else {
                m_SelectedSpawnIndex = -1;
                m_IsDragging = false;
            }
        }
    }
    
    // Handle dragging
    if (m_IsDragging && m_SelectedSpawnIndex >= 0) {
        if (Input::IsMouseButtonDown(1)) {
            float worldX, worldY;
            if (RaycastToGround(camera, currMouseX, currMouseY, screenWidth, screenHeight, worldX, worldY)) {
                m_SpawnLocations[m_SelectedSpawnIndex].x = worldX + m_DragOffsetX;
                m_SpawnLocations[m_SelectedSpawnIndex].y = worldY + m_DragOffsetY;
                UpdateSpawnEntities();
            }
        } else {
            m_IsDragging = false;
        }
    }
}

void VisualSpawnEditor::HandleKeyboardInput(float dt) {
    float moveSpeed = 10.0f;  // units per second
    
    // Get keyboard state directly from SDL
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    
    bool wDown = keyState[SDL_SCANCODE_W];
    bool sDown = keyState[SDL_SCANCODE_S];
    bool aDown = keyState[SDL_SCANCODE_A];
    bool dDown = keyState[SDL_SCANCODE_D];
    bool shiftDown = keyState[SDL_SCANCODE_LSHIFT] || keyState[SDL_SCANCODE_RSHIFT];
    bool spaceDown = keyState[SDL_SCANCODE_SPACE];
    bool eDown = keyState[SDL_SCANCODE_E];
    
    // Calculate camera direction vectors (like Creative Mode)
    float dirX = cos(m_CameraYaw) * cos(m_CameraPitch);
    float dirY = sin(m_CameraYaw) * cos(m_CameraPitch);
    float dirZ = sin(m_CameraPitch);
    
    // Calculate camera right direction
    float rightX = cos(m_CameraYaw - M_PI / 2.0f);
    float rightY = sin(m_CameraYaw - M_PI / 2.0f);
    
    // WASD directional movement (based on camera orientation)
    if (wDown) {
        m_CameraX += dirX * moveSpeed * dt;
        m_CameraY += dirY * moveSpeed * dt;
        m_CameraHeight += dirZ * moveSpeed * dt;
    }
    if (sDown) {
        m_CameraX -= dirX * moveSpeed * dt;
        m_CameraY -= dirY * moveSpeed * dt;
        m_CameraHeight -= dirZ * moveSpeed * dt;
    }
    if (aDown) {
        m_CameraX -= rightX * moveSpeed * dt;
        m_CameraY -= rightY * moveSpeed * dt;
    }
    if (dDown) {
        m_CameraX += rightX * moveSpeed * dt;
        m_CameraY += rightY * moveSpeed * dt;
    }
    
    // SPACE/SHIFT for vertical movement
    if (spaceDown) {
        m_CameraHeight += moveSpeed * dt;
    }
    if (shiftDown) {
        m_CameraHeight -= moveSpeed * dt;
    }
    
    // R to rotate selected spawn (only if not in spawn mode)
    static bool rWasDown = false;
    bool rIsDown = keyState[SDL_SCANCODE_R];
    
    if (rIsDown && !rWasDown && m_SelectedSpawnIndex >= 0 && !m_SpawnModeActive) {
        auto* transform = m_Registry->GetComponent<Transform3DComponent>(m_SpawnEntities[m_SelectedSpawnIndex]);
        if (transform) {
            transform->rot += M_PI / 2.0f;
            if (transform->rot > 2 * M_PI) {
                transform->rot -= 2 * M_PI;
            }
            m_SpawnLocations[m_SelectedSpawnIndex].rotation = transform->rot;
        }
    }
    rWasDown = rIsDown;
    
    // Delete key to remove selected spawn (only if not in spawn mode)
    if (keyState[SDL_SCANCODE_DELETE] && m_SelectedSpawnIndex >= 0 && !m_SpawnModeActive) {
        m_SpawnLocations.erase(m_SpawnLocations.begin() + m_SelectedSpawnIndex);
        CreateSpawnEntities();
        m_SelectedSpawnIndex = -1;
    }
    
    // P to toggle Patrol Edit Mode
    static bool pWasDown = false;
    bool pIsDown = keyState[SDL_SCANCODE_P];
    if (pIsDown && !pWasDown && m_SelectedSpawnIndex >= 0 && !m_SpawnModeActive) {
        m_PatrolEditMode = !m_PatrolEditMode;
    }
    pWasDown = pIsDown;
    
    // Backspace to remove last waypoint
    static bool bkspWasDown = false;
    bool bkspIsDown = keyState[SDL_SCANCODE_BACKSPACE];
    if (bkspIsDown && !bkspWasDown && m_PatrolEditMode && m_SelectedSpawnIndex >= 0) {
        if (!m_SpawnLocations[m_SelectedSpawnIndex].waypoints.empty()) {
            m_SpawnLocations[m_SelectedSpawnIndex].waypoints.pop_back();
        }
    }
    bkspWasDown = bkspIsDown;
    
    // E key to enter spawn preview mode (E + click to place)
    if (eDown && !m_SpawnModeActive) {
        m_SpawnModeActive = true;
        m_PatrolEditMode = false; // Disable patrol mode
    } else if (!eDown && m_SpawnModeActive) {
        // E released - cancel spawn mode
        m_SpawnModeActive = false;
        if (m_PreviewSkeleton != INVALID_ENTITY) {
            m_Registry->DestroyEntity(m_PreviewSkeleton);
            m_PreviewSkeleton = INVALID_ENTITY;
        }
    }
}

void VisualSpawnEditor::RenderSpawnPoints(GLRenderer* renderer, Camera* camera) {
    // Render visual indicators for each spawn point
    // For now, we rely on the skeleton entities being rendered
}

void VisualSpawnEditor::RenderPatrolPaths(GLRenderer* renderer, Camera* camera) {
    if (!m_IsActive || !camera) return;
    
    // Only render for selected spawn or all? Let's do selected for clarity, or all faint + selected bright.
    
    for (size_t i = 0; i < m_SpawnLocations.size(); ++i) {
        const auto& loc = m_SpawnLocations[i];
        if (loc.waypoints.empty()) continue;
        
        bool isSelected = (i == (size_t)m_SelectedSpawnIndex);
        SDL_Color pathColor = isSelected ? SDL_Color{255, 255, 0, 255} : SDL_Color{100, 100, 100, 100};
        
        float startX = loc.x;
        float startY = loc.y;
        float prevX = startX;
        float prevY = startY;
        
        for (const auto& wp : loc.waypoints) {
            renderer->DrawLine(prevX, prevY, 0.5f, wp.first, wp.second, 0.5f, pathColor);
            
            // Draw a small box at the waypoint
            renderer->DrawRect2D(0, 0, 0, 0, {0,0,0,0}); // Dummy call
            // Since we don't have DrawRect3D, we can draw a small cross
            float s = 0.5f;
            renderer->DrawLine(wp.first - s, wp.second, 0.5f, wp.first + s, wp.second, 0.5f, pathColor);
            renderer->DrawLine(wp.first, wp.second - s, 0.5f, wp.first, wp.second + s, 0.5f, pathColor);
            
            prevX = wp.first;
            prevY = wp.second;
        }
        
        // Loop back to start
        renderer->DrawLine(prevX, prevY, 0.5f, startX, startY, 0.5f, {pathColor.r, pathColor.g, pathColor.b, 100});
    }
}

void VisualSpawnEditor::RenderSelectedHighlight(GLRenderer* renderer, Camera* camera, int screenWidth, int screenHeight) {
    // Draw white border highlight around selected skeleton
    if (m_SelectedSpawnIndex < 0 || m_SelectedSpawnIndex >= (int)m_SpawnEntities.size()) {
        return;
    }
    
    Entity selectedEntity = m_SpawnEntities[m_SelectedSpawnIndex];
    auto* transform = m_Registry->GetComponent<Transform3DComponent>(selectedEntity);
    
    if (!transform || !camera) {
        return;
    }
    
    // Draw a white screen border to indicate selection
    SDL_Color selectionColor = {255, 255, 255, 220};  // Bright white
    int borderThickness = 4;
    int borderMargin = 15;
    
    // Draw thick white border on screen edges
    renderer->DrawRect2D(borderMargin, borderMargin, screenWidth - 2*borderMargin, borderThickness, selectionColor);
    renderer->DrawRect2D(borderMargin, screenHeight - borderMargin - borderThickness, screenWidth - 2*borderMargin, borderThickness, selectionColor);
    renderer->DrawRect2D(borderMargin, borderMargin, borderThickness, screenHeight - 2*borderMargin, selectionColor);
    renderer->DrawRect2D(screenWidth - borderMargin - borderThickness, borderMargin, borderThickness, screenHeight - 2*borderMargin, selectionColor);
    
    // Also draw a selection label
    std::string selectionInfo = "Selected Skeleton #" + std::to_string(m_SelectedSpawnIndex);
    // This would be drawn by textRenderer in the Render method
}

void VisualSpawnEditor::Update(float dt, Camera* camera, int screenWidth, int screenHeight) {
    if (!m_IsActive) return;
    
    HandleKeyboardInput(dt);
    HandleMouseInput(camera, screenWidth, screenHeight);
    UpdatePreviewSkeleton(camera, screenWidth, screenHeight);
    
    // Update camera with new position and rotation
    if (camera) {
        camera->x = m_CameraX;
        camera->y = m_CameraY;
        camera->z = m_CameraHeight;
        camera->pitch = m_CameraPitch;
        camera->yaw = m_CameraYaw;
    }
}

void VisualSpawnEditor::Render(GLRenderer* renderer, TextRenderer* textRenderer, Camera* camera, int screenWidth, int screenHeight) {
    if (!m_IsActive) return;
    
    // Draw white border/highlight around selected skeleton
    RenderSelectedHighlight(renderer, camera, screenWidth, screenHeight);
    
    // Draw patrol paths
    RenderPatrolPaths(renderer, camera);
    
    // Draw UI overlay
    UIHelper::DrawPanel(renderer, 10, 10, 350, 200, {0, 0, 0, 200});
    
    textRenderer->RenderText(renderer, "SPAWN EDITOR", 20, 15, {0, 255, 100, 255});
    textRenderer->RenderText(renderer, "Mouse Look: Free Camera", 20, 35, {200, 200, 200, 255});
    textRenderer->RenderText(renderer, "WASD: Move | SPACE/SHIFT: Up/Down", 20, 50, {200, 200, 200, 255});
    
    if (m_SpawnModeActive) {
        textRenderer->RenderText(renderer, "E+CLICK: Place Skeleton | ESC: Cancel", 20, 65, {255, 200, 100, 255});
    } else {
        textRenderer->RenderText(renderer, "Click+Drag: Move | E: Spawn Preview", 20, 65, {200, 200, 200, 255});
    }
    
    textRenderer->RenderText(renderer, "R: Rotate | DEL: Remove", 20, 80, {200, 200, 200, 255});
    
    // Patrol Mode UI
    if (m_PatrolEditMode) {
        textRenderer->RenderText(renderer, "PATROL MODE ACTIVE", 20, 100, {255, 255, 0, 255});
        textRenderer->RenderText(renderer, "Click Ground: Add Waypoint", 20, 115, {255, 255, 0, 255});
        textRenderer->RenderText(renderer, "Backspace: Remove Last Waypoint", 20, 130, {255, 255, 0, 255});
    } else {
        textRenderer->RenderText(renderer, "P: Toggle Patrol Edit Mode", 20, 100, {150, 150, 150, 255});
    }
    
    std::string spawnCount = "Spawns: " + std::to_string(m_SpawnLocations.size());
    textRenderer->RenderText(renderer, spawnCount, 20, 155, {100, 200, 255, 255});
    
    if (m_SelectedSpawnIndex >= 0 && !m_SpawnModeActive) {
        std::string selected = "Selected: " + std::to_string(m_SelectedSpawnIndex);
        if (!m_SpawnLocations[m_SelectedSpawnIndex].waypoints.empty()) {
            selected += " (Waypoints: " + std::to_string(m_SpawnLocations[m_SelectedSpawnIndex].waypoints.size()) + ")";
        }
        textRenderer->RenderText(renderer, selected, 20, 170, {255, 255, 0, 255});
    }
    
    // Draw crosshair at screen center
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    int crosshairSize = 15;
    SDL_Color crosshairColor = {100, 200, 100, 200};
    
    // Horizontal line
    renderer->DrawRect2D(centerX - crosshairSize, centerY - 2, crosshairSize * 2, 4, crosshairColor);
    // Vertical line
    renderer->DrawRect2D(centerX - 2, centerY - crosshairSize, 4, crosshairSize * 2, crosshairColor);
    // Center dot
    renderer->DrawRect2D(centerX - 3, centerY - 3, 6, 6, {150, 255, 150, 255});
}

} // namespace PixelsEngine