#include "BattleMode.h"
#include "../engine/Components.h"
#include "../engine/GLTFLoader.h"
#include "../engine/Input.h"
#include "../engine/TextRenderer.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace PixelsEngine;

BattleMode::BattleMode(Registry *registry, GLRenderer *renderer)
    : m_Registry(registry), m_Renderer(renderer) {
  m_SelectedAction = Melee;
}

void BattleMode::Init(Camera *camera, Entity &playerEntity) {
  m_Camera = camera;

  // Initialize Audio Manager
  m_AudioManager = std::make_unique<AudioManager>();
  auto music = m_AudioManager->LoadMusic("assets/ambience_flute.mp3");
  if (music)
    m_AudioManager->PlayMusic(music, -1);

  m_SfxJump = m_AudioManager->LoadWAV("assets/jump.wav");
  m_SfxShoot = m_AudioManager->LoadWAV("assets/bow_shoot.wav");
  m_SfxBowHit = m_AudioManager->LoadWAV("assets/bow_hit.wav");
  m_SfxSwordHit = m_AudioManager->LoadWAV("assets/sword_hit.wav");
  m_SfxSwordMiss = m_AudioManager->LoadWAV("assets/sword_miss.wav");

  // Initialize Asset Manager
  auto assetManager = std::make_unique<AssetManager>(m_Renderer);
  
  // Load character assets
  assetManager->LoadCharacter("Knight",
                               "assets/adventurers/Characters/gltf/Knight.glb",
                               "assets/adventurers/Textures/knight_texture.png");
  assetManager->LoadCharacter("Ranger",
                               "assets/adventurers/Characters/gltf/Ranger.glb",
                               "assets/adventurers/Textures/ranger_texture.png");
  assetManager->LoadCharacter("Mage",
                               "assets/adventurers/Characters/gltf/Mage.glb",
                               "assets/adventurers/Textures/mage_texture.png");
  assetManager->LoadCharacter("Rogue",
                               "assets/adventurers/Characters/gltf/Rogue.glb",
                               "assets/adventurers/Textures/rogue_texture.png");
  assetManager->LoadCharacter("Skeleton_Warrior",
                               "assets/skeletons/characters/gltf/Skeleton_Warrior.glb",
                               "assets/skeletons/texture/skeleton_texture.png");
  assetManager->LoadCharacter("Skeleton_Mage",
                               "assets/skeletons/characters/gltf/Skeleton_Mage.glb",
                               "assets/skeletons/texture/skeleton_texture.png");
  assetManager->LoadCharacter("Skeleton_Minion",
                               "assets/skeletons/characters/gltf/Skeleton_Minion.glb",
                               "assets/skeletons/texture/skeleton_texture.png");

  // Load Weapons
  m_Renderer->LoadMesh("Sword",
                       "assets/adventurers/Assets/gltf/sword_1handed.gltf");
  m_Renderer->LoadMesh("Crossbow",
                       "assets/adventurers/Assets/gltf/crossbow_1handed.gltf");
  m_Renderer->LoadMesh("Arrow",
                       "assets/adventurers/Assets/gltf/arrow_bow.gltf");
  m_Renderer->LoadMesh("Staff", "assets/adventurers/Assets/gltf/staff.gltf");
  m_Renderer->LoadMesh("Dagger", "assets/adventurers/Assets/gltf/dagger.gltf");

  // Load skeleton special animations (includes Skeletons_Death)
  m_Renderer->LoadMesh("SkeletonAnimations", "assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_Special.glb");
  
  // Copy special skeleton animations to skeleton meshes
  RenderMesh* skelAnimMesh = m_Renderer->GetRenderMesh("SkeletonAnimations");
  if (skelAnimMesh) {
    RenderMesh* skelWarriorMesh = m_Renderer->GetRenderMesh("Skeleton_Warrior");
    RenderMesh* skelMinionMesh = m_Renderer->GetRenderMesh("Skeleton_Minion");
    RenderMesh* skelMageMesh = m_Renderer->GetRenderMesh("Skeleton_Mage");
    
    if (skelWarriorMesh) skelWarriorMesh->animations = skelAnimMesh->animations;
    if (skelMinionMesh) skelMinionMesh->animations = skelAnimMesh->animations;
    if (skelMageMesh) skelMageMesh->animations = skelAnimMesh->animations;
  }

  // Load Map
  m_CurrentMapPath = "assets/saves/my_dungeon.map";
  MapLoader::LoadFromFile(m_CurrentMapPath, m_Registry,
                           m_Renderer);

  // Initialize Console and Editors
  m_Console = std::make_unique<Console>();
  m_SpawnEditor = std::make_unique<StealthSpawnEditor>();
  m_VisualSpawnEditor = std::make_unique<VisualSpawnEditor>(m_Registry, m_Renderer);
  
  // Load battle spawns
  m_SpawnEditor->LoadFromFile("assets/saves/battle_spawn_config.txt");
  
  // Spawn Units from Config
  auto spawnLocations = m_SpawnEditor->GetSpawnLocations();
  if (spawnLocations.empty()) {
      // Fallback defaults if file missing/empty
      SpawnCharacter("Knight", 12.0f, -4.0f, BattleUnitComponent::Player);
      SpawnCharacter("Ranger", 14.0f, -5.0f, BattleUnitComponent::Player);
      SpawnCharacter("Mage", 10.0f, -5.0f, BattleUnitComponent::Player);

      SpawnCharacter("Skeleton_Warrior", 12.0f, 4.0f, BattleUnitComponent::Enemy);
      SpawnCharacter("Skeleton_Mage", 15.0f, 6.0f, BattleUnitComponent::Enemy);
      SpawnCharacter("Skeleton_Minion", 9.0f, 6.0f, BattleUnitComponent::Enemy);
      
      // Seed the editor with these so they can be saved
      m_SpawnEditor->AddSpawn(12.0f, -4.0f, 3.14f, SpawnType::Player, "Knight");
      m_SpawnEditor->AddSpawn(14.0f, -5.0f, 3.14f, SpawnType::Player, "Ranger");
      m_SpawnEditor->AddSpawn(10.0f, -5.0f, 3.14f, SpawnType::Player, "Mage");
      m_SpawnEditor->AddSpawn(12.0f, 4.0f, 0.0f, SpawnType::Enemy, "Skeleton_Warrior");
      m_SpawnEditor->AddSpawn(15.0f, 6.0f, 0.0f, SpawnType::Enemy, "Skeleton_Mage");
      m_SpawnEditor->AddSpawn(9.0f, 6.0f, 0.0f, SpawnType::Enemy, "Skeleton_Minion");
  } else {
      for (const auto& loc : spawnLocations) {
          BattleUnitComponent::Team team = (loc.type == SpawnType::Player) ? 
              BattleUnitComponent::Player : BattleUnitComponent::Enemy;
          
          std::string meshName = loc.subtype;
          if (meshName.empty()) {
              meshName = (team == BattleUnitComponent::Player) ? "Knight" : "Skeleton_Warrior";
          }
          
          SpawnCharacter(meshName, loc.x, loc.y, team, loc.rotation);
      }
  }
  
  // Sync Visual Editor
  m_VisualSpawnEditor->SetSpawnLocations(m_SpawnEditor->GetSpawnLocations());

  // Register Console Commands
  m_Console->RegisterCommand("/edit", [this](const std::vector<std::string>& args) {
      bool wasActive = m_VisualSpawnEditor->IsActive();
      m_VisualSpawnEditor->Activate(!wasActive);
      if (!wasActive) {
          m_ConsoleWasOpen = m_Console->IsOpen();
          m_Console->Close();
          m_Console->SetInputEnabled(false);
          SDL_SetRelativeMouseMode(SDL_TRUE);
      } else {
          m_Console->SetInputEnabled(true);
          SDL_SetRelativeMouseMode(SDL_FALSE);
          if (m_ConsoleWasOpen) m_Console->Open();
          // Sync back changes
          auto locs = m_VisualSpawnEditor->GetSpawnLocations();
          m_SpawnEditor->SetSpawnLocations(locs);
          m_SpawnEditor->SaveToFile("assets/saves/battle_spawn_config.txt");
      }
  });
  
  m_Console->RegisterCommand("/save", [this](const std::vector<std::string>& args) {
      if (m_VisualSpawnEditor->IsActive()) {
          auto locs = m_VisualSpawnEditor->GetSpawnLocations();
          m_SpawnEditor->SetSpawnLocations(locs);
      }
      m_SpawnEditor->SaveToFile("assets/saves/battle_spawn_config.txt");
      m_Console->AddLog("Saved battle config.");
  });
  
  m_Console->RegisterCommand("/load", [this](const std::vector<std::string>& args) {
      m_SpawnEditor->LoadFromFile("assets/saves/battle_spawn_config.txt");
      m_VisualSpawnEditor->SetSpawnLocations(m_SpawnEditor->GetSpawnLocations());
      m_Console->AddLog("Loaded battle config (restart to apply).");
  });
  
  m_Console->RegisterCommand("/help", [this](const std::vector<std::string>& args) {
      m_Console->AddLog("Available commands:");
      m_Console->AddLog("  /edit - Toggle Visual Spawn Editor");
      m_Console->AddLog("  /save - Save spawn config");
      m_Console->AddLog("  /load - Load spawn config");
      m_Console->AddLog("  /creative - Edit current level map");
      m_Console->AddLog("  /help - Show this help");
  });

  m_Console->RegisterCommand("/creative", [this](const std::vector<std::string>& args) {
      m_RequestedCreative = true;
      m_RequestedMapPath = m_CurrentMapPath;
      m_Console->AddLog("Switching to Creative Mode with: " + m_CurrentMapPath);
  });

  // 4. Setup Camera
  if (m_Camera) {
    m_Camera->x = 12.0f;
    m_Camera->y = 0.0f;
    m_Camera->z = 12.0f;
    m_Camera->yaw = M_PI * 0.5f;
    m_Camera->pitch = -1.0f; // Look down
  }

  StartBattle();

  if (!m_TurnOrder.empty())
    playerEntity = m_TurnOrder[0];
}

