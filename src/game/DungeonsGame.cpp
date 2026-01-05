#include "DungeonsGame.h"
#include "../engine/Components.h"
#include "../engine/Input.h"

using namespace PixelsEngine;

void DungeonsGame::OnUpdate(float deltaTime) {
  if (m_State == GameState::MainMenu) {
    m_MenuCamAngle += 0.2f * deltaTime;
    HandleInputMenu();
  }
  else if (m_State == GameState::Playing) {
    if (!m_GameFinished)
      m_RunTimer += deltaTime;

    auto *phys = m_Registry.GetComponent<PhysicsComponent>(m_PlayerEntity);
    auto *weapon = m_Registry.GetComponent<WeaponComponent>(m_PlayerEntity);

    m_TimeScale = 1.0f;
    if (phys && !phys->isGrounded && weapon && weapon->isDrawing) {
      m_TimeScale = 0.3f;
    }

    float dt = deltaTime * m_TimeScale;
    HandleInputGameplay(dt);
    UpdatePhysics(dt);
    UpdateProjectiles(dt);

    if (m_HitmarkerTimer > 0)
      m_HitmarkerTimer -= deltaTime;

    float targetRoll = 0.0f;
    if (phys) {
      if (Input::IsKeyDown(SDL_SCANCODE_A)) targetRoll = -2.0f;
      if (Input::IsKeyDown(SDL_SCANCODE_D)) targetRoll = 2.0f;
    }
    m_CameraRoll = m_CameraRoll * 0.9f + targetRoll * 0.1f;

    if (!m_GameFinished && m_TargetsDestroyed >= m_TotalTargets && m_TotalTargets > 0) {
      m_GameFinished = true;
    }

    if (m_ShakeTimer > 0) {
      m_ShakeTimer -= deltaTime;
      if (m_ShakeTimer < 0) m_ShakeTimer = 0;
    } else {
      m_ShakeIntensity = 0;
    }

    // Particle System
    auto &particles = m_Registry.View<ParticleComponent>();
    std::vector<Entity> deadParticles;
    for (auto &pair : particles) {
      Entity e = pair.first;
      auto &p = pair.second;
      auto *t = m_Registry.GetComponent<Transform3DComponent>(e);
      if (t) {
        t->x += p.vx * dt;
        t->y += p.vy * dt;
        t->z += p.vz * dt;
        p.vz -= 9.8f * dt;
        p.life -= dt;
        if (p.life <= 0) deadParticles.push_back(e);
      }
    }
    for (auto e : deadParticles) m_Registry.DestroyEntity(e);

    // Camera Sync
    auto *t = m_Registry.GetComponent<Transform3DComponent>(m_PlayerEntity);
    if (t) {
      m_Camera->x = t->x;
      m_Camera->y = t->y;
      m_Camera->z = t->z;
      m_Camera->yaw = t->rot;
      m_Camera->pitch = t->pitch;
    }
  }
  else if (m_State == GameState::Creative) {
    float dt = deltaTime;
    bool inventoryOpen = m_Editor.IsInventoryOpen();

    if (!inventoryOpen) {
      HandleInputGameplay(dt);
      UpdatePhysics(dt);
      if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
          m_State = GameState::Paused;
          SDL_SetRelativeMouseMode(SDL_FALSE);
          m_MenuSelection = 0;
      }
    }
    
    ActionResult res = m_Editor.Update(dt, *m_Camera);
    if (res != ActionResult::None) {
        if (res == ActionResult::Placed || res == ActionResult::Broken) {
            if (m_SfxHit) Mix_PlayChannel(-1, m_SfxHit, 0);
        } else if (res == ActionResult::InteractedDoor) {
            if (m_SfxDoor) Mix_PlayChannel(-1, m_SfxDoor, 0);
        } else if (res == ActionResult::InteractedChest) {
            if (m_SfxChestOpen) Mix_PlayChannel(-1, m_SfxChestOpen, 0);
        }
    }

    auto *t = m_Registry.GetComponent<Transform3DComponent>(m_PlayerEntity);
    if (t) {
      m_Camera->x = t->x; m_Camera->y = t->y; m_Camera->z = t->z;
      m_Camera->yaw = t->rot; m_Camera->pitch = t->pitch;
    }
  }
  else if (m_State == GameState::Siege) {
    float dt = deltaTime;
    HandleInputGameplay(dt);
    UpdatePhysics(dt);
    UpdateSiege(dt);

    auto *t = m_Registry.GetComponent<Transform3DComponent>(m_PlayerEntity);
    if (t) {
      m_Camera->x = t->x; m_Camera->y = t->y; m_Camera->z = t->z;
      m_Camera->yaw = t->rot; m_Camera->pitch = t->pitch;
    }

    if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
      m_State = GameState::Paused;
      SDL_SetRelativeMouseMode(SDL_FALSE);
      m_MenuSelection = 0;
    }
  }
  else if (m_State == GameState::Paused) {
    HandleInputPause();
  }
}

void DungeonsGame::UpdateSiege(float dt) {
  auto &enemies = m_Registry.View<EnemyComponent>();
  auto *playerTrans = m_Registry.GetComponent<Transform3DComponent>(m_PlayerEntity);
  if (!playerTrans) return;

  for (auto &pair : enemies) {
    auto &enemy = pair.second;
    auto *t = m_Registry.GetComponent<Transform3DComponent>(pair.first);
    if (t) {
      float dx = playerTrans->x - t->x;
      float dy = playerTrans->y - t->y;
      float dist = sqrt(dx * dx + dy * dy);
      if (dist > 1.5f) {
        t->rot = atan2(dy, dx);
        t->x += cos(t->rot) * enemy.speed * dt;
        t->y += sin(t->rot) * enemy.speed * dt;
      } else {
        m_ShakeTimer = 0.1f;
        m_ShakeIntensity = 0.02f;
      }
    }
  }
}

void DungeonsGame::PlaySpatialSfx(Mix_Chunk *chunk, float x, float y, float z) {
  if (!chunk || !m_Camera) return;
  int channel = Mix_PlayChannel(-1, chunk, 0);
  if (channel == -1) return;

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