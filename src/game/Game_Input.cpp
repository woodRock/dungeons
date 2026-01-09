#include "../engine/Components.h"
#include "../engine/Config.h"
#include "../engine/Input.h"
#include "DungeonsGame.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <filesystem>
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
    // Game finished - return to main menu
    if (Input::IsKeyPressed(SDL_SCANCODE_SPACE) || Input::IsKeyPressed(SDL_SCANCODE_R)) {
      m_State = GameState::MainMenu;
      SDL_SetRelativeMouseMode(SDL_FALSE);
      m_MenuSelection = 0;
      return;
    }
    return;
  }

  if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
    m_PreviousState = m_State;
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

  // Weapon is optional (Creative mode may not have weapon)
  if (!t || !p || !phys) {
    return;
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

  phys->velX += dvx;
  phys->velY += dvy;

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

  if (currentSpeed > maxSpeed && !phys->isSliding) {
    phys->velX = (phys->velX / currentSpeed) * maxSpeed;
    phys->velY = (phys->velY / currentSpeed) * maxSpeed;
  }

  // ---------------------------------------------------------
  // 6. Shooting & Combat
  // ---------------------------------------------------------

  // Only handle combat if weapon component exists (not in Creative mode)
  if (weapon) {
    if (Input::IsMouseButtonDown(SDL_BUTTON_LEFT) && !Input::IsKeyDown(SDL_SCANCODE_E)) {
      if (!weapon->isDrawing && weapon->cooldown <= 0) {
        weapon->isDrawing = true;
        weapon->drawTime = 0.0f;
      } else if (weapon->isDrawing) {
        weapon->drawTime += dt;
        if (weapon->drawTime > 1.0f)
          weapon->drawTime = 1.0f;
      }
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
  int startY = 150;
  int gap = 60;
  int startX = w / 2 - btnW / 2;
  bool action = false;
  int numButtons = m_InOptions ? 2 : 8;

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
        m_Registry = Registry();
        m_GameFinished = false;
        // Initialize camera to face forward (0 radians) and level (0 pitch)
        m_Camera->yaw = 0.0f;
        m_Camera->pitch = 0.0f;
        m_Camera->x = 5.0f;
        m_Camera->y = 5.0f;
        m_Camera->z = 1.7f;
        m_CreativeMode.Init(&m_Registry, &m_GLRenderer, m_Camera.get());
        m_CreativeMode.ToggleEditorMode();
        m_CreativeMode.ToggleSaveMenu(); // Show map selection menu
        m_State = GameState::Creative;
        SDL_SetRelativeMouseMode(SDL_FALSE);
        break;
      case 1:
        // Dungeon Mode - Go to Character Selection
        m_State = GameState::CharacterSelect;
        m_CharacterSelection = 0;
        SDL_SetRelativeMouseMode(SDL_FALSE);
        break;
      case 2:
        m_Registry = Registry();
        m_GameFinished = false;
        m_SidescrollerMode = std::make_unique<SidescrollerMode>(&m_Registry, &m_GLRenderer);
        m_SidescrollerMode->Init(m_Camera.get(), m_PlayerEntity);
        m_State = GameState::Sidescroller;
        SDL_SetRelativeMouseMode(SDL_TRUE);
        break;
      case 3:
        m_Registry = Registry();
        m_GameFinished = false;
        m_StealthMode = std::make_unique<StealthMode>(&m_Registry, &m_GLRenderer);
        m_StealthMode->Init(m_Camera.get(), m_PlayerEntity);
        m_State = GameState::Stealth;
        SDL_SetRelativeMouseMode(SDL_TRUE);
        break;
      case 4:
        InitSiege();
        m_State = GameState::Siege;
        SDL_SetRelativeMouseMode(SDL_TRUE);
        break;
      case 5:
        InitBattle();
        m_State = GameState::Battle;
        SDL_SetRelativeMouseMode(SDL_FALSE);
        break;
      case 6:
        m_InOptions = true;
        m_MenuSelection = 0;
        break;
      case 7:
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
      // Resume to previous game mode
      m_State = m_PreviousState;
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
        // Resume to previous game mode
        m_State = m_PreviousState;
        SDL_SetRelativeMouseMode(SDL_TRUE);
        break;
      case 1:
        m_CreativeMode.ToggleSaveMenu();
        m_State = GameState::Creative;
        SDL_SetRelativeMouseMode(SDL_FALSE);
        break;
      case 2:
        m_CreativeMode.ToggleSaveMenu();
        m_State = GameState::Creative;
        SDL_SetRelativeMouseMode(SDL_FALSE);
        break;
      case 3:
        // Go to Settings
        m_State = GameState::Settings;
        m_SettingsSelection = 0;
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
void DungeonsGame::HandleInputSettings() {
  GameSettings& settings = GameSettings::Instance();
  int numOptions = 5;

  if (Input::IsKeyPressed(SDL_SCANCODE_W) ||
      Input::IsKeyPressed(SDL_SCANCODE_UP)) {
    m_SettingsSelection--;
    if (m_SettingsSelection < 0)
      m_SettingsSelection = numOptions - 1;
  }
  if (Input::IsKeyPressed(SDL_SCANCODE_S) ||
      Input::IsKeyPressed(SDL_SCANCODE_DOWN)) {
    m_SettingsSelection++;
    if (m_SettingsSelection >= numOptions)
      m_SettingsSelection = 0;
  }

  if (Input::IsKeyPressed(SDL_SCANCODE_A) ||
      Input::IsKeyPressed(SDL_SCANCODE_LEFT)) {
    if (m_SettingsSelection == 0)
      settings.SetMusicVolume(settings.GetMusicVolume() - 0.1f);
    else if (m_SettingsSelection == 1)
      settings.SetSFXVolume(settings.GetSFXVolume() - 0.1f);
    else if (m_SettingsSelection == 2)
      settings.SetMouseSensitivity(settings.GetMouseSensitivity() - 0.1f);
    else if (m_SettingsSelection == 3)
      settings.SetDifficulty(settings.GetDifficulty() - 1);
  }
  if (Input::IsKeyPressed(SDL_SCANCODE_D) ||
      Input::IsKeyPressed(SDL_SCANCODE_RIGHT)) {
    if (m_SettingsSelection == 0)
      settings.SetMusicVolume(settings.GetMusicVolume() + 0.1f);
    else if (m_SettingsSelection == 1)
      settings.SetSFXVolume(settings.GetSFXVolume() + 0.1f);
    else if (m_SettingsSelection == 2)
      settings.SetMouseSensitivity(settings.GetMouseSensitivity() + 0.1f);
    else if (m_SettingsSelection == 3)
      settings.SetDifficulty(settings.GetDifficulty() + 1);
  }

  if (Input::IsKeyPressed(SDL_SCANCODE_RETURN) ||
      Input::IsKeyPressed(SDL_SCANCODE_SPACE)) {
    if (m_SettingsSelection == 4) {
      // Back
      m_State = GameState::Paused;
      m_MenuSelection = 0;
    }
  }

  if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
    m_State = GameState::Paused;
    m_MenuSelection = 0;
  }
}

void DungeonsGame::HandleInputGameOver() {
  int w = m_Width;
  int h = m_Height;
  int mx, my;
  Input::GetMousePosition(mx, my);
  bool clicked = Input::IsMouseButtonPressed(SDL_BUTTON_LEFT);
  
  int btnW = 200;
  int btnH = 50;
  int btnStartX = w / 2 - 100;
  int btnStartY = h - 120;
  
  // Retry button
  if (mx >= btnStartX - btnW - 20 && mx <= btnStartX - 20 && my >= btnStartY && my <= btnStartY + btnH) {
    m_MenuSelection = 0;
    if (clicked) {
      // Retry - go back to map select with same character
      m_AvailableMaps.clear();
      if (std::filesystem::exists("assets/saves/")) {
          for (const auto & entry : std::filesystem::directory_iterator("assets/saves/")) {
              if (entry.path().extension() == ".dungeon") {
                  m_AvailableMaps.push_back(entry.path().stem().string());
              }
          }
      }
      if (!m_AvailableMaps.empty()) {
          m_State = GameState::MapSelect;
          m_MapSelectIdx = 0;
          SDL_SetRelativeMouseMode(SDL_FALSE);
      }
      return;
    }
  }
  // Main Menu button
  else if (mx >= btnStartX + btnW + 20 && mx <= btnStartX + btnW + 20 + btnW && my >= btnStartY && my <= btnStartY + btnH) {
    m_MenuSelection = 1;
    if (clicked) {
      m_State = GameState::MainMenu;
      m_MenuSelection = 0;
      return;
    }
  }
  
  // Keyboard input
  if (Input::IsKeyPressed(SDL_SCANCODE_LEFT) ||
      Input::IsKeyPressed(SDL_SCANCODE_A)) {
    m_MenuSelection = 0;
  }
  if (Input::IsKeyPressed(SDL_SCANCODE_RIGHT) ||
      Input::IsKeyPressed(SDL_SCANCODE_D)) {
    m_MenuSelection = 1;
  }

  if (Input::IsKeyPressed(SDL_SCANCODE_RETURN) ||
      Input::IsKeyPressed(SDL_SCANCODE_SPACE)) {
    if (m_MenuSelection == 0) {
      // Retry - go back to map select
      m_AvailableMaps.clear();
      if (std::filesystem::exists("assets/saves/")) {
          for (const auto & entry : std::filesystem::directory_iterator("assets/saves/")) {
              if (entry.path().extension() == ".dungeon") {
                  m_AvailableMaps.push_back(entry.path().stem().string());
              }
          }
      }
      if (!m_AvailableMaps.empty()) {
          m_State = GameState::MapSelect;
          m_MapSelectIdx = 0;
          SDL_SetRelativeMouseMode(SDL_FALSE);
      }
    } else {
      // Main Menu
      m_State = GameState::MainMenu;
      m_MenuSelection = 0;
    }
  }
}

void DungeonsGame::HandleInputCharacterSelect() {
  int w = m_Width;
  int h = m_Height;
  int mx, my;
  Input::GetMousePosition(mx, my);
  bool clicked = Input::IsMouseButtonPressed(SDL_BUTTON_LEFT);
  
  int numCharacters = 5; // 4 characters + confirm button
  int charactersPerRow = 2;

  // Character button positions
  int btnW = 200;
  int btnH = 60;
  int gap = 100;
  int startX = (w - (btnW * 2 + gap)) / 2;
  int startY = 200;
  
  // Knight button
  if (mx >= startX && mx <= startX + btnW && my >= startY && my <= startY + btnH) {
    m_CharacterSelection = 0;
    if (clicked) {
      m_SelectedCharacter = "Knight";
      m_CharacterSelection = 4; // Move to confirm
    }
  }
  // Ranger button
  else if (mx >= startX + btnW + gap && mx <= startX + btnW + gap + btnW && my >= startY && my <= startY + btnH) {
    m_CharacterSelection = 1;
    if (clicked) {
      m_SelectedCharacter = "Ranger";
      m_CharacterSelection = 4; // Move to confirm
    }
  }
  // Mage button
  else if (mx >= startX && mx <= startX + btnW && my >= startY + btnH + 50 && my <= startY + btnH + 50 + btnH) {
    m_CharacterSelection = 2;
    if (clicked) {
      m_SelectedCharacter = "Mage";
      m_CharacterSelection = 4; // Move to confirm
    }
  }
  // Rogue button
  else if (mx >= startX + btnW + gap && mx <= startX + btnW + gap + btnW && my >= startY + btnH + 50 && my <= startY + btnH + 50 + btnH) {
    m_CharacterSelection = 3;
    if (clicked) {
      m_SelectedCharacter = "Rogue";
      m_CharacterSelection = 4; // Move to confirm
    }
  }
  // Confirm button
  else if (mx >= w/2 - 100 && mx <= w/2 + 100 && my >= h - 120 && my <= h - 70) {
    m_CharacterSelection = 4;
    if (clicked) {
      // Character confirmed - scan for available dungeon campaigns
      m_AvailableMaps.clear();
      if (std::filesystem::exists("assets/saves/")) {
          for (const auto & entry : std::filesystem::directory_iterator("assets/saves/")) {
              if (entry.path().extension() == ".dungeon") {
                  m_AvailableMaps.push_back(entry.path().stem().string());
              }
          }
      }
      if (!m_AvailableMaps.empty()) {
          m_State = GameState::MapSelect;
          m_MapSelectIdx = 0;
          SDL_SetRelativeMouseMode(SDL_FALSE);
      }
      return;
    }
  }

  // Keyboard navigation
  if (Input::IsKeyPressed(SDL_SCANCODE_W) ||
      Input::IsKeyPressed(SDL_SCANCODE_UP)) {
    if (m_CharacterSelection >= 4) {
      m_CharacterSelection = 1;
    } else {
      m_CharacterSelection -= charactersPerRow;
      if (m_CharacterSelection < 0)
        m_CharacterSelection = numCharacters - 1;
    }
  }
  if (Input::IsKeyPressed(SDL_SCANCODE_S) ||
      Input::IsKeyPressed(SDL_SCANCODE_DOWN)) {
    if (m_CharacterSelection < 4) {
      m_CharacterSelection = 4;
    } else {
      m_CharacterSelection = 0;
    }
  }
  if (Input::IsKeyPressed(SDL_SCANCODE_A) ||
      Input::IsKeyPressed(SDL_SCANCODE_LEFT)) {
    m_CharacterSelection--;
    if (m_CharacterSelection < 0)
      m_CharacterSelection = numCharacters - 1;
  }
  if (Input::IsKeyPressed(SDL_SCANCODE_D) ||
      Input::IsKeyPressed(SDL_SCANCODE_RIGHT)) {
    m_CharacterSelection++;
    if (m_CharacterSelection >= numCharacters)
      m_CharacterSelection = 0;
  }

  if (Input::IsKeyPressed(SDL_SCANCODE_RETURN) ||
      Input::IsKeyPressed(SDL_SCANCODE_SPACE)) {
    if (m_CharacterSelection < 4) {
      // Select character and move to confirm
      if (m_CharacterSelection == 0)
        m_SelectedCharacter = "Knight";
      else if (m_CharacterSelection == 1)
        m_SelectedCharacter = "Ranger";
      else if (m_CharacterSelection == 2)
        m_SelectedCharacter = "Mage";
      else if (m_CharacterSelection == 3)
        m_SelectedCharacter = "Rogue";
      m_CharacterSelection = 4;
    } else if (m_CharacterSelection == 4) {
      // Confirm and proceed
      m_AvailableMaps.clear();
      if (std::filesystem::exists("assets/saves/")) {
          for (const auto & entry : std::filesystem::directory_iterator("assets/saves/")) {
              if (entry.path().extension() == ".dungeon") {
                  m_AvailableMaps.push_back(entry.path().stem().string());
              }
          }
      }
      if (!m_AvailableMaps.empty()) {
          m_State = GameState::MapSelect;
          m_MapSelectIdx = 0;
          SDL_SetRelativeMouseMode(SDL_FALSE);
      }
      return;
    }
  }

  if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
    m_State = GameState::MainMenu;
    m_MenuSelection = 0;
  }
}