bool BattleMode::isMouseOverUI(int mx, int my, int w, int h) {
  // Action bar is centered at bottom
  int btnW = 80;
  int btnH = 80;
  int gap = 10;
  int count = 7;
  int barW = count * (btnW + gap);
  int startX = (w - barW) / 2;
  int startY = h - btnH - 20;

  if (mx >= startX && mx <= startX + barW && my >= startY && my <= h)
    return true;

  // Turn order is at top
  int turnOrderH = 80;
  if (my <= turnOrderH)
    return true;

  return false;
}

bool BattleMode::IsPositionOccupied(float x, float y, Entity ignoreEntity) {
  auto &units = m_Registry->View<BattleUnitComponent>();
  for (auto &pair : units) {
    Entity e = pair.first;
    if (e == ignoreEntity)
      continue;

    auto &u = pair.second;
    if (u.hp <= 0)
      continue; // Don't collide with dead

    auto *t = m_Registry->GetComponent<Transform3DComponent>(e);
    if (t) {
      float dist = sqrt(pow(x - t->x, 2) + pow(y - t->y, 2));
      if (dist < 1.2f)
        return true; // Collision radius
    }
  }
  return false;
}

void BattleMode::LoadMap(const std::string &path) {
  MapLoader::LoadFromFile(path, m_Registry, m_Renderer);
}

PixelsEngine::Entity BattleMode::SpawnCharacter(const std::string &mesh, float x, float y,
                                BattleUnitComponent::Team team, float rotation) {
  auto e = m_Registry->CreateEntity();
  std::string tex = "Knight_tex";
  if (mesh == "Ranger")
    tex = "Ranger_tex";
  if (mesh == "Mage")
    tex = "Mage_tex";
  if (mesh == "Rogue")
    tex = "Rogue_tex";
  if (mesh.find("Skeleton") != std::string::npos)
    tex = mesh + "_tex";  // Use the actual skeleton name with _tex suffix

  // Use provided rotation if >= 0, otherwise use team default
  float initialRot = rotation;
  if (initialRot < 0.0f) {
      initialRot = (team == BattleUnitComponent::Player) ? 3.14f : 0.0f;
  }
  
  m_Registry->AddComponent<Transform3DComponent>(
      e, {x, y, 0.0f, initialRot, 0.0f});
  m_Registry->AddComponent<MeshComponent>(e, {mesh, tex, 1.0f, 1.0f, 1.0f});

  RenderMesh *rm = m_Renderer->GetRenderMesh(mesh);
  if (rm && rm->isSkinned) {
    int idleIdx = 0;
    for (size_t i = 0; i < rm->animations.size(); ++i) {
      if (rm->animations[i].name.find("Idle") != std::string::npos) {
        idleIdx = (int)i;
        break;
      }
    }
    m_Registry->AddComponent<SkeletalAnimationComponent>(
        e, {idleIdx, (float)(rand() % 100) * 0.01f, 1.0f});
  }

  BattleUnitComponent unit;
  unit.team = team;
  unit.hp = (team == BattleUnitComponent::Enemy) ? 10 : 20;
  unit.maxHp = unit.hp;
  unit.ac = (team == BattleUnitComponent::Enemy) ? 10 : 12;
  unit.movement = 6.0f;
  unit.currentMovement = 6.0f;
  unit.initiative = rand() % 20 + 1;
  m_Registry->AddComponent<BattleUnitComponent>(e, unit);
  m_Registry->AddComponent<ColliderComponent>(e, {0.5f, 2.0f, true});

  // Attach Weapons
  AttachmentComponent attach;
  attach.textureName = tex;
  attach.scale = 1.0f;
  attach.boneName = "hand.r";
  attach.rotX = -7.63f;
  attach.rotY = 18.148f;
  attach.rotZ = 1.702f;

  if (mesh == "Knight" || mesh == "Skeleton_Warrior") {
    attach.meshName = "Sword";
    m_Registry->AddComponent<AttachmentComponent>(e, attach);
  } else if (mesh == "Ranger") {
    attach.meshName = "Crossbow";
    attach.rotX = -16.896f;
    attach.rotY = 30.052f;
    attach.rotZ = 11.404f;
    m_Registry->AddComponent<AttachmentComponent>(e, attach);
  } else if (mesh == "Mage" || mesh == "Skeleton_Mage") {
    attach.meshName = "Staff";
    m_Registry->AddComponent<AttachmentComponent>(e, attach);
  } else if (mesh == "Rogue" || mesh == "Skeleton_Minion") {
    attach.meshName = "Dagger";
    m_Registry->AddComponent<AttachmentComponent>(e, attach);
  }
  
  return e;
}

void BattleMode::StartBattle() {
  m_TurnOrder.clear();
  auto view = m_Registry->View<BattleUnitComponent>();
  for (auto &pair : view)
    m_TurnOrder.push_back(pair.first);

  std::sort(m_TurnOrder.begin(), m_TurnOrder.end(), [&](Entity a, Entity b) {
    auto *ua = m_Registry->GetComponent<BattleUnitComponent>(a);
    auto *ub = m_Registry->GetComponent<BattleUnitComponent>(b);
    return ua->initiative > ub->initiative;
  });

  m_CurrentTurnIndex = -1;
  NextTurn();
}

