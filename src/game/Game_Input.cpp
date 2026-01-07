#include "../engine/Components.h"
#include "../engine/Config.h"
#include "../engine/Input.h"
#include "DungeonsGame.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace PixelsEngine;

void DungeonsGame::HandleInputGameplay(float dt) {
  // ---------------------------------------------------------
  // 1. Global / System Input (Victory, Pause, Fullscreen)
  // ---------------------------------------------------------

  // In Creative mode, ignore movement input while the inventory is open
  if (m_State == GameState::Creative && m_CreativeMode.IsInventoryOpen()) {
    return;
  }

  if (m_GameFinished) {
    if (Input::IsKeyPressed(SDL_SCANCODE_SPACE)) {
      // Next Level
      m_CurrentLevel++;
      if (m_CurrentLevel > 3)
        m_CurrentLevel = 1;
      InitGame();
      return;
    }
    if (Input::IsKeyPressed(SDL_SCANCODE_R)) {
      // Retry
      InitGame();
      return;
    }
    return;
  }

  if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
    m_State = GameState::Paused;
    SDL_SetRelativeMouseMode(SDL_FALSE);
    m_MenuSelection = 0;
    return;
  }

  if (Input::IsKeyPressed(Config::GetKeybind(GameAction::ToggleFullScreen))) {
    ToggleFullScreen();
  }

  // ---------------------------------------------------------
  // 2. Component Retrieval & Safety Check
  // ---------------------------------------------------------

  auto *t = m_Registry.GetComponent<Transform3DComponent>(m_PlayerEntity);
  auto *p = m_Registry.GetComponent<PlayerControlComponent>(m_PlayerEntity);
  auto *phys = m_Registry.GetComponent<PhysicsComponent>(m_PlayerEntity);
  auto *weapon = m_Registry.GetComponent<WeaponComponent>(m_PlayerEntity);

  // CRITICAL FIX: Debugging why WASD might fail
  // Weapon is optional (Creative mode may not have weapon)
  if (!t || !p || !phys) {
    // Only print this once every few seconds to avoid spamming, or just print
    // it:
    static int debugSpamCounter = 0;
    if (debugSpamCounter++ % 600 ==
        0) { // Print once every ~10 seconds (assuming 60fps)
      std::cout << "[System] Player Input Skipped - Missing Component: ";
      if (!t)
        std::cout << "[Transform] ";
      if (!p)
        std::cout << "[PlayerControl] ";
      if (!phys)
        std::cout << "[Physics] ";
      std::cout << std::endl;
    }
    return; // We cannot move without these components
  }

  static int debugCreativeCounter = 0;
  if (m_State == GameState::Creative) {
    if (debugCreativeCounter++ % 60 == 0) {
      std::cout << "[CREATIVE] Has all components, ready for input" << std::endl;
    }
  }

  // ---------------------------------------------------------
  // 3. Mouse Look
  // ---------------------------------------------------------

  int mx, my;
  Input::GetMouseDelta(mx, my);
  float sensitivity = Config::GetMouseSensitivity();

  t->rot -= mx * sensitivity;
  t->pitch -= my * sensitivity;

  // Clamp pitch
  if (t->pitch > 1.5f)
    t->pitch = 1.5f;
  if (t->pitch < -1.5f)
    t->pitch = -1.5f;

  // ---------------------------------------------------------
  // 4. Movement Logic (WASD)
  // ---------------------------------------------------------

  float baseAccel = 100.0f;
  float accel = baseAccel * dt;
  if (phys->isSliding)
    accel = 0.0f;

  float dvx = 0, dvy = 0;
  bool isMoving = false;

  // Fly / Jump Logic
  if (Input::IsKeyPressed(SDL_SCANCODE_SPACE)) {
    uint32_t now = SDL_GetTicks();
    if (now - phys->lastSpaceTime < 250) {
      // Double tap space to toggle fly
      phys->isFlying = !phys->isFlying;
      phys->velZ = 0;
      phys->lastSpaceTime = 0;
    } else {
      phys->lastSpaceTime = now;
      if (!phys->isFlying && phys->isGrounded) {
        // Normal Jump
        phys->velZ = p->jumpForce;
        phys->isGrounded = false;
      }
    }
  }

  if (phys->isFlying) {
    if (Input::IsKeyDown(SDL_SCANCODE_SPACE))
      phys->velZ += accel;
    if (Input::IsKeyDown(SDL_SCANCODE_LSHIFT))
      phys->velZ -= accel;
  }

  // Standard Movement
  // In Siege mode, use camera angle. In other modes, use player rotation.
  float movementAngle = t->rot;
  if (m_State == GameState::Siege && m_SiegeMode) {
    // Use camera angle for Siege mode
    movementAngle = m_SiegeMode->GetCameraYaw();
  }
  
  if (Input::IsKeyDown(SDL_SCANCODE_W)) {
    dvx += cos(movementAngle) * accel;
    dvy += sin(movementAngle) * accel;
    isMoving = true;
    if (m_State == GameState::Creative) {
      std::cout << "[CREATIVE-WASD] W pressed: accel=" << accel << " dvx=" << dvx << " dvy=" << dvy << std::endl;
    }
  }
  if (Input::IsKeyDown(SDL_SCANCODE_S)) {
    dvx -= cos(movementAngle) * accel;
    dvy -= sin(movementAngle) * accel;
    isMoving = true;
  }
  if (Input::IsKeyDown(SDL_SCANCODE_A)) {
    dvx -= cos(movementAngle + M_PI / 2) * accel;
    dvy -= sin(movementAngle + M_PI / 2) * accel;
    isMoving = true;
  }
  if (Input::IsKeyDown(SDL_SCANCODE_D)) {
    dvx += cos(movementAngle - M_PI / 2) * accel;
    dvy += sin(movementAngle - M_PI / 2) * accel;
    isMoving = true;
  }

  if (m_State == GameState::Creative && (dvx != 0 || dvy != 0)) {
    std::cout << "[CREATIVE-VEL] Before: velX=" << phys->velX << " velY=" << phys->velY;
  }

  phys->velX += dvx;
  phys->velY += dvy;

  if (m_State == GameState::Creative && (dvx != 0 || dvy != 0)) {
    std::cout << " After: velX=" << phys->velX << " velY=" << phys->velY << std::endl;
  }

  // ---------------------------------------------------------
  // 5. Crouch & Slide
  // ---------------------------------------------------------

  bool wantsCrouch =
      Input::IsKeyDown(SDL_SCANCODE_LCTRL) || Input::IsKeyDown(SDL_SCANCODE_C);

  if (wantsCrouch && phys->isGrounded && !phys->isSliding) {
    float speed = sqrt(phys->velX * phys->velX + phys->velY * phys->velY);
    if (speed > 4.0f) {
      phys->isSliding = true;
      phys->slideTimer = 1.0f;
    }
  }
  if (!wantsCrouch)
    phys->isSliding = false;

  // Speed Limiting
  float maxSpeed = phys->isSliding ? p->speed * 2.0f : p->speed;
  float currentSpeed = sqrt(phys->velX * phys->velX + phys->velY * phys->velY);

  if (currentSpeed > maxSpeed && !m_IsGrappling && !phys->isSliding) {
    phys->velX = (phys->velX / currentSpeed) * maxSpeed;
    phys->velY = (phys->velY / currentSpeed) * maxSpeed;
  }

  // ---------------------------------------------------------
  // 6. Shooting & Combat
  // ---------------------------------------------------------

  // Only handle combat if weapon component exists (not in Creative mode)
  if (weapon) {
    bool wantsGrapple = Input::IsMouseButtonPressed(SDL_BUTTON_RIGHT) ||
                        (Input::IsKeyDown(SDL_SCANCODE_E) &&
                         Input::IsMouseButtonPressed(SDL_BUTTON_LEFT));

    if (Input::IsMouseButtonDown(SDL_BUTTON_LEFT) &&
        !Input::IsKeyDown(SDL_SCANCODE_E)) {
      if (!weapon->isDrawing && weapon->cooldown <= 0) {
        weapon->isDrawing = true;
        weapon->drawTime = 0.0f;
      } else if (weapon->isDrawing) {
        weapon->drawTime += dt;
        if (weapon->drawTime > 1.0f)
          weapon->drawTime = 1.0f;
      }
    } else if (wantsGrapple && weapon->cooldown <= 0) {
      // Fire Grapple
      if (m_SfxGrapple)
        Mix_PlayChannel(-1, m_SfxGrapple, 0);

      auto arrow = m_Registry.CreateEntity();
      float ax = t->x + cos(t->rot) * 0.5f;
      float ay = t->y + sin(t->rot) * 0.5f;

      m_Registry.AddComponent<Transform3DComponent>(
          arrow, {ax, ay, t->z, t->rot, t->pitch});
      m_Registry.AddComponent<ProjectileComponent>(
          arrow, {ProjectileComponent::Grapple, 0.0f, true, 2.0f});

      float speed = 25.0f;
      m_Registry.AddComponent<PhysicsComponent>(
          arrow, {cos(t->rot) * speed, sin(t->rot) * speed, t->pitch * 0.05f,
                  0.0f, false, false, 0.0f, 0.0f});
      weapon->cooldown = 1.0f;
    } else {
      if (weapon->isDrawing) {
        // Fire Arrow
        float power = std::min(1.0f, weapon->drawTime);
        weapon->isDrawing = false;
        weapon->cooldown = 0.5f;

        if (m_State != GameState::Creative) {
          if (m_SfxShoot)
            Mix_PlayChannel(-1, m_SfxShoot, 0);

          // Recoil
          t->pitch += 10.0f + (power * 20.0f);
          m_ShakeTimer = 0.1f + (power * 0.1f);
          m_ShakeIntensity = 0.04f + (power * 0.06f);

          auto arrow = m_Registry.CreateEntity();
          float ax = t->x + cos(t->rot) * 0.5f;
          float ay = t->y + sin(t->rot) * 0.5f;

          m_Registry.AddComponent<Transform3DComponent>(
              arrow, {ax, ay, t->z, t->rot, t->pitch});
          m_Registry.AddComponent<ProjectileComponent>(
              arrow, {ProjectileComponent::Arrow, 50.0f, true, 5.0f});

          float speed = 8.0f + (power * 17.0f);
          float verticalBias = -1.5f + (power * 3.5f);
          float vz = (t->pitch * 0.03f) + verticalBias;

          m_Registry.AddComponent<PhysicsComponent>(
              arrow, {cos(t->rot) * speed, sin(t->rot) * speed, vz, 15.0f, false,
                      false, 0.0f, 0.0f});
          m_Registry.AddComponent<BillboardComponent>(
              arrow, {m_BowIdle, 0.2f, 0.2f, 0.2f, true});
        }
      }
    }

    if (weapon->cooldown > 0)
      weapon->cooldown -= dt;
  }
}

