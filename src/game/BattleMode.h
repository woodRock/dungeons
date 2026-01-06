#pragma once
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/Camera.h"
#include "../engine/Components.h" // For BattleUnitComponent
#include <string>
#include <vector>

class BattleMode {
public:
    BattleMode(PixelsEngine::Registry* registry, PixelsEngine::GLRenderer* renderer);
    void Init(PixelsEngine::Camera* camera, PixelsEngine::Entity& playerEntity);
    void Update(float dt, PixelsEngine::Entity playerEntity);

private:
    PixelsEngine::Registry* m_Registry;
    PixelsEngine::GLRenderer* m_Renderer;
    PixelsEngine::Camera* m_Camera = nullptr;
    
    enum State { Setup, PlayerTurn, EnemyTurn, Moving, Attacking, Victory, Defeat };
    State m_State = Setup;
    
    std::vector<PixelsEngine::Entity> m_TurnOrder;
    int m_CurrentTurnIndex = 0;
    
    struct {
        bool active = false;
        float x = 0, y = 0, z = 0;
        PixelsEngine::Entity hoveredEntity = -1;
    } m_Cursor;
    
    struct {
        float startX, startY;
        float targetX, targetY;
        PixelsEngine::Entity actor = -1;
        float timer = 0.0f;
        float duration = 0.0f;
    } m_AnimState;

    void LoadMap(const std::string& path);
    void SpawnCharacter(const std::string& mesh, float x, float y, PixelsEngine::BattleUnitComponent::Team team);
    
    void StartBattle();
    void NextTurn();
    void HandlePlayerInput();
    void UpdateAI(float dt);
    
    // Helpers
    void RaycastCursor();
    bool IsWalkable(float x, float y);
    float Distance(float x1, float y1, float x2, float y2);
};