void BattleMode::NextTurn() {
  m_CurrentTurnIndex = (m_CurrentTurnIndex + 1) % m_TurnOrder.size();
  Entity current = m_TurnOrder[m_CurrentTurnIndex];
  auto *unit = m_Registry->GetComponent<BattleUnitComponent>(current);

  if (!unit || unit->hp <= 0) {
    NextTurn();
    return;
  } // Skip dead

  unit->currentMovement = unit->movement;
  unit->hasAction = true;
  unit->hasBonusAction = true;
  unit->isTurn = true;

  // Deselect others turn flag
  for (auto e : m_TurnOrder) {
    if (e != current) {
      auto *u = m_Registry->GetComponent<BattleUnitComponent>(e);
      if (u)
        u->isTurn = false;
    }
  }

  if (unit->team == BattleUnitComponent::Player) {
    m_State = PlayerTurn;
    std::cout << "Turn: Player Unit " << current << std::endl;
  } else {
    m_State = EnemyTurn;
    std::cout << "Turn: Enemy Unit " << current << std::endl;
  }
  m_Cursor.active = false;
  m_SelectedAction = Move; // Default to move
}

void BattleMode::Update(float dt, Entity playerEntity) {
  // Update Console
  if (m_Console) {
      if (Input::IsKeyPressed(SDL_SCANCODE_GRAVE)) {
          m_Console->Toggle();
          bool open = m_Console->IsOpen();
          m_Console->SetInputEnabled(open);
          if (open) {
              Input::StartTextInput();
              SDL_SetRelativeMouseMode(SDL_FALSE);
          } else {
              Input::StopTextInput();
              // Battle mode usually doesn't use relative mouse mode, but let's be safe
              SDL_SetRelativeMouseMode(SDL_FALSE);
          }
      }
      
      if (m_Console->IsOpen()) {
          m_Console->ProcessInput();
          return;
      }
  }
  
  if (m_VisualSpawnEditor) {
      // Need screen width/height for editor raycast logic
      int w = 1280, h = 720;
      SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &w, &h);
      m_VisualSpawnEditor->Update(dt, m_Camera, w, h);
      if (m_VisualSpawnEditor->IsActive()) return;
  }

  if (m_State == Victory)
    return;

  // Update door states and animations
  UpdateDoors(dt);

  if (m_Camera) {
    float speed = 10.0f * dt;
    if (Input::IsKeyDown(SDL_SCANCODE_W))
      m_Camera->y += speed;
    if (Input::IsKeyDown(SDL_SCANCODE_S))
      m_Camera->y -= speed;
    if (Input::IsKeyDown(SDL_SCANCODE_A))
      m_Camera->x -= speed;
    if (Input::IsKeyDown(SDL_SCANCODE_D))
      m_Camera->x += speed;
  }

  CheckVictory();

  if (m_State == Moving) {
    auto *t = m_Registry->GetComponent<Transform3DComponent>(m_AnimState.actor);
    if (t) {
      m_AnimState.timer += dt;
      float p = std::min(1.0f, m_AnimState.timer / m_AnimState.duration);
      
      float nextX = m_AnimState.startX + (m_AnimState.targetX - m_AnimState.startX) * p;
      float nextY = m_AnimState.startY + (m_AnimState.targetY - m_AnimState.startY) * p;
      
      // Collision check for unit movement
      if (CheckCollision(nextX, nextY, t->z, m_AnimState.actor)) {
          // Stop movement if blocked
          p = 1.0f;
          m_AnimState.timer = m_AnimState.duration;
      } else {
          t->x = nextX;
          t->y = nextY;
      }

      if (m_SelectedAction == Jump) {
        float h = sin(p * M_PI) * 2.0f;
        t->z = h;
      }

      if (p >= 1.0f) {
        t->z = 0.0f;
        // Revert to Idle animation
        auto *animComp = m_Registry->GetComponent<SkeletalAnimationComponent>(
            m_AnimState.actor);
        auto *mesh = m_Registry->GetComponent<MeshComponent>(m_AnimState.actor);
        if (animComp && mesh) {
          RenderMesh *rm = m_Renderer->GetRenderMesh(mesh->meshName);
          if (rm) {
            for (size_t i = 0; i < rm->animations.size(); i++) {
              if (rm->animations[i].name.find("Idle") != std::string::npos) {
                animComp->animationIndex = (int)i;
                animComp->currentTime = 0.0f;
                animComp->loop = true;
                break;
              }
            }
          }
        }

        auto *unit =
            m_Registry->GetComponent<BattleUnitComponent>(m_AnimState.actor);
        if (unit && unit->team == BattleUnitComponent::Player)
          m_State = PlayerTurn;
        else
          m_State = EnemyTurn;
        m_SelectedAction = None;
      }
    }
  } else if (m_State == Attacking) {
    m_AnimState.timer += dt;
    float p = std::min(1.0f, m_AnimState.timer / m_AnimState.duration);

    // Update Projectile
    if (m_ActiveProjectile.entity != -1) {
      auto *pt = m_Registry->GetComponent<Transform3DComponent>(
          m_ActiveProjectile.entity);
      if (pt) {
        float nextX = m_ActiveProjectile.startX +
                (m_ActiveProjectile.targetX - m_ActiveProjectile.startX) * p;
        float nextY = m_ActiveProjectile.startY +
                (m_ActiveProjectile.targetY - m_ActiveProjectile.startY) * p;
        float nextZ = m_ActiveProjectile.startZ +
                (m_ActiveProjectile.targetZ - m_ActiveProjectile.startZ) * p;
        
        // Wall collision for projectile
        if (CheckCollision(nextX, nextY, nextZ, m_AnimState.actor)) {
            m_Registry->DestroyEntity(m_ActiveProjectile.entity);
            m_ActiveProjectile.entity = -1;
            m_AnimState.timer = m_AnimState.duration; // End animation
        } else {
            pt->x = nextX;
            pt->y = nextY;
            pt->z = nextZ;
        }
      }
    }

    if (m_AnimState.timer >= m_AnimState.duration) {
      // Cleanup Projectile
      if (m_ActiveProjectile.entity != -1) {
        m_Registry->DestroyEntity(m_ActiveProjectile.entity);
        m_ActiveProjectile.entity = -1;
      }

      // Revert to Idle
      auto *animComp = m_Registry->GetComponent<SkeletalAnimationComponent>(
          m_AnimState.actor);
      auto *mesh = m_Registry->GetComponent<MeshComponent>(m_AnimState.actor);
      if (animComp && mesh) {
        RenderMesh *rm = m_Renderer->GetRenderMesh(mesh->meshName);
        if (rm) {
          for (size_t i = 0; i < rm->animations.size(); i++) {
            if (rm->animations[i].name.find("Idle") != std::string::npos) {
              animComp->animationIndex = (int)i;
              animComp->currentTime = 0.0f;
              animComp->loop = true;
              break;
            }
          }
        }
      }
      auto *unit =
          m_Registry->GetComponent<BattleUnitComponent>(m_AnimState.actor);
      if (unit && unit->team == BattleUnitComponent::Player)
        m_State = PlayerTurn;
      else
        m_State = EnemyTurn;
    }
  } else if (m_State == PlayerTurn) {
    HandlePlayerInput();

    Entity current = m_TurnOrder[m_CurrentTurnIndex];
    auto *unit = m_Registry->GetComponent<BattleUnitComponent>(current);
    auto *t = m_Registry->GetComponent<Transform3DComponent>(current);
    auto *attach = m_Registry->GetComponent<AttachmentComponent>(current);

    // Update preferred action if user explicitly toggled Melee or Ranged
    if (unit && (m_SelectedAction == Melee || m_SelectedAction == Ranged)) {
      unit->preferredAction = (int)m_SelectedAction;
    }

    if (t && m_Cursor.active) {
      t->rot = atan2(t->x - m_Cursor.x, t->y - m_Cursor.y);
    }

    if (attach && unit) {
      if (unit->preferredAction == (int)Ranged) {
        attach->meshName = "Crossbow";
        attach->rotX = -16.896f;
        attach->rotY = 30.052f;
        attach->rotZ = 11.404f;
      } else {
        attach->meshName = "Sword";
        attach->rotX = -7.63f;
        attach->rotY = 18.148f;
        attach->rotZ = 1.702f;
      }
    }
  } else if (m_State == EnemyTurn) {
    UpdateAI(dt);
  }

  auto &skeletalView = m_Registry->View<SkeletalAnimationComponent>();
  static int frameCounter = 0;
  frameCounter++;
  bool debugFrame = (frameCounter % 60 == 0);  // Debug every 60 frames
  
  for (auto &pair : skeletalView) {
    Entity e = pair.first;
    auto &anim = pair.second;
    auto *mesh = m_Registry->GetComponent<MeshComponent>(e);
    if (mesh) {
      RenderMesh *rm = m_Renderer->GetRenderMesh(mesh->meshName);
      if (debugFrame && rm) {
        std::cout << "[DEBUG] Frame " << frameCounter << ": Entity " << e << " mesh=" << mesh->meshName << " animIdx=" << anim.animationIndex << " time=" << anim.currentTime << " loop=" << anim.loop << std::endl;
      }
      if (rm && rm->isSkinned) {
        // Handle animation looping properly
        if (anim.animationIndex >= 0 && 
            anim.animationIndex < (int)rm->animations.size()) {
          float animDuration = rm->animations[anim.animationIndex].duration;
          if (animDuration > 0) {
            anim.currentTime += dt * anim.speed;
            if (anim.loop) {
              // Wrap time for looping animations
              if (anim.currentTime >= animDuration) {
                anim.currentTime = fmod(anim.currentTime, animDuration);
              }
            } else {
              // Clamp time for non-looping animations
              if (anim.currentTime > animDuration) {
                anim.currentTime = animDuration;
              }
            }
          } else {
            anim.currentTime += dt * anim.speed;
          }
        } else {
          anim.currentTime += dt * anim.speed;
        }
        m_Renderer->UpdateSkinnedMesh(*rm, anim.animationIndex,
                                      anim.currentTime);
      }
    }
  }

  // Decay flash amount
  auto &unitView = m_Registry->View<BattleUnitComponent>();
  for (auto &pair : unitView) {
    if (pair.second.flashAmount > 0) {
      pair.second.flashAmount -= dt * 5.0f;
      if (pair.second.flashAmount < 0)
        pair.second.flashAmount = 0;
    }
  }
}

