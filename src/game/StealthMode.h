#pragma once
#include "../engine/Camera.h"
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/MapLoader.h"
#include "../engine/Skybox.h"
#include "../engine/InputController.h"
#include "../engine/CameraController.h"
#include "../engine/Console.h"
#include "../engine/StealthSpawnEditor.h"
#include "../engine/VisualSpawnEditor.h"
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>

namespace PixelsEngine {

class TextRenderer;

// Guard AI state
enum class GuardState {
    IDLE,
    SEARCHING,
    ALERT,
    ATTACKING,
    DEAD
};

// Hide spot component
struct HideSpot {
    float x, y, z;
    float radius;
    bool occupied = false;
};

class StealthMode {
public:
    StealthMode(Registry* registry, GLRenderer* renderer);
    ~StealthMode();

    void Init(Camera* camera, Entity& playerEntity);
    void Update(float dt, Entity& playerEntity);
    void RenderUI(GLRenderer* renderer, TextRenderer* textRenderer, int width, int height);

    bool IsActive() const { return m_Active; }
    void SetActive(bool active) { m_Active = active; }
    Entity GetPlayerEntity() const { return m_PlayerEntity; }

private:
    void LoadLevel(const std::string& mapFile);
    void UpdateGuardAI(float dt);
    void CheckPlayerDetection();
    void CheckTakedowns();
    void CheckLineOfSight(Entity guardEntity, Entity playerEntity, bool& canSee);
    bool IsPlayerInHideSpot();
    float GetPlayerVisibility();
    void UpdatePlayerAnimation(float speed, bool isGrounded);
    void SpawnEnemies();
    void RenderGuardLineOfSight();
    void RenderMinimap(GLRenderer* renderer, TextRenderer* textRenderer, int screenWidth, int screenHeight);
    
    // Collision detection helper
    bool RayHitsCollider(float startX, float startY, float endX, float endY, float& outDistance);
    
    Registry* m_Registry;
    GLRenderer* m_Renderer;
    Camera* m_Camera = nullptr;
    Entity m_PlayerEntity = PixelsEngine::INVALID_ENTITY;
    std::unique_ptr<ThirdPersonCamera> m_CameraController;
    std::unique_ptr<ThirdPersonMovementController> m_MovementController;
    std::unique_ptr<Skybox> m_Skybox;
    std::unique_ptr<Console> m_Console;
    std::unique_ptr<StealthSpawnEditor> m_SpawnEditor;
    std::unique_ptr<VisualSpawnEditor> m_VisualSpawnEditor;
    
    // Console state tracking
    bool m_ConsoleWasOpen = false;
    
    // Stealth mode data
    std::vector<Entity> m_Guards;
    std::vector<HideSpot> m_HideSpots;
    std::vector<GuardState> m_GuardStates;
    std::vector<float> m_GuardAlertLevels;
    std::vector<float> m_GuardSearchTimers;
    std::vector<std::pair<float, float>> m_GuardSearchTargets;
    
    // Player sneak state
    bool m_PlayerSneaking = false;
    
    float m_PlayerAlertLevel = 0.0f;  // 0 = undetected, 1 = full alert
    bool m_Detected = false;
    bool m_Active = false;
    
    // Stealth mechanics
    float m_MaxDetectionDistance = 20.0f;
    float m_ViewAngle = 120.0f;  // degrees
    float m_AlertDecayRate = 0.5f;  // per second
    float m_SneakVisibilityMultiplier = 0.3f;  // Player is 70% less visible when sneaking
    
    // Mission State
    Entity m_ObjectiveEntity = PixelsEngine::INVALID_ENTITY;
    bool m_MissionComplete = false;
    
    // Helper methods
    void SpawnObjective();
    void CheckObjectives();
    void RenderObjectiveMarker(GLRenderer* renderer, TextRenderer* textRenderer, int screenWidth, int screenHeight);

    // Screen dimensions (cached for spawn editor)
    int m_ScreenWidth = 1280;
    int m_ScreenHeight = 720;
    
    // Minimap configuration
    int m_MinimapX = 10;
    int m_MinimapY = 10;
    int m_MinimapWidth = 250;
    int m_MinimapHeight = 200;
    float m_MinimapScale = 2.0f;  // pixels per world unit
};

} // namespace PixelsEngine
