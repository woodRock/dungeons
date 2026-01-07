#include "DungeonsGame.h"
#include "../engine/Components.h"
#include "../engine/Input.h"

using namespace PixelsEngine;

void DungeonsGame::OnUpdate(float deltaTime) {
  if (m_State == GameState::MainMenu) {
    m_MenuCamAngle += 0.2f * deltaTime;
    HandleInputMenu();
    return;
  }

  if (m_State == GameState::Paused) {
    HandleInputPause();
    return;
  }

  // From here on, delegate to mode-specific update functions.
  switch (m_State) {
  case GameState::Creative: {
    // Handle Creative camera movement with WASD and mouse look
    HandleInputCreative(deltaTime);
    // Let the creative mode/editor own its loop for placement/UI
    m_CreativeMode.Update(deltaTime, m_PlayerEntity);
    if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
      m_State = GameState::Paused;
      SDL_SetRelativeMouseMode(SDL_FALSE);
      m_MenuSelection = 0;
    }
    break;
  }
  case GameState::Siege: {
    if (m_SiegeMode)
      m_SiegeMode->Update(deltaTime, m_PlayerEntity, m_TunerDist,
                          m_TunerShoulder, m_TunerHeight);
    UpdatePhysics(deltaTime);
    UpdateDoors(deltaTime);
    UpdateProjectiles(deltaTime);
    UpdateAnimations(deltaTime);
    if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
      m_State = GameState::Paused;
      SDL_SetRelativeMouseMode(SDL_FALSE);
      m_MenuSelection = 0;
    }
    break;
  }
  case GameState::Battle: {
    UpdateBattle(deltaTime);
    UpdatePhysics(deltaTime);
    UpdateDoors(deltaTime);
    UpdateProjectiles(deltaTime);
    UpdateAnimations(deltaTime);
    if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
      m_State = GameState::Paused;
      m_MenuSelection = 0;
    }
    break;
  }
  case GameState::Playing: {
    // Core playing loop is now expected to live inside a mode; no-op here.
    break;
  }
  default:
    break;
  }
}

void DungeonsGame::UpdateBattle(float dt) {
  if (m_BattleMode) {
    m_BattleMode->Update(dt, m_PlayerEntity);
  }
}

void DungeonsGame::UpdateSiege(float dt) {
  if (m_SiegeMode) {
    m_SiegeMode->Update(dt, m_PlayerEntity, m_TunerDist, m_TunerShoulder,
                        m_TunerHeight);
  }
}

void DungeonsGame::PlaySpatialSfx(Mix_Chunk *chunk, float x, float y, float z) {
  if (!chunk || !m_Camera)
    return;
  int channel = Mix_PlayChannel(-1, chunk, 0);
  if (channel == -1)
    return;

  float dx = x - m_Camera->x;
  float dy = y - m_Camera->y;
  float dist = sqrt(dx * dx + dy * dy);
  int volume = (int)(MIX_MAX_VOLUME * (1.0f - std::min(1.0f, dist / 20.0f)));
  Mix_Volume(channel, volume);

  float angle = atan2(dy, dx) - m_Camera->yaw;
  float pan = sin(angle);
  Uint8 left = (Uint8)(127 * (1.0f - pan));
  Uint8 right = (Uint8)(127 * (1.0f + pan));
  Mix_SetPanning(channel, left, right);
}

void DungeonsGame::UpdateAnimations(float dt) {

  // 1. Procedural Animations (Bobbing)

  auto &proceduralView = m_Registry.View<ProceduralAnimationComponent>();

  for (auto &pair : proceduralView) {

    Entity e = pair.first;

    auto &anim = pair.second;

    auto *t = m_Registry.GetComponent<Transform3DComponent>(e);

    if (t) {

      anim.timeOffset += dt;

      // Bobbing (Z axis)

      t->z = anim.baseZ + sin(anim.timeOffset * anim.bobSpeed) * anim.bobAmount;

      // Subtle sway (Pitch)

      t->pitch = sin(anim.timeOffset * anim.swaySpeed) * anim.swayAmount;
    }
  }

  // 2. Skeletal Animations

  auto &skeletalView = m_Registry.View<SkeletalAnimationComponent>();

  for (auto &pair : skeletalView) {

    Entity e = pair.first;

    auto &anim = pair.second;

    auto *mesh = m_Registry.GetComponent<MeshComponent>(e);

    if (mesh) {

      RenderMesh *rm = m_GLRenderer.GetRenderMesh(mesh->meshName);

      if (rm && rm->isSkinned) {

        anim.currentTime += dt * anim.speed;

        m_GLRenderer.UpdateSkinnedMesh(*rm, anim.animationIndex,
                                       anim.currentTime);
      }
    }
  }
}