void BattleMode::RenderWorld(GLRenderer *renderer) {
  // 1. Draw turn indicator circle at base of current unit
  if (m_CurrentTurnIndex >= 0 && m_CurrentTurnIndex < (int)m_TurnOrder.size()) {
    Entity currentTurnEntity = m_TurnOrder[m_CurrentTurnIndex];
    auto *currentTrans =
        m_Registry->GetComponent<Transform3DComponent>(currentTurnEntity);
    auto *currentUnit =
        m_Registry->GetComponent<BattleUnitComponent>(currentTurnEntity);
    if (currentTrans && currentUnit) {
      SDL_Color circleColor = (currentUnit->team == BattleUnitComponent::Player)
                                  ? SDL_Color{255, 255, 255, 255}
                                  : SDL_Color{255, 50, 50, 255};
      // Firmer circle for turn unit
      renderer->DrawWireCircle(currentTrans->x, currentTrans->y, 0.02f, 0.85f,
                               circleColor, 5.0f);
    }
  }

  if (m_State == PlayerTurn && m_Cursor.active) {
    Entity current = m_TurnOrder[m_CurrentTurnIndex];
    auto *t = m_Registry->GetComponent<Transform3DComponent>(current);
    auto *unit = m_Registry->GetComponent<BattleUnitComponent>(current);

    // 2. Hover indicator
    if (m_Cursor.hoveredEntity != -1) {
      auto *ht = m_Registry->GetComponent<Transform3DComponent>(
          m_Cursor.hoveredEntity);
      auto *targetUnit =
          m_Registry->GetComponent<BattleUnitComponent>(m_Cursor.hoveredEntity);

      if (ht && targetUnit) {
        SDL_Color hoverColor = (targetUnit->team == BattleUnitComponent::Player)
                                   ? SDL_Color{255, 255, 255, 255}
                                   : SDL_Color{255, 50, 50, 255};
        // Thicker hover circle
        renderer->DrawWireCircle(ht->x, ht->y, 0.05f, 0.95f, hoverColor, 3.0f);
      }
    }

    // 3. Movement range indicators
    if (t && unit) {
      float dist = sqrt(pow(m_Cursor.x - t->x, 2) + pow(m_Cursor.y - t->y, 2));
      float maxRange = unit->currentMovement;
      bool isJump = (m_SelectedAction == Jump);
      bool isDash = (m_SelectedAction == Dash);
      if (isJump)
        maxRange = 5.0f;

      // Draw Jump Circle
      if (isJump) {
        renderer->DrawWireCircle(t->x, t->y, 0.01f, maxRange,
                                 {100, 100, 255, 150}, 2.0f);
      }

      // Draw line to cursor
      if (m_SelectedAction == Move || m_SelectedAction == Jump ||
          m_SelectedAction == Dash || m_SelectedAction == Melee ||
          m_SelectedAction == Ranged || m_SelectedAction == Shove) {

        float attackRange = 0.0f;
        if (m_SelectedAction == Melee || m_SelectedAction == Shove)
          attackRange = 2.0f;
        else if (m_SelectedAction == Ranged)
          attackRange = 15.0f;

        float effectiveRange = (attackRange > 0.0f) ? attackRange : maxRange;

        SDL_Color lineColor = (dist <= effectiveRange)
                                  ? SDL_Color{255, 255, 255, 255}
                                  : SDL_Color{255, 50, 50, 255};
        renderer->DrawLine(t->x, t->y, 0.1f, m_Cursor.x, m_Cursor.y, 0.1f,
                           lineColor, 2.0f);

        // Cursor marker
        renderer->DrawWireCircle(m_Cursor.x, m_Cursor.y, 0.05f, 0.3f, lineColor,
                                 2.0f);
      }
    }
  }
}