void DungeonsGame::HandleInputMenu() {
  int w = m_Width;
  int h = m_Height;
  int mx, my;
  Input::GetMousePosition(mx, my);

  int btnW = 240;
  int btnH = 50;
  int startY = 300;
  int gap = 70;
  int startX = w / 2 - btnW / 2;
  bool action = false;
  int numButtons = m_InOptions ? 2 : 6;

  // Mouse
  for (int i = 0; i < numButtons; i++) {
    int by = startY + gap * i;
    if (mx >= startX && mx <= startX + btnW && my >= by && my <= by + btnH) {
      if (m_MenuSelection != i)
        m_MenuSelection = i;
      if (Input::IsMouseButtonPressed(SDL_BUTTON_LEFT))
        action = true;
    }
  }

  // Keyboard
  if (Input::IsKeyPressed(SDL_SCANCODE_W) ||
      Input::IsKeyPressed(SDL_SCANCODE_UP)) {
    m_MenuSelection--;
    if (m_MenuSelection < 0)
      m_MenuSelection = numButtons - 1;
  }
  if (Input::IsKeyPressed(SDL_SCANCODE_S) ||
      Input::IsKeyPressed(SDL_SCANCODE_DOWN)) {
    m_MenuSelection++;
    if (m_MenuSelection > numButtons - 1)
      m_MenuSelection = 0;
  }
  if (Input::IsKeyPressed(SDL_SCANCODE_RETURN) ||
      Input::IsKeyPressed(SDL_SCANCODE_SPACE)) {
    action = true;
  }
  if (Input::IsKeyPressed(Config::GetKeybind(GameAction::ToggleFullScreen))) {
    ToggleFullScreen();
  }

  if (action) {
    if (!m_InOptions) {
      switch (m_MenuSelection) {
      case 0:
        InitGame();
        m_State = GameState::Playing;
        SDL_SetRelativeMouseMode(SDL_TRUE);
        break;
      case 1:
        InitGame();
        m_State = GameState::Creative;
        m_CreativeMode.ToggleEditorMode();
        SDL_SetRelativeMouseMode(SDL_TRUE);
        break;
      case 2:
        InitSiege();
        m_State = GameState::Siege;
        SDL_SetRelativeMouseMode(SDL_TRUE);
        break;
      case 3:
        InitBattle();
        m_State = GameState::Battle;
        SDL_SetRelativeMouseMode(SDL_FALSE);
        break;
      case 4:
        m_InOptions = true;
        m_MenuSelection = 0;
        break;
      case 5:
        m_IsRunning = false;
        break;
      }
    } else {
      if (m_MenuSelection == 0)
        ToggleFullScreen();
      else if (m_MenuSelection == 1) {
        m_InOptions = false;
        m_MenuSelection = 4;
      }
    }
  }
}

