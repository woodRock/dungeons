#pragma once
#include "../engine/Camera.h"
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "CreativeMode.h"

class ExplorationMode {
public:
  ExplorationMode(PixelsEngine::Registry *registry,
                  PixelsEngine::GLRenderer *renderer,
                  PixelsEngine::CreativeMode *editor);
  void Init(PixelsEngine::Camera *camera, PixelsEngine::Entity &playerEntity);

private:
  PixelsEngine::Registry *m_Registry;
  PixelsEngine::GLRenderer *m_Renderer;
  PixelsEngine::CreativeMode *m_CreativeMode;
};