void BattleMode::HandlePlayerInput() {
  // std::cout << "HandlePlayerInput" << std::endl;
  RaycastCursor();

  int mx, my;
  Input::GetMousePosition(mx, my);
  if (isMouseOverUI(mx, my, 1280, 720))
    return;

  Entity current = m_TurnOrder[m_CurrentTurnIndex];
  auto *unit = m_Registry->GetComponent<BattleUnitComponent>(current);
  auto *t = m_Registry->GetComponent<Transform3DComponent>(current);
  if (!unit)
    return;

  // Baldur's Gate 3 style keybinds
  if (Input::IsKeyPressed(SDL_SCANCODE_Z))
    m_SelectedAction = Jump;
  if (Input::IsKeyPressed(SDL_SCANCODE_C))
    unit->isSneaking = !unit->isSneaking;
  if (Input::IsKeyPressed(SDL_SCANCODE_V))
    m_SelectedAction = Shove;

  // F to toggle attack modes
  if (Input::IsKeyPressed(SDL_SCANCODE_F)) {
    if (m_SelectedAction == Melee)
      m_SelectedAction = Ranged;
    else if (m_SelectedAction == Ranged)
      m_SelectedAction = Melee;
    else
      m_SelectedAction = Melee;
  }

  if (Input::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
    bool ctrl = Input::IsKeyDown(SDL_SCANCODE_LCTRL) ||
                Input::IsKeyDown(SDL_SCANCODE_RCTRL);

    // Ctrl + Click to attack hovered target using preferred weapon
    if (ctrl && m_Cursor.hoveredEntity != -1) {
      auto *target =
          m_Registry->GetComponent<BattleUnitComponent>(m_Cursor.hoveredEntity);
      if (target && target->team == BattleUnitComponent::Enemy &&
          target->hp > 0 && unit->hasAction) {
        float dist = sqrt(pow(t->x - m_Registry
                                         ->GetComponent<Transform3DComponent>(
                                             m_Cursor.hoveredEntity)
                                         ->x,
                              2) +
                          pow(t->y - m_Registry
                                         ->GetComponent<Transform3DComponent>(
                                             m_Cursor.hoveredEntity)
                                         ->y,
                              2));

        ActionType quickAction = (ActionType)unit->preferredAction;
        float range = (quickAction == Melee) ? 2.0f : 15.0f;
        if (dist <= range) {
          ExecuteAction(current, quickAction, m_Cursor.hoveredEntity, 0, 0);
          m_SelectedAction = Move; // Reset to default move
        }
        return;
      }
    }

    // Allow movement if Move, Jump, or Melee is selected
    if (m_SelectedAction == Move || m_SelectedAction == Jump ||
        m_SelectedAction == Melee) {
      if (m_Cursor.hoveredEntity == -1 &&
          !IsPositionOccupied(m_Cursor.x, m_Cursor.y, current) &&
          !CheckCollision(m_Cursor.x, m_Cursor.y, t->z, current)) {
        float dist =
            sqrt(pow(m_Cursor.x - t->x, 2) + pow(m_Cursor.y - t->y, 2));
        float maxRange =
            (m_SelectedAction == Jump) ? 5.0f : unit->currentMovement;

        if (dist <= maxRange) {
          if (m_SelectedAction == Jump && m_SfxJump)
            Mix_PlayChannel(-1, m_SfxJump, 0);
          if (m_SelectedAction != Jump)
            unit->currentMovement -= dist;
          m_AnimState.actor = current;
          m_AnimState.startX = t->x;
          m_AnimState.startY = t->y;
          m_AnimState.targetX = m_Cursor.x;
          m_AnimState.targetY = m_Cursor.y;
          m_AnimState.duration = dist / 5.0f;
          m_AnimState.timer = 0.0f;
          m_State = Moving;
          t->rot = atan2(t->x - m_Cursor.x, t->y - m_Cursor.y);

          // Set Run Animation
          auto *animComp =
              m_Registry->GetComponent<SkeletalAnimationComponent>(current);
          auto *mesh = m_Registry->GetComponent<MeshComponent>(current);
          if (animComp && mesh) {
            RenderMesh *rm = m_Renderer->GetRenderMesh(mesh->meshName);
            if (rm) {
              for (size_t i = 0; i < rm->animations.size(); i++) {
                if (rm->animations[i].name.find("Run") != std::string::npos ||
                    rm->animations[i].name.find("Walk") != std::string::npos) {
                  animComp->animationIndex = (int)i;
                  animComp->currentTime = 0.0f;
                  animComp->loop = true;  // Walking should loop
                  animComp->speed = 1.0f;  // Normal speed
                  break;
                }
              }
            }
          }
        }
      }
    }

    if (m_Cursor.hoveredEntity != -1 && unit->hasAction) {
      auto *target =
          m_Registry->GetComponent<BattleUnitComponent>(m_Cursor.hoveredEntity);
      if (target && target->team == BattleUnitComponent::Enemy && target->hp > 0) {
        float dist = sqrt(pow(t->x - m_Registry
                                         ->GetComponent<Transform3DComponent>(
                                             m_Cursor.hoveredEntity)
                                         ->x,
                              2) +
                          pow(t->y - m_Registry
                                         ->GetComponent<Transform3DComponent>(
                                             m_Cursor.hoveredEntity)
                                         ->y,
                              2));

        if (m_SelectedAction == Melee) {
          if (dist < 2.0f) {
            ExecuteAction(current, Melee, m_Cursor.hoveredEntity, 0, 0);
            m_SelectedAction = Melee;
          }
        } else if (m_SelectedAction == Ranged) {
          if (dist < 15.0f) {
            ExecuteAction(current, Ranged, m_Cursor.hoveredEntity, 0, 0);
            m_SelectedAction = Melee;
          }
        } else if (m_SelectedAction == Shove) {
          if (dist < 2.0f) {
            ExecuteAction(current, Shove, m_Cursor.hoveredEntity, 0, 0);
            m_SelectedAction = Melee;
          }
        }
      }
    }
  }

  if (Input::IsKeyPressed(SDL_SCANCODE_SPACE)) {
    NextTurn();
  }
}

