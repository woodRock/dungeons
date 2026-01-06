#pragma once
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/Camera.h"

class SiegeMode {
public:
    SiegeMode(PixelsEngine::Registry* registry, PixelsEngine::GLRenderer* renderer);
    ~SiegeMode();

    void Init(PixelsEngine::Camera* camera, PixelsEngine::Entity& playerEntity);
    void Update(float dt, PixelsEngine::Entity playerEntity);

private:
    PixelsEngine::Registry* m_Registry;
    PixelsEngine::GLRenderer* m_Renderer;
};
