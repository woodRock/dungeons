#pragma once
#include "../engine/Camera.h"
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/MapLoader.h"
#include "../engine/CameraController.h"
#include "../engine/InputController.h"
#include "../engine/AudioManager.h"
#include "../engine/AssetManager.h"
#include "../engine/CharacterFactory.h"
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>

namespace PixelsEngine {

class TextRenderer;

class DungeonMode {
public:
    DungeonMode(Registry* registry, GLRenderer* renderer);
    ~DungeonMode();

    void Init(Camera* camera, Entity& playerEntity);
    void Update(float dt, Entity& playerEntity);
    void RenderUI(GLRenderer* renderer, TextRenderer* textRenderer, int width, int height);

    void StartDungeon(const std::vector<std::string>& levels);
    void LoadDungeonFile(const std::string& filename);
    void NextLevel();
    
    bool IsActive() const { return m_Active; }
    void SetActive(bool active) { m_Active = active; }
    Entity GetPlayerEntity() const { return m_PlayerEntity; }

private:
    void LoadLevel(const std::string& levelName);
    void CheckLevelCompletion();

    Registry* m_Registry;
    GLRenderer* m_Renderer;
    Camera* m_Camera = nullptr;
    Entity m_PlayerEntity = PixelsEngine::INVALID_ENTITY;
    
    std::unique_ptr<TopDownCamera> m_CameraController;
    std::unique_ptr<TopDownMovementController> m_MovementController;
    std::unique_ptr<AudioManager> m_AudioManager;
    
    bool m_Active = false;
    bool m_LevelComplete = false;
    bool m_DoorUnlocked = false;
    bool m_FirstFrame = true;
    int m_CurrentLevelIdx = 0;
    std::vector<std::string> m_LevelList;
    
    std::string m_StatusMessage;
    float m_MessageTimer = 0.0f;
    
    Mix_Chunk* m_SfxShoot = nullptr;
    Mix_Chunk* m_SfxHit = nullptr;
    Mix_Chunk* m_SfxDoor = nullptr;
    
    float m_AttackTimer = 0.0f;
};

} // namespace PixelsEngine