void BattleMode::ExecuteAction(Entity actor, ActionType action, Entity target,
                               float x, float y) {
  std::cout << "ExecuteAction: " << action << " on Target: " << target
            << std::endl;
  auto *uActor = m_Registry->GetComponent<BattleUnitComponent>(actor);
  auto *uTarget = target != -1
                      ? m_Registry->GetComponent<BattleUnitComponent>(target)
                      : nullptr;
  auto *tActor = m_Registry->GetComponent<Transform3DComponent>(actor);
  auto *tTarget = target != -1
                      ? m_Registry->GetComponent<Transform3DComponent>(target)
                      : nullptr;

  // Rotate actor to face target
  if (tActor && tTarget) {
    tActor->rot = atan2(tActor->x - tTarget->x, tActor->y - tTarget->y);
  }

  if (action == Ranged) {
    if (m_SfxShoot)
      Mix_PlayChannel(-1, m_SfxShoot, 0);

    // Spawn Arrow Projectile
    if (tActor && tTarget) {
      auto arrow = m_Registry->CreateEntity();
      m_Registry->AddComponent<Transform3DComponent>(
          arrow, {tActor->x, tActor->y, 1.0f, tActor->rot, 0});
      // Create a simple arrow projectile with scaling
      m_Registry->AddComponent<MeshComponent>(arrow,
                                              {"Arrow", "", 0.3f, 0.3f, 0.3f});
      m_Registry->AddComponent<ProceduralAnimationComponent>(arrow, {0.0f, 1.0f, 0.3f, 0.0f, 0.0f});

      m_ActiveProjectile.entity = arrow;
      m_ActiveProjectile.startX = tActor->x;
      m_ActiveProjectile.startY = tActor->y;
      m_ActiveProjectile.startZ = 1.0f;
      m_ActiveProjectile.targetX = tTarget->x;
      m_ActiveProjectile.targetY = tTarget->y;
      m_ActiveProjectile.targetZ = 1.0f;
    }
  }

  // Animation Trigger
  auto *mesh = m_Registry->GetComponent<MeshComponent>(actor);
  auto *animComp = m_Registry->GetComponent<SkeletalAnimationComponent>(actor);
  if (mesh && animComp) {
    RenderMesh *rm = m_Renderer->GetRenderMesh(mesh->meshName);
    if (rm) {
      std::string animName = "Idle";
      if (action == Melee)
        animName = "Melee_1H_Attack_Chop";
      else if (action == Ranged)
        animName = "Ranged_Bow_Shoot";
      else if (action == Jump)
        animName = "Jump_Full_Long";
      else if (action == Shove)
        animName = "Melee_Unarmed_Attack_Kick";
      else if (action == Sneak)
        animName = "Sneaking";

      std::cout << "[ACTION] Looking for animation '" << animName << "' on " << mesh->meshName << " with " << rm->animations.size() << " available animations" << std::endl;
      
      // Search for best match
      int bestIdx = -1;
      for (size_t i = 0; i < rm->animations.size(); i++) {
        std::cout << "  [CHECK] Animation " << i << ": " << rm->animations[i].name << std::endl;
        if (rm->animations[i].name.find(animName) != std::string::npos) {
          bestIdx = (int)i;
          std::cout << "  [MATCH] Found at index " << bestIdx << std::endl;
          break;
        }
      }

      // Fallback for attack animations
      if (bestIdx == -1) {
        for (size_t i = 0; i < rm->animations.size(); i++) {
          if (rm->animations[i].name.find("Attack") != std::string::npos) {
            bestIdx = (int)i;
            break;
          }
        }
      }

      if (bestIdx != -1) {
        animComp->animationIndex = bestIdx;
        animComp->currentTime = 0.0f;
        animComp->loop = false;  // Action animations should not loop
        animComp->speed = 1.0f;  // Ensure normal playback speed

        // Set up Attacking state to wait for animation
        m_State = Attacking;
        m_AnimState.actor = actor;
        m_AnimState.timer = 0.0f;
        // Speed up ranged attacks (fixed 0.1s travel time)
        m_AnimState.duration =
            (action == Ranged) ? 0.1f : rm->animations[bestIdx].duration;
        if (m_AnimState.duration <= 0)
          m_AnimState.duration = 1.0f;
        
        std::cout << "[DEBUG] Animation triggered: " << animName << " (index " << bestIdx << ", name=" << rm->animations[bestIdx].name << ", duration " << m_AnimState.duration << ", loop=" << animComp->loop << ")" << std::endl;
      } else {
        std::cout << "No animation found for: " << animName << std::endl;
      }
    }
  }

  if (action == Melee || action == Ranged) {
    int roll = rand() % 20 + 1;
    int mod = 5;
    if (uActor->isSneaking) {
      roll = std::max(roll, rand() % 20 + 1);
      uActor->isSneaking = false;
    }
    if (roll + mod >= uTarget->ac) {
      int dmg = rand() % 8 + 3;
      uTarget->hp -= dmg;
      uTarget->flashAmount = 1.0f; // Flash red!

      if (action == Melee) {
        if (m_SfxSwordHit)
          Mix_PlayChannel(-1, m_SfxSwordHit, 0);
      } else {
        if (m_SfxBowHit)
          Mix_PlayChannel(-1, m_SfxBowHit, 0);
      }

      std::cout << "Hit! Damage: " << dmg << std::endl;
      if (uTarget->hp <= 0) {
        // Play death animation instead of immediately destroying
        auto *targetMesh = m_Registry->GetComponent<MeshComponent>(target);
        auto *targetAnim = m_Registry->GetComponent<SkeletalAnimationComponent>(target);
        if (targetMesh && targetAnim) {
          RenderMesh *trm = m_Renderer->GetRenderMesh(targetMesh->meshName);
          if (trm) {
            // Skeletons have special death animation where legs fall off
            std::string deathAnim = (targetMesh->meshName.find("Skeleton") != std::string::npos) ? "Skeletons_Death" : "Death_A";
            for (size_t i = 0; i < trm->animations.size(); i++) {
              if (trm->animations[i].name.find(deathAnim) != std::string::npos) {
                targetAnim->animationIndex = (int)i;
                targetAnim->currentTime = 0.0f;
                targetAnim->loop = false; // Don't loop death animation
                targetAnim->speed = 1.0f; // Ensure speed is set
                break;
              }
            }
          }
        }
      }
    } else {
      if (action == Melee) {
        if (m_SfxSwordMiss)
          Mix_PlayChannel(-1, m_SfxSwordMiss, 0);
      }
      std::cout << "Miss!" << std::endl;
    }
    uActor->hasAction = false;
  } else if (action == Shove) {
    if (tActor && tTarget) {
      float angle = atan2(tTarget->y - tActor->y, tTarget->x - tActor->x);
      tTarget->x += cos(angle) * 2.0f;
      tTarget->y += sin(angle) * 2.0f;
      uTarget->flashAmount = 0.5f;
      if (m_SfxSwordHit)
        Mix_PlayChannel(-1, m_SfxSwordHit, 0); // Reuse hit for shove impact
    }
    uActor->hasAction = false;
  }
}

void BattleMode::RenderUI(GLRenderer *ren, TextRenderer *tr, int w, int h) {
  if (m_State == Victory) {
    ren->DrawRect2D(0, 0, w, h, {0, 0, 0, 150});
    tr->RenderTextCentered(ren, "VICTORY!", w / 2, h / 2 - 20,
                           {255, 255, 255, 255});
    tr->RenderTextCentered(ren, "All enemies defeated.", w / 2, h / 2 + 20,
                           {200, 200, 200, 255});
    return;
  }

  DrawTurnOrder(ren, tr, w, h);
  if (m_State == PlayerTurn) {
    DrawActionBar(ren, tr, w, h);
  }
  
  // Render Editor UI
  if (m_Console) m_Console->Render(ren, tr, w, h);
  if (m_SpawnEditor) m_SpawnEditor->Render(ren, tr, w, h);
  if (m_VisualSpawnEditor) m_VisualSpawnEditor->Render(ren, tr, m_Camera, w, h);
}

void BattleMode::CheckVictory() {
  bool enemyAlive = false;
  auto &units = m_Registry->View<BattleUnitComponent>();
  for (auto &pair : units) {
    if (pair.second.team == BattleUnitComponent::Enemy && pair.second.hp > 0) {
      enemyAlive = true;
      break;
    }
  }
  if (!enemyAlive) {
    m_State = Victory;
  }
}

void BattleMode::DrawActionBar(GLRenderer *ren, TextRenderer *tr, int w,
                               int h) {
  int btnW = 80;
  int btnH = 80;
  int gap = 10;
  int count = 7;
  int startX = (w - (count * (btnW + gap))) / 2;
  int y = h - btnH - 20;

  struct ActionBtn {
    ActionType type;
    std::string label;
    SDL_Color color;
    std::string key;
  };

  std::vector<ActionBtn> btns = {{Move, "Move", {50, 150, 50, 255}, ""},
                                 {Melee, "Melee", {150, 50, 50, 255}, "F"},
                                 {Ranged, "Ranged", {150, 100, 50, 255}, "F"},
                                 {Dash, "Dash", {50, 100, 150, 255}, ""},
                                 {Jump, "Jump", {100, 50, 150, 255}, "Z"},
                                 {Sneak, "Sneak", {50, 50, 50, 255}, "C"},
                                 {Shove, "Shove", {150, 150, 50, 255}, "V"}};

  int mx, my;
  Input::GetMousePosition(mx, my);
  bool click = Input::IsMouseButtonPressed(SDL_BUTTON_LEFT);

  Entity current = m_TurnOrder[m_CurrentTurnIndex];
  auto *unit = m_Registry->GetComponent<BattleUnitComponent>(current);

  for (int i = 0; i < count; i++) {
    int x = startX + i * (btnW + gap);
    bool hover = (mx >= x && mx <= x + btnW && my >= y && my <= y + btnH);
    bool selected = (m_SelectedAction == btns[i].type);
    if (btns[i].type == Sneak && unit && unit->isSneaking)
      selected = true;

    SDL_Color c = btns[i].color;
    if (selected) {
      c.r = std::min(255, c.r + 50);
      c.g = std::min(255, c.g + 50);
      c.b = std::min(255, c.b + 50);
    } else if (hover) {
      c.r = std::min(255, c.r + 30);
      c.g = std::min(255, c.g + 30);
      c.b = std::min(255, c.b + 30);
    }

    ren->DrawRect2D(x, y, btnW, btnH, c);
    tr->RenderTextCentered(ren, btns[i].label, x + btnW / 2, y + btnH / 2,
                           {255, 255, 255, 255});

    // Render Keybind letter in bottom left
    if (!btns[i].key.empty()) {
      tr->RenderText(ren, btns[i].key, x + 5, y + btnH - 20,
                     {200, 200, 200, 255});
    }

    if (click && hover) {
      if (btns[i].type == Dash && unit && unit->hasAction) {
        unit->currentMovement += unit->movement;
        unit->hasAction = false;
      } else if (btns[i].type == Sneak && unit) {
        unit->isSneaking = !unit->isSneaking;
      } else {
        m_SelectedAction = btns[i].type;
      }
    }
  }

  if (m_SelectedAction != None) {
    tr->RenderTextCentered(ren,
                           "Selected: " + btns[(int)m_SelectedAction - 1].label,
                           w / 2, y - 30, {255, 255, 255, 255});
  }
}