void DungeonsGame::HandleInputPause() {
  if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
    if (m_InOptions) {
      m_InOptions = false;
      m_MenuSelection = 3;
    } else {
      m_State = GameState::Playing;
      SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    return;
  }

  int w = m_Width;
  int h = m_Height;
  int mx, my;
  Input::GetMousePosition(mx, my);

  int btnW = 240;
  int btnH = 50;
  int startY = 200;
  int gap = 60;
  int startX = w / 2 - btnW / 2;
  bool action = false;
  int numButtons = m_InOptions ? 2 : 6;

  for (int i = 0; i < numButtons; i++) {
    int by = startY + gap * i;
    if (mx >= startX && mx <= startX + btnW && my >= by && my <= by + btnH) {
      if (m_MenuSelection != i)
        m_MenuSelection = i;
      if (Input::IsMouseButtonPressed(SDL_BUTTON_LEFT))
        action = true;
    }
  }

  if (Input::IsKeyPressed(SDL_SCANCODE_W) ||
      Input::IsKeyPressed(SDL_SCANCODE_UP)) {
    m_MenuSelection--;
    if (m_MenuSelection < 0)
      m_MenuSelection = numButtons - 1;
  }
  if (Input::IsKeyPressed(SDL_SCANCODE_S) ||
      Input::IsKeyPressed(SDL_SCANCODE_DOWN)) {
    m_MenuSelection++;
    if (m_MenuSelection > numButtons - 1)
      m_MenuSelection = 0;
  }
  if (Input::IsKeyPressed(SDL_SCANCODE_RETURN) ||
      Input::IsKeyPressed(SDL_SCANCODE_SPACE)) {
    action = true;
  }

  if (action) {
    if (!m_InOptions) {
      switch (m_MenuSelection) {
      case 0:
        m_State = GameState::Playing;
        if (m_CreativeMode.IsActive())
          m_State = GameState::Creative;
        SDL_SetRelativeMouseMode(SDL_TRUE);
        break;
      case 1:
        m_CreativeMode.SaveDungeon("assets/dungeons/my_dungeon.map");
        break;
      case 2:
        m_CreativeMode.LoadDungeon("assets/dungeons/my_dungeon.map");
        break;
      case 3:
        m_InOptions = true;
        m_MenuSelection = 0;
        break;
      case 4:
        m_State = GameState::MainMenu;
        m_MenuSelection = 0;
        break;
      case 5:
        m_IsRunning = false;
        break;
      }
    } else {
      if (m_MenuSelection == 0)
        ToggleFullScreen();
      else if (m_MenuSelection == 1) {
        m_InOptions = false;
        m_MenuSelection = 3;
      }
    }
  }
}

