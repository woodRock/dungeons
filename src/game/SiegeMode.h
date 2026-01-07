#pragma once
#include "../engine/Camera.h"
#include "../engine/CameraController.h"
#include "../engine/InputController.h"
#include "../engine/Components.h"
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/AudioManager.h"
#include "../engine/AssetManager.h"
#include "../engine/CharacterFactory.h"
#include "../engine/MapLoader.h"
#include <memory>

namespace PixelsEngine {
class TextRenderer;
}

class SiegeMode {
public:
  SiegeMode(PixelsEngine::Registry *registry,
            PixelsEngine::GLRenderer *renderer);
  ~SiegeMode();

  void Init(PixelsEngine::Camera *camera, PixelsEngine::Entity &playerEntity);
  void Update(float dt, PixelsEngine::Entity playerEntity,
              float tunerDist = 3.5f, float tunerShoulder = 0.8f,
              float tunerHeight = 1.8f);
  void RenderUI(PixelsEngine::GLRenderer *ren, PixelsEngine::TextRenderer *tr,
                int w, int h);
  
  // Getters
  float GetCameraYaw() const;

private:
  PixelsEngine::Registry *m_Registry;
  PixelsEngine::GLRenderer *m_Renderer;
  PixelsEngine::Camera *m_Camera = nullptr;
  std::unique_ptr<PixelsEngine::ThirdPersonCamera> m_CameraController;
  std::unique_ptr<PixelsEngine::ThirdPersonMovementController> m_MovementController;

  float m_HitmarkerTimer = 0.0f;
  float m_AttackTimer = 0.0f;

  struct {
    bool active = false;
    float x = 0, y = 0, z = 0;
  } m_Cursor;

  // Audio manager
  std::unique_ptr<AudioManager> m_AudioManager;
  Mix_Chunk *m_SfxShoot = nullptr;
  Mix_Chunk *m_SfxHit = nullptr;
  Mix_Chunk *m_SfxSwordHit = nullptr;
  Mix_Chunk *m_SfxSwordMiss = nullptr;

  void SpawnEnemy(const std::string &mesh, float x, float y,
                  PixelsEngine::CharacterComponent::Type type);
  void RaycastCursor();
};