void BattleMode::DrawTurnOrder(GLRenderer *ren, TextRenderer *tr, int w,
                               int h) {
  struct RenderUnit {
    Entity e;
    bool isCurrent;
  };
  std::vector<RenderUnit> toRender;
  for (int i = 0; i < (int)m_TurnOrder.size(); i++) {
    Entity e = m_TurnOrder[i];
    if (!m_Registry->Valid(e))
      continue; // Skip destroyed
    auto *u = m_Registry->GetComponent<BattleUnitComponent>(e);
    // Only render if unit exists and is alive
    if (u && u->hp > 0) {
      toRender.push_back({e, i == m_CurrentTurnIndex});
    }
  }

  if (toRender.empty())
    return;

  int size = 40;
  int gap = 8;
  int totalW = (int)toRender.size() * (size + gap);
  int startX = (w - totalW) / 2;
  int y = 20;

  for (int i = 0; i < (int)toRender.size(); i++) {
    int x = startX + i * (size + gap);
    Entity e = toRender[i].e;
    bool isCurrent = toRender[i].isCurrent;

    SDL_Color borderCol =
        isCurrent ? SDL_Color{255, 255, 0, 255} : SDL_Color{50, 50, 50, 255};

    auto *u = m_Registry->GetComponent<BattleUnitComponent>(e);
    SDL_Color bgCol = {100, 100, 100, 255};
    if (u) {
      if (u->team == BattleUnitComponent::Player)
        bgCol = {50, 50, 150, 255};
      else
        bgCol = {150, 50, 50, 255};
    }

    // Draw larger highlight for current turn
    if (isCurrent) {
      ren->DrawRect2D(x - 4, y - 4, size + 8, size + 8, borderCol);
      // Arrow removed
    }

    ren->DrawRect2D(x, y, size, size, bgCol);

    auto *mesh = m_Registry->GetComponent<MeshComponent>(e);
    if (mesh) {
      ren->RenderThumbnail(mesh->meshName, mesh->textureName, x + 2, y + 2,
                           size - 4);
    }

    if (u) {
      float pct = (float)u->hp / (float)u->maxHp;
      pct = std::max(0.0f, std::min(1.0f, pct));
      int barH = (int)(size * pct);
      ren->DrawRect2D(x, y + size - barH, 4, barH, {255, 0, 0, 200});
    }
  }
}

void BattleMode::UpdateAI(float dt) {
  static float aiTimer = 0.0f;
  aiTimer += dt;
  if (aiTimer < 1.0f)
    return;
  aiTimer = 0.0f;

  Entity current = m_TurnOrder[m_CurrentTurnIndex];
  auto *actor = m_Registry->GetComponent<BattleUnitComponent>(current);
  if (!actor) {
    NextTurn();
    return;
  }

  Entity target = -1;
  float minDist = 999.0f;
  auto *t1 = m_Registry->GetComponent<Transform3DComponent>(current);

  auto view = m_Registry->View<BattleUnitComponent>();
  for (auto &pair : view) {
    if (pair.second.team == BattleUnitComponent::Player && pair.second.hp > 0) {
      auto *t2 = m_Registry->GetComponent<Transform3DComponent>(pair.first);
      float d = sqrt(pow(t1->x - t2->x, 2) + pow(t1->y - t2->y, 2));
      if (d < minDist) {
        minDist = d;
        target = pair.first;
      }
    }
  }

  if (target != -1 && actor->hasAction) {
    auto *t2 = m_Registry->GetComponent<Transform3DComponent>(target);
    if (minDist < 2.0f) {
      ExecuteAction(current, Melee, target, 0, 0);
      return;
    } else {
      float moveDist = std::min(actor->currentMovement, minDist - 1.5f);
      if (moveDist > 0) {
        float angle = atan2(t2->y - t1->y, t2->x - t1->x);

        float destX = t1->x + cos(angle) * moveDist;
        float destY = t1->y + sin(angle) * moveDist;

        // Simple collision avoidance for AI: if destination is blocked, try
        // reducing distance
        while (moveDist > 0.5f && (IsPositionOccupied(destX, destY, current) || CheckCollision(destX, destY, t1->z, current))) {
          moveDist -= 0.5f;
          destX = t1->x + cos(angle) * moveDist;
          destY = t1->y + sin(angle) * moveDist;
        }

        if (moveDist > 0.5f) {
          m_AnimState.actor = current;
          m_AnimState.startX = t1->x;
          m_AnimState.startY = t1->y;
          m_AnimState.targetX = destX;
          m_AnimState.targetY = destY;
          m_AnimState.duration = moveDist / 5.0f;
          m_AnimState.timer = 0.0f;
          m_State = Moving;
          t1->rot =
              atan2(t1->x - destX, t1->y - destY); // Face movement direction
          actor->currentMovement -= moveDist;

          // Set Run Animation for enemy
          auto *animComp =
              m_Registry->GetComponent<SkeletalAnimationComponent>(current);
          auto *mesh = m_Registry->GetComponent<MeshComponent>(current);
          if (animComp && mesh) {
            RenderMesh *rm = m_Renderer->GetRenderMesh(mesh->meshName);
            if (rm) {
              for (size_t i = 0; i < rm->animations.size(); i++) {
                if (rm->animations[i].name.find("Run") != std::string::npos ||
                    rm->animations[i].name.find("Walk") != std::string::npos) {
                  animComp->animationIndex = (int)i;
                  animComp->currentTime = 0.0f;
                  break;
                }
              }
            }
          }
          return;
        }
      }
    }
  }

  NextTurn();
}

