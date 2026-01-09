#pragma once
#include "ECS.h"
#include "Camera.h"
#include "GLRenderer.h"
#include "SpawnLocation.h"
#include <vector>

namespace PixelsEngine {

class TextRenderer;

class VisualSpawnEditor {
public:
    VisualSpawnEditor(Registry* registry, GLRenderer* renderer);
    ~VisualSpawnEditor();

    void Update(float dt, Camera* camera, int screenWidth = 1280, int screenHeight = 720);
    void Render(GLRenderer* renderer, TextRenderer* textRenderer, Camera* camera, int screenWidth, int screenHeight);
    
    bool IsActive() const { return m_IsActive; }
    void Activate(bool active = true) { m_IsActive = active; }
    
    // Get the spawn locations
    std::vector<SpawnLocation> GetSpawnLocations() const;
    
    // Set spawn locations to edit
    void SetSpawnLocations(const std::vector<SpawnLocation>& locations);
    
    // Clear all spawns
    void ClearAllSpawns();

private:
    Registry* m_Registry;
    GLRenderer* m_Renderer;
    bool m_IsActive = false;
    
    // Visual representation of spawns
    std::vector<Entity> m_SpawnEntities;
    std::vector<SpawnLocation> m_SpawnLocations;
    
    // Editing state
    int m_SelectedSpawnIndex = -1;
    bool m_IsDragging = false;
    float m_DragOffsetX = 0.0f;
    float m_DragOffsetY = 0.0f;
    
    // Spawn mode (E + click to place)
    bool m_SpawnModeActive = false;
    // Patrol edit mode (P + click to add waypoint to selected)
    bool m_PatrolEditMode = false;
    
    Entity m_PreviewSkeleton = INVALID_ENTITY;
    bool m_PreviewIsTransparent = false;
    float m_PreviewX = 0.0f;
    float m_PreviewY = 0.0f;
    
    // Free camera state
    float m_CameraX = 36.0f;
    float m_CameraY = -20.0f;
    float m_CameraHeight = 5.0f;
    float m_CameraYaw = 0.0f;
    float m_CameraPitch = -0.3f;
    
    // Helper functions
    void CreateSpawnEntities();
    void UpdateSpawnEntities();
    void HandleMouseInput(Camera* camera, int screenWidth, int screenHeight);
    void HandleKeyboardInput(float dt);
    void UpdatePreviewSkeleton(Camera* camera, int screenWidth, int screenHeight);
    bool RaycastToGround(Camera* camera, int mouseX, int mouseY, int screenWidth, int screenHeight, float& outX, float& outY);
    void RenderSpawnPoints(GLRenderer* renderer, Camera* camera);
    void RenderPatrolPaths(GLRenderer* renderer, Camera* camera);
    void RenderSelectedHighlight(GLRenderer* renderer, Camera* camera, int screenWidth, int screenHeight);
};

} // namespace PixelsEngine
