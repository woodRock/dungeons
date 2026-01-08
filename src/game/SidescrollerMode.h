#pragma once
#include "../engine/Camera.h"
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/MapLoader.h"
#include "../engine/InputController.h"
#include "../engine/Skybox.h"
#include <SDL2/SDL.h>
#include <memory>
#include <string>

namespace PixelsEngine {

class TextRenderer;

class SidescrollerMode {
public:
    SidescrollerMode(Registry* registry, GLRenderer* renderer);
    ~SidescrollerMode();

    void Init(Camera* camera, Entity& playerEntity);
    void Update(float dt, Entity& playerEntity);
    void RenderUI(GLRenderer* renderer, TextRenderer* textRenderer, int width, int height);

    bool IsActive() const { return m_Active; }
    void SetActive(bool active) { m_Active = active; }
    Entity GetPlayerEntity() const { return m_PlayerEntity; }

private:
    void LoadLevel(const std::string& mapFile);
    
    Registry* m_Registry;
    GLRenderer* m_Renderer;
    Camera* m_Camera = nullptr;
    Entity m_PlayerEntity = PixelsEngine::INVALID_ENTITY;
    std::unique_ptr<TopDownMovementController> m_MovementController;
    std::unique_ptr<Skybox> m_Skybox;
    int m_AnimIdle = -1;
    int m_AnimRun = -1;
    int m_AnimJump = -1;
    
    bool m_Active = false;
};

} // namespace PixelsEngine