void BattleMode::RaycastCursor() {
  int mx, my;
  Input::GetMousePosition(mx, my);
  if (!m_Camera)
    return;

  float aspect = 1280.0f / 720.0f;

  float ndcX = (2.0f * mx) / 1280.0f - 1.0f;
  float ndcY = 1.0f - (2.0f * my) / 720.0f;

  float tanFov = tan(1.04f / 2.0f);

  float vx = ndcX * aspect * tanFov;
  float vy = ndcY * tanFov;
  float vz = -1.0f;

  float cy = cos(m_Camera->yaw);
  float sy = sin(m_Camera->yaw);
  float cp = cos(m_Camera->pitch);
  float sp = sin(m_Camera->pitch);

  float dx = cy * cp;
  float dy = sy * cp;
  float dz = sp;

  Vec3 F_gl = {dx, dz, -dy};
  Vec3 R_gl = Vec3::Normalize(Vec3::Cross(F_gl, {0, 1, 0}));
  Vec3 U_gl = Vec3::Normalize(Vec3::Cross(R_gl, F_gl));

  // View direction is (vx, vy, -1).
  // Camera Basis: Right, Up, Back (-F).
  // World Ray = Right * vx + Up * vy + Back * (-1) = R * vx + U * vy + F
  Vec3 RayDir_gl = Vec3::Normalize(R_gl * vx + U_gl * vy + F_gl);
  Vec3 RayOrigin_gl = {m_Camera->x, m_Camera->z, -m_Camera->y};

  if (abs(RayDir_gl.y) > 0.001f) {
    float t = -RayOrigin_gl.y / RayDir_gl.y;

    if (t > 0) {
      Vec3 Hit_gl = RayOrigin_gl + RayDir_gl * t;
      m_Cursor.x = Hit_gl.x;
      m_Cursor.y = -Hit_gl.z;
      m_Cursor.z = 0;
      m_Cursor.active = true;

      // Check entities
      m_Cursor.hoveredEntity = -1;
      auto view = m_Registry->View<ColliderComponent>();
      for (auto &pair : view) {
        auto *trans =
            m_Registry->GetComponent<Transform3DComponent>(pair.first);
        auto *unit = m_Registry->GetComponent<BattleUnitComponent>(pair.first);
        if (trans && (!unit || unit->hp > 0)) {  // Only target alive entities
          float dist = sqrt(pow(m_Cursor.x - trans->x, 2) +
                            pow(m_Cursor.y - trans->y, 2));
          if (dist < 1.0f) {
            m_Cursor.hoveredEntity = pair.first;
            break;
          }
        }
      }
    }
  }
}

PixelsEngine::AABB BattleMode::GetWorldAABB(const PixelsEngine::AABB& local, const PixelsEngine::Transform3DComponent& t, float scaleX, float scaleY, float scaleZ) {
    float minX = 1e9, minY = 1e9, minZ = 1e9;
    float maxX = -1e9, maxY = -1e9, maxZ = -1e9;
    
    float corners[8][3] = {
        {local.minX, local.minY, local.minZ},
        {local.maxX, local.minY, local.minZ},
        {local.minX, local.maxY, local.minZ},
        {local.maxX, local.maxY, local.minZ},
        {local.minX, local.minY, local.maxZ},
        {local.maxX, local.minY, local.maxZ},
        {local.minX, local.maxY, local.maxZ},
        {local.maxX, local.maxY, local.maxZ}
    };

    float cosR = cos(-t.rot);
    float sinR = sin(-t.rot);

    for(int i=0; i<8; ++i) {
        float smx = corners[i][0] * scaleX;
        float smy = corners[i][1] * scaleZ; 
        float smz = corners[i][2] * scaleY; 
        
        float gl_x = smx * cosR + smz * sinR;
        float gl_y = smy;
        float gl_z = -smx * sinR + smz * cosR;
        
        float g_x = gl_x + t.x;
        float g_y = -gl_z + t.y; 
        float g_z = gl_y + t.z;
        
        if(g_x < minX) minX = g_x;
        if(g_x > maxX) maxX = g_x;
        if(g_y < minY) minY = g_y;
        if(g_y > maxY) maxY = g_y;
        if(g_z < minZ) minZ = g_z;
        if(g_z > maxZ) maxZ = g_z;
    }
    return PixelsEngine::AABB(minX, minY, minZ, maxX, maxY, maxZ);
}

bool BattleMode::CheckCollision(float x, float y, float z, Entity ignoreEntity) {
    float radius = 0.4f;
    float height = 1.8f;
    PixelsEngine::AABB entityAABB(x - radius, y - radius, z, x + radius, y + radius, z + height);

    auto &meshes = m_Registry->View<MeshComponent>();
    for (auto &mPair : meshes) {
        if (mPair.first == ignoreEntity) continue;
        
        auto *hc = m_Registry->GetComponent<HitboxComponent>(mPair.first);
        if (!hc || !hc->isSolid) continue;

        // Skip doors if they are fully open
        auto *dc = m_Registry->GetComponent<DoorComponent>(mPair.first);
        if (dc && dc->isOpen && dc->currentOpenDistance >= dc->maxOpenDistance * 0.8f) continue;

        auto *wt = m_Registry->GetComponent<Transform3DComponent>(mPair.first);
        if (!wt) continue;

        RenderMesh* rm = m_Renderer->GetRenderMesh(mPair.second.meshName);
        if (rm) {
            PixelsEngine::AABB wallAABB = GetWorldAABB(rm->boundingBox, *wt, mPair.second.scaleX, mPair.second.scaleY, mPair.second.scaleZ);
            if (entityAABB.Overlaps(wallAABB)) return true;
        }
    }
    return false;
}

void BattleMode::UpdateDoors(float dt) {
    auto &units = m_Registry->View<BattleUnitComponent>();
    auto &doors = m_Registry->View<DoorComponent>();

    for (auto &dPair : doors) {
        auto *door = &dPair.second;
        auto *dtans = m_Registry->GetComponent<Transform3DComponent>(dPair.first);
        auto *mc = m_Registry->GetComponent<MeshComponent>(dPair.first);
        if (!dtans || !mc) continue;

        bool unitNear = false;
        bool moveThrough = false;
        
        // 1. Check current unit positions
        for (auto &uPair : units) {
            auto *ut = m_Registry->GetComponent<Transform3DComponent>(uPair.first);
            if (!ut) continue;
            float dist = sqrt(pow(ut->x - dtans->x, 2) + pow(ut->y - dtans->y, 2));
            if (dist < 2.5f) {
                unitNear = true;
                break;
            }
        }

        // 2. Predict based on active movement path
        if (m_State == Moving && m_AnimState.actor != INVALID_ENTITY) {
            // Check if movement segment (startX, startY) -> (targetX, targetY) passes near door
            float x1 = m_AnimState.startX, y1 = m_AnimState.startY;
            float x2 = m_AnimState.targetX, y2 = m_AnimState.targetY;
            float dx = x2 - x1, dy = y2 - y1;
            float lenSq = dx*dx + dy*dy;
            
            if (lenSq > 0.001f) {
                // Parameter t of projection of door onto segment
                float t = ((dtans->x - x1) * dx + (dtans->y - y1) * dy) / lenSq;
                t = std::max(0.0f, std::min(1.0f, t));
                float projX = x1 + t * dx;
                float projY = y1 + t * dy;
                float distToPath = sqrt(pow(dtans->x - projX, 2) + pow(dtans->y - projY, 2));
                
                if (distToPath < 1.5f) {
                    moveThrough = true;
                }
            }
        }

        // Auto open/close logic
        if ((unitNear || moveThrough) && !door->isOpen) {
            door->isOpen = true;
        } else if (!unitNear && !moveThrough && door->isOpen) {
            door->isOpen = false;
        }

        // Animate - Faster when moving through
        float speedMult = moveThrough ? 4.0f : 1.5f;
        float targetDist = door->isOpen ? door->maxOpenDistance : 0.0f;
        if (door->currentOpenDistance < targetDist) {
            door->currentOpenDistance = std::min(targetDist, door->currentOpenDistance + door->openSpeed * speedMult * dt);
        } else if (door->currentOpenDistance > targetDist) {
            door->currentOpenDistance = std::max(targetDist, door->currentOpenDistance - door->openSpeed * dt);
        }

        // Apply visual rotation/translation
        RenderMesh* rm = m_Renderer->GetRenderMesh(mc->meshName);
        float doorWidth = rm ? (rm->boundingBox.maxX - rm->boundingBox.minX) : 1.0f;
        float doorRadius = doorWidth * 0.5f;
        float angle = (doorRadius > 0.0f) ? door->currentOpenDistance / doorRadius : 0.0f;
        dtans->rot = angle;
        float hingeWorldX = door->originalX - doorWidth * 0.5f;
        dtans->x = hingeWorldX + (doorWidth * 0.5f) * cos(angle);
        dtans->y = door->originalY + (doorWidth * 0.5f) * sin(angle);
    }
}