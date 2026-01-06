#pragma once
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/Camera.h"
#include "DungeonEditor.h"

class ExplorationMode {
public:
    ExplorationMode(PixelsEngine::Registry* registry, PixelsEngine::GLRenderer* renderer, PixelsEngine::DungeonEditor* editor);
    void Init(PixelsEngine::Camera* camera, PixelsEngine::Entity& playerEntity);

private:
    PixelsEngine::Registry* m_Registry;
    PixelsEngine::GLRenderer* m_Renderer;
    PixelsEngine::DungeonEditor* m_Editor;
};
