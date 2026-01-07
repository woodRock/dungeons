#pragma once
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/Camera.h"
#include "../engine/Components.h" 
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>

namespace PixelsEngine { class TextRenderer; }

class BattleMode {
public:
    BattleMode(PixelsEngine::Registry* registry, PixelsEngine::GLRenderer* renderer);
    void Init(PixelsEngine::Camera* camera, PixelsEngine::Entity& playerEntity);
    void Update(float dt, PixelsEngine::Entity playerEntity);
    void RenderWorld(PixelsEngine::GLRenderer* renderer);
    void RenderUI(PixelsEngine::GLRenderer* renderer, PixelsEngine::TextRenderer* textRenderer, int w, int h);

private:
    PixelsEngine::Registry* m_Registry;
    PixelsEngine::GLRenderer* m_Renderer;
    PixelsEngine::Camera* m_Camera = nullptr;
    
    enum State { Setup, PlayerTurn, EnemyTurn, Moving, Attacking, Victory, Defeat };
    State m_State = Setup;
    
    enum ActionType { None, Move, Melee, Ranged, Dash, Jump, Sneak, Shove };
    ActionType m_SelectedAction = None;
    
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

    struct {
        PixelsEngine::Entity entity = -1;
        float startX, startY, startZ;
        float targetX, targetY, targetZ;
    } m_ActiveProjectile;

    // Audio
    Mix_Music* m_Music = nullptr;
    Mix_Chunk* m_SfxJump = nullptr;
    Mix_Chunk* m_SfxShoot = nullptr;
    Mix_Chunk* m_SfxBowHit = nullptr;
    Mix_Chunk* m_SfxSwordHit = nullptr;
    Mix_Chunk* m_SfxSwordMiss = nullptr;

    bool isMouseOverUI(int mx, int my, int w, int h);
    bool IsPositionOccupied(float x, float y, PixelsEngine::Entity ignoreEntity);
    void LoadMap(const std::string& path);
    void SpawnCharacter(const std::string& mesh, float x, float y, PixelsEngine::BattleUnitComponent::Team team);
    
    void StartBattle();
    void NextTurn();
    void HandlePlayerInput();
    void ExecuteAction(PixelsEngine::Entity actor, ActionType action, PixelsEngine::Entity target, float x, float y);
    void UpdateAI(float dt);
    void CheckVictory();
    
    void DrawActionBar(PixelsEngine::GLRenderer* ren, PixelsEngine::TextRenderer* tr, int w, int h);
    void DrawTurnOrder(PixelsEngine::GLRenderer* ren, PixelsEngine::TextRenderer* tr, int w, int h);
    
    void RaycastCursor();
};