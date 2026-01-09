#include "DungeonsGame.h"
#include "../engine/Components.h"
#include "../engine/Input.h"
#include <fstream>
#include <iostream>

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

  if (m_State == GameState::Settings) {
    HandleInputSettings();
    return;
  }

  if (m_State == GameState::GameOver) {
    HandleInputGameOver();
    return;
  }

  if (m_State == GameState::CharacterSelect) {
    HandleInputCharacterSelect();
    return;
  }

  // From here on, delegate to mode-specific update functions.
  switch (m_State) {
  case GameState::Creative: {
    HandleInputCreative(deltaTime);
    m_CreativeMode.Update(deltaTime, m_PlayerEntity);
    if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
      m_PreviousState = m_State;
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
      m_PreviousState = m_State;
      m_State = GameState::Paused;
      SDL_SetRelativeMouseMode(SDL_FALSE);
      m_MenuSelection = 0;
    }
    break;
  }
  case GameState::Dungeon: {
    if (m_DungeonMode) {
      m_DungeonMode->Update(deltaTime, m_PlayerEntity);
      // Update dungeon stats
      m_LastDungeonStats.currentLevel = m_DungeonMode->GetCurrentLevelIdx() + 1;
      m_LastDungeonStats.totalLevels = m_DungeonMode->GetTotalLevels();
      // Check if dungeon is cleared
      if (!m_DungeonMode->IsActive() && m_State == GameState::Dungeon) {
        m_State = GameState::GameOver;
        SDL_SetRelativeMouseMode(SDL_FALSE);
        m_MenuSelection = 0;
      }
    }
    m_RunTimer += deltaTime;
    m_LastDungeonStats.timeElapsed = m_RunTimer;
    UpdatePhysics(deltaTime);
    UpdateDoors(deltaTime);
    UpdateProjectiles(deltaTime);
    UpdateAnimations(deltaTime);
    if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
      m_PreviousState = m_State;
      m_State = GameState::Paused;
      SDL_SetRelativeMouseMode(SDL_FALSE);
      m_MenuSelection = 0;
    }
    break;
  }
  case GameState::Sidescroller: {
    if (m_SidescrollerMode) {
      m_SidescrollerMode->Update(deltaTime, m_PlayerEntity);
      // Sync player entity back from sidescroller mode
      m_PlayerEntity = m_SidescrollerMode->GetPlayerEntity();
      UpdatePhysics(deltaTime);
      UpdateDoors(deltaTime);
      UpdateProjectiles(deltaTime);
      UpdateAnimations(deltaTime);
      if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_PreviousState = m_State;
        m_State = GameState::Paused;
        SDL_SetRelativeMouseMode(SDL_FALSE);
        m_MenuSelection = 0;
      }
    }
    break;
  }
  case GameState::Stealth: {
    if (m_StealthMode) {
      m_StealthMode->Update(deltaTime, m_PlayerEntity);
      // Sync player entity back from stealth mode
      m_PlayerEntity = m_StealthMode->GetPlayerEntity();
      UpdatePhysics(deltaTime);
      UpdateDoors(deltaTime);
      UpdateProjectiles(deltaTime);
      UpdateAnimations(deltaTime);
      if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_PreviousState = m_State;
        m_State = GameState::Paused;
        SDL_SetRelativeMouseMode(SDL_FALSE);
        m_MenuSelection = 0;
      }
    }
    break;
  }
  case GameState::MapSelect: {
      HandleInputMapSelect();
      break;
  }
  case GameState::FloorSelect: {
      HandleInputFloorSelect();
      break;
  }
  case GameState::Battle: {
    UpdateBattle(deltaTime);
    UpdatePhysics(deltaTime);
    UpdateAnimations(deltaTime);
    if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
      m_PreviousState = m_State;
      m_State = GameState::Paused;
      m_MenuSelection = 0;
    }
    break;
  }
  case GameState::Playing: {
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

void DungeonsGame::InitDungeon() {
    // This is now handled in HandleInputFloorSelect
}

void DungeonsGame::RenderMapSelect() {
    m_GLRenderer.DrawRect2D(0, 0, (float)m_Width, (float)m_Height, {20, 20, 30, 255});
    m_TextRenderer->RenderTextCentered(&m_GLRenderer, "SELECT DUNGEON CAMPAIGN", m_Width / 2, 100, {255, 255, 100, 255});

    int btnW = 300;
    int btnH = 50;
    int startY = 200;
    int gap = 60;

    for (size_t i = 0; i < m_AvailableMaps.size(); ++i) {
        DrawButton(m_Width / 2 - btnW / 2, startY + (int)i * gap, btnW, btnH, m_AvailableMaps[i], (int)i == m_MapSelectIdx);
    }
    
    m_TextRenderer->RenderTextCentered(&m_GLRenderer, "UP/DOWN: Navigate | ENTER: Select | ESC: Back", m_Width / 2, m_Height - 100, {150, 150, 150, 255});
}

void DungeonsGame::RenderFloorSelect() {
    m_GLRenderer.DrawRect2D(0, 0, (float)m_Width, (float)m_Height, {20, 20, 30, 255});
    m_TextRenderer->RenderTextCentered(&m_GLRenderer, "SELECT START FLOOR", m_Width / 2, 100, {255, 255, 100, 255});
    m_TextRenderer->RenderTextCentered(&m_GLRenderer, "Dungeon: " + m_SelectedDungeonFile, m_Width / 2, 140, {200, 200, 200, 255});

    int btnW = 300;
    int btnH = 40;
    int startY = 200;
    int gap = 50;

    for (size_t i = 0; i < m_AvailableFloors.size(); ++i) {
        DrawButton(m_Width / 2 - btnW / 2, startY + (int)i * gap, btnW, btnH, std::to_string(i+1) + ". " + m_AvailableFloors[i], (int)i == m_FloorSelectIdx);
    }
    
    m_TextRenderer->RenderTextCentered(&m_GLRenderer, "ENTER: Start from here | ESC: Back", m_Width / 2, m_Height - 100, {150, 150, 150, 255});
}

void DungeonsGame::HandleInputMapSelect() {
    int mx, my;
    Input::GetMousePosition(mx, my);
    bool clicked = Input::IsMouseButtonPressed(SDL_BUTTON_LEFT);

    int btnW = 300;
    int btnH = 50;
    int startY = 200;
    int gap = 60;
    int startX = m_Width / 2 - btnW / 2;

    for (size_t i = 0; i < m_AvailableMaps.size(); ++i) {
        int by = startY + (int)i * gap;
        if (mx >= startX && mx <= startX + btnW && my >= by && my <= by + btnH) {
            m_MapSelectIdx = (int)i;
            if (clicked) {
                // Trigger the select logic
                m_SelectedDungeonFile = m_AvailableMaps[m_MapSelectIdx];
                m_AvailableFloors.clear();
                std::ifstream in("assets/saves/" + m_SelectedDungeonFile + ".dungeon");
                if (in.is_open()) {
                    std::string floor;
                    while (in >> floor) if (!floor.empty()) m_AvailableFloors.push_back(floor);
                    in.close();
                }
                
                if (!m_AvailableFloors.empty()) {
                    m_State = GameState::FloorSelect;
                    m_FloorSelectIdx = 0;
                }
                return;
            }
        }
    }

    if (Input::IsKeyPressed(SDL_SCANCODE_UP)) {
        m_MapSelectIdx = (m_MapSelectIdx - 1 + (int)m_AvailableMaps.size()) % (int)m_AvailableMaps.size();
    }
    if (Input::IsKeyPressed(SDL_SCANCODE_DOWN)) {
        m_MapSelectIdx = (m_MapSelectIdx + 1) % (int)m_AvailableMaps.size();
    }
    if (Input::IsKeyPressed(SDL_SCANCODE_RETURN) && !m_AvailableMaps.empty()) {
        // Load floors for this dungeon
        m_SelectedDungeonFile = m_AvailableMaps[m_MapSelectIdx];
        m_AvailableFloors.clear();
        std::ifstream in("assets/dungeons/" + m_SelectedDungeonFile + ".dungeon");
        if (in.is_open()) {
            std::string floor;
            while (in >> floor) if (!floor.empty()) m_AvailableFloors.push_back(floor);
            in.close();
        }
        
        if (!m_AvailableFloors.empty()) {
            m_State = GameState::FloorSelect;
            m_FloorSelectIdx = 0;
        }
    }
    if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_State = GameState::MainMenu;
    }
}

