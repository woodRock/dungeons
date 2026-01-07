#pragma once
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/Camera.h"
#include "../engine/Components.h"
#include <string>

class AnimationTestMode {
public:
    AnimationTestMode(PixelsEngine::Registry* registry, PixelsEngine::GLRenderer* renderer);
    void Init(PixelsEngine::Camera* camera);
    void Update(float dt);
    void RenderUI(PixelsEngine::GLRenderer* renderer, int w, int h);

private:
    PixelsEngine::Registry* m_Registry;
    PixelsEngine::GLRenderer* m_Renderer;
    PixelsEngine::Entity m_TestEntity;
    
    bool m_ShowBow = false;
    int m_CurrentAnim = 0;
};