void DungeonsGame::HandleInputCreative(float dt) {
  // Skip movement input while the inventory is open
  if (m_CreativeMode.IsInventoryOpen()) {
    return;
  }

  // 1. Mouse Look (moves camera rotation)
  int mx, my;
  Input::GetMouseDelta(mx, my);
  float sensitivity = 0.003f;
  
  if (!m_Camera) return;
  
  m_Camera->yaw -= mx * sensitivity;
  m_Camera->pitch -= my * sensitivity;
  
  // Clamp pitch
  if (m_Camera->pitch > 1.5f) m_Camera->pitch = 1.5f;
  if (m_Camera->pitch < -1.5f) m_Camera->pitch = -1.5f;

  // 2. WASD Camera Movement
  float moveSpeed = 10.0f * dt;
  
  // Get camera forward direction
  float dirX = cos(m_Camera->yaw) * cos(m_Camera->pitch);
  float dirY = sin(m_Camera->yaw) * cos(m_Camera->pitch);
  float dirZ = sin(m_Camera->pitch);
  
  // Get camera right direction
  float rightX = cos(m_Camera->yaw - M_PI / 2.0f);
  float rightY = sin(m_Camera->yaw - M_PI / 2.0f);
  
  if (Input::IsKeyDown(SDL_SCANCODE_W)) {
    m_Camera->x += dirX * moveSpeed;
    m_Camera->y += dirY * moveSpeed;
    m_Camera->z += dirZ * moveSpeed;
  }
  if (Input::IsKeyDown(SDL_SCANCODE_S)) {
    m_Camera->x -= dirX * moveSpeed;
    m_Camera->y -= dirY * moveSpeed;
    m_Camera->z -= dirZ * moveSpeed;
  }
  if (Input::IsKeyDown(SDL_SCANCODE_A)) {
    m_Camera->x -= rightX * moveSpeed;
    m_Camera->y -= rightY * moveSpeed;
  }
  if (Input::IsKeyDown(SDL_SCANCODE_D)) {
    m_Camera->x += rightX * moveSpeed;
    m_Camera->y += rightY * moveSpeed;
  }
  if (Input::IsKeyDown(SDL_SCANCODE_SPACE)) {
    m_Camera->z += moveSpeed;
  }
  if (Input::IsKeyDown(SDL_SCANCODE_LSHIFT)) {
    m_Camera->z -= moveSpeed;
  }
}