void DungeonsGame::HandleInputFloorSelect() {
    int mx, my;
    Input::GetMousePosition(mx, my);
    bool clicked = Input::IsMouseButtonPressed(SDL_BUTTON_LEFT);

    int btnW = 300;
    int btnH = 40;
    int startY = 200;
    int gap = 50;
    int startX = m_Width / 2 - btnW / 2;

    for (size_t i = 0; i < m_AvailableFloors.size(); ++i) {
        int by = startY + (int)i * gap;
        if (mx >= startX && mx <= startX + btnW && my >= by && my <= by + btnH) {
            m_FloorSelectIdx = (int)i;
            if (clicked) {
                m_DungeonMode = std::make_unique<DungeonMode>(&m_Registry, &m_GLRenderer);
                m_DungeonMode->SetMap(&m_Map);  // Pass map for collision detection
                std::vector<std::string> floors;
                for (size_t j = m_FloorSelectIdx; j < m_AvailableFloors.size(); ++j) {
                    floors.push_back(m_AvailableFloors[j]);
                }
                m_DungeonMode->StartDungeon(floors);
                m_DungeonMode->Init(m_Camera.get(), m_PlayerEntity);
                m_State = GameState::Dungeon;
                SDL_SetRelativeMouseMode(SDL_TRUE);
                return;
            }
        }
    }

    if (Input::IsKeyPressed(SDL_SCANCODE_UP)) {
        m_FloorSelectIdx = (m_FloorSelectIdx - 1 + (int)m_AvailableFloors.size()) % (int)m_AvailableFloors.size();
    }
    if (Input::IsKeyPressed(SDL_SCANCODE_DOWN)) {
        m_FloorSelectIdx = (m_FloorSelectIdx + 1) % (int)m_AvailableFloors.size();
    }
    if (Input::IsKeyPressed(SDL_SCANCODE_RETURN) && !m_AvailableFloors.empty()) {
        m_DungeonMode = std::make_unique<DungeonMode>(&m_Registry, &m_GLRenderer);
        m_DungeonMode->SetMap(&m_Map);  // Pass map for collision detection
        
        std::vector<std::string> floors;
        for (size_t i = m_FloorSelectIdx; i < m_AvailableFloors.size(); ++i) {
            floors.push_back(m_AvailableFloors[i]);
        }
        
        m_DungeonMode->StartDungeon(floors);
        m_DungeonMode->Init(m_Camera.get(), m_PlayerEntity);
        
        // Initialize dungeon stats
        m_LastDungeonStats.enemiesKilled = 0;
        m_LastDungeonStats.timeElapsed = 0.0f;
        m_LastDungeonStats.currentLevel = m_FloorSelectIdx + 1;
        m_LastDungeonStats.totalLevels = m_AvailableFloors.size();
        m_LastDungeonStats.playerHealth = 100;
        m_RunTimer = 0.0f;
        
        m_State = GameState::Dungeon;
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_State = GameState::MapSelect;
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
  auto &proceduralView = m_Registry.View<ProceduralAnimationComponent>();
  for (auto &pair : proceduralView) {
    Entity e = pair.first;
    auto &anim = pair.second;
    auto *t = m_Registry.GetComponent<Transform3DComponent>(e);
    if (t) {
      anim.timeOffset += dt;
      t->z = anim.baseZ + sin(anim.timeOffset * anim.bobSpeed) * anim.bobAmount;
      t->pitch = sin(anim.timeOffset * anim.swaySpeed) * anim.swayAmount;
    }
  }

  auto &skeletalView = m_Registry.View<SkeletalAnimationComponent>();
  for (auto &pair : skeletalView) {
    Entity e = pair.first;
    auto &anim = pair.second;
    auto *mesh = m_Registry.GetComponent<MeshComponent>(e);
    if (mesh) {
      RenderMesh *rm = m_GLRenderer.GetRenderMesh(mesh->meshName);
      if (rm && rm->isSkinned) {
        // Advance animation time
        anim.currentTime += dt * anim.speed;
        
        // Handle looping and clamping based on animation settings
        if (anim.animationIndex >= 0 && anim.animationIndex < (int)rm->animations.size()) {
          float animDuration = rm->animations[anim.animationIndex].duration;
          if (animDuration > 0) {
            if (anim.loop) {
              // Wrap time for looping animations
              while (anim.currentTime >= animDuration) {
                anim.currentTime -= animDuration;
              }
            } else {
              // Clamp time for non-looping animations
              if (anim.currentTime > animDuration) {
                anim.currentTime = animDuration;
              }
            }
          }
        }
        
        // Removed UpdateSkinnedMesh call - moved to GLRenderer::Render
      }
    }
  }
}

void DungeonsGame::DamagePlayer(int damage) {
  if (m_State != GameState::Dungeon && m_State != GameState::Siege && m_State != GameState::Battle)
    return;
  
  m_LastDungeonStats.playerHealth -= damage;
  if (m_LastDungeonStats.playerHealth <= 0) {
    m_LastDungeonStats.playerHealth = 0;
    m_State = GameState::GameOver;
    SDL_SetRelativeMouseMode(SDL_FALSE);
    m_MenuSelection = 0;
  }
}
