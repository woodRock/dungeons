#pragma once
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/Camera.h"
#include <string>

class BattleMode {
public:
    BattleMode(PixelsEngine::Registry* registry, PixelsEngine::GLRenderer* renderer);
    void Init(PixelsEngine::Camera* camera, PixelsEngine::Entity& playerEntity);
    void Update(float dt, PixelsEngine::Entity playerEntity);

private:
    PixelsEngine::Registry* m_Registry;
    PixelsEngine::GLRenderer* m_Renderer;
    
    void SpawnCharacter(const std::string& mesh, float x, float y);
};
