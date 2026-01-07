#include "SiegeMode.h"
#include "../engine/Components.h"
#include "../engine/GLTFLoader.h"
#include "../engine/Input.h"
#include "../engine/TextRenderer.h"
#include <SDL2/SDL_mixer.h> // Required for Mix_Chunk types
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace PixelsEngine;

SiegeMode::SiegeMode(Registry *registry, GLRenderer *renderer)
    : m_Registry(registry), m_Renderer(renderer) {
  // AudioManager will be initialized in Init()
}

SiegeMode::~SiegeMode() {
  // AudioManager handles cleanup automatically
}

void SiegeMode::Init(Camera *camera, Entity &playerEntity) {
  m_Camera = camera;

  // 1. Initialize Audio Manager
  m_AudioManager = std::make_unique<AudioManager>();
  m_SfxShoot = m_AudioManager->LoadWAV("assets/bow_shoot.wav");
  m_SfxHit = m_AudioManager->LoadWAV("assets/bow_hit.wav");
  m_SfxSwordHit = m_AudioManager->LoadWAV("assets/sword_hit.wav");
  m_SfxSwordMiss = m_AudioManager->LoadWAV("assets/sword_miss.wav");

  // 2. Initialize Asset Manager
  auto assetManager = std::make_unique<AssetManager>(m_Renderer);
  assetManager->LoadCharacter("Knight",
                               "assets/adventurers/Characters/gltf/Knight.glb",
                               "assets/adventurers/Textures/knight_texture.png");
  assetManager->LoadCharacter("skel_minion",
                               "assets/skeletons/characters/gltf/Skeleton_Minion.glb",
                               "assets/skeletons/texture/skeleton_texture.png");
  assetManager->LoadCharacter("skel_warrior",
                               "assets/skeletons/characters/gltf/Skeleton_Warrior.glb",
                               "assets/skeletons/texture/skeleton_texture.png");

  // Load weapon meshes
  m_Renderer->LoadMesh("Sword",
                       "assets/adventurers/Assets/gltf/sword_1handed.gltf");
  m_Renderer->LoadMesh("Crossbow",
                       "assets/adventurers/Assets/gltf/crossbow_1handed.gltf");
  m_Renderer->LoadMesh("Arrow",
                       "assets/adventurers/Assets/gltf/arrow_bow.gltf");
  m_Renderer->LoadMesh("Dagger",
                       "assets/adventurers/Assets/gltf/dagger.gltf");

  // 3. Load Map
  MapLoader::LoadFromFile("assets/dungeons/my_dungeon.map", m_Registry,
                           m_Renderer);

  // 4. Spawn Player
  playerEntity =
      CharacterFactory::CreatePlayer(m_Registry, m_Renderer, 12.0f, 0.0f, 0.0f);

  // 5. Create camera controller AFTER spawning the player
  m_CameraController = std::make_unique<ThirdPersonCamera>(
      m_Camera, m_Registry, playerEntity);
  m_CameraController->SetDistance(3.0f);      // Distance behind player
  m_CameraController->SetShoulder(0.0f);     // Center behind, not on shoulder
  m_CameraController->SetHeight(1.6f);       // Eye level height
  m_CameraController->SetYaw(-M_PI / 2);  // Rotate 180 degrees to look from behind
  // Create movement controller for player input handling
  m_MovementController = std::make_unique<ThirdPersonMovementController>(
      m_Registry, playerEntity);

  // 5. Spawn Initial Enemies
  SpawnEnemy("skel_minion", 12.0f, 10.0f, CharacterComponent::SkeletonMinion);
  SpawnEnemy("skel_warrior", 15.0f, 12.0f, CharacterComponent::SkeletonWarrior);
  SpawnEnemy("skel_minion", 8.0f, 15.0f, CharacterComponent::SkeletonMinion);
}

void SiegeMode::SpawnEnemy(const std::string &mesh, float x, float y,
                           CharacterComponent::Type type) {
  Entity e = CharacterFactory::CreateSkeleton(m_Registry, m_Renderer, mesh, x, y, 0.0f);
  
  // Add weapon attachment based on skeleton type
  auto *unit = m_Registry->GetComponent<BattleUnitComponent>(e);
  if (unit) {
    AttachmentComponent attach;
    attach.textureName = mesh + "_tex";
    attach.scale = 1.0f;
    attach.boneName = "hand.r";
    attach.rotX = -7.63f;
    attach.rotY = 18.148f;
    attach.rotZ = 1.702f;
    
    if (mesh == "skel_warrior") {
      attach.meshName = "Sword";
    } else if (mesh == "skel_minion") {
      attach.meshName = "Dagger";
    }
    
    m_Registry->AddComponent<AttachmentComponent>(e, attach);
  }
}

void SiegeMode::Update(float dt, Entity playerEntity, float tunerDist,
                       float tunerShoulder, float tunerHeight) {
  auto *t = m_Registry->GetComponent<Transform3DComponent>(playerEntity);
  auto *phys = m_Registry->GetComponent<PhysicsComponent>(playerEntity);
  auto *unit = m_Registry->GetComponent<BattleUnitComponent>(playerEntity);
  auto *pctrl = m_Registry->GetComponent<PlayerControlComponent>(playerEntity);

  if (!t || !m_Camera || !phys || !unit)
    return;

  // 1. Mouse Camera Control (Free Look)
  int mx, my;
  Input::GetMouseDelta(mx, my);
  float sensitivity = 0.003f;

  // Handle camera input
  m_CameraController->HandleInput(dt);

  // Sync player rotation with camera yaw for consistent facing direction
  if (m_CameraController) {
    float player_rot_offset = -M_PI / 2; // Adjust for model facing direction
    t->rot = -m_CameraController->GetCameraYaw() + player_rot_offset;
  }

  // Update camera position (third-person follows player)
  m_CameraController->Update(dt);

  // Handle player movement with input controller
  if (m_MovementController) {
    m_MovementController->HandleInput(dt);
  }

  // Determine if player is moving for animation
  // Use a small threshold to avoid jitter and ensure idle plays when stopped
  bool moving = phys && (phys->velX * phys->velX + phys->velY * phys->velY > 0.01f);

  if (false) {  // Keep this block for future reference, but skip execution
    phys->velZ = pctrl ? pctrl->jumpForce : 6.0f;
    phys->isGrounded = false;
  }

  // 3. Animation Logic
  auto *anim =
      m_Registry->GetComponent<SkeletalAnimationComponent>(playerEntity);
  RenderMesh *rm = m_Renderer->GetRenderMesh("Knight");

  if (anim && rm) {
    if (m_AttackTimer > 0) {
      m_AttackTimer -= dt;
    } else {
      std::string targetAnim = moving ? "Walking_A" : "Idle_A";

      // Check if we need to switch
      bool needsSwitch = true;
      if (anim->animationIndex < rm->animations.size()) {
        if (rm->animations[anim->animationIndex].name.find(targetAnim) !=
            std::string::npos) {
          needsSwitch = false;
        }
      }

      if (needsSwitch) {
        int bestIdx = -1;
        for (size_t i = 0; i < rm->animations.size(); i++) {
          if (rm->animations[i].name.find(targetAnim) != std::string::npos) {
            bestIdx = (int)i;
            break;
          }
        }
        // Fallback search
        if (bestIdx == -1) {
          for (size_t i = 0; i < rm->animations.size(); i++) {
            if (rm->animations[i].name.find(moving ? "Walk" : "Idle") !=
                std::string::npos) {
              bestIdx = (int)i;
              break;
            }
          }
        }

        if (bestIdx != -1) {
          anim->animationIndex = bestIdx;
          anim->currentTime = 0.0f;
        }
      }
    }
  }

  // 4. Weapon Switching
  auto *attach = m_Registry->GetComponent<AttachmentComponent>(playerEntity);
  if (attach) {
    if (Input::IsKeyPressed(SDL_SCANCODE_1))
      unit->preferredAction = 2; // Melee
    if (Input::IsKeyPressed(SDL_SCANCODE_2))
      unit->preferredAction = 3; // Ranged

    if (unit->preferredAction == 3) {
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

  // 5. Combat Logic
  if (m_HitmarkerTimer > 0)
    m_HitmarkerTimer -= dt;

  // Use IsMouseButtonDown for more responsive combat feel
  if (Input::IsMouseButtonDown(SDL_BUTTON_LEFT)) {
    if (m_AttackTimer <= 0.0f) {

      std::string animName = (unit->preferredAction == 3)
                                 ? "Ranged_Bow_Shoot"
                                 : "Melee_1H_Attack_Chop";
      
      bool animFound = false;
      if (rm && anim) {
        for (size_t i = 0; i < rm->animations.size(); i++) {
          if (rm->animations[i].name.find(animName) != std::string::npos) {
            anim->animationIndex = (int)i;
            anim->currentTime = 0.0f;
            m_AttackTimer = rm->animations[i].duration;
            animFound = true;
            break;
          }
        }
      }
      
      // Fallback timer if animation not found
      if (!animFound) {
        m_AttackTimer = 0.5f;
      }

      if (unit->preferredAction == 3) { // Ranged
        if (m_SfxShoot)
          Mix_PlayChannel(-1, m_SfxShoot, 0);

        auto arrow = m_Registry->CreateEntity();
        // Position arrow at chest height and forward
        float forwardX = -sin(t->rot);
        float forwardY = -cos(t->rot);
        
        m_Registry->AddComponent<Transform3DComponent>(
            arrow, {t->x + forwardX * 0.5f, t->y + forwardY * 0.5f, t->z + 1.2f, t->rot, m_CameraController->GetCameraPitch()});
        m_Registry->AddComponent<MeshComponent>(
            arrow, {"Arrow", "Knight_tex", 1, 1, 1});
        m_Registry->AddComponent<ProjectileComponent>(
            arrow, {ProjectileComponent::Arrow, 25.0f, true, 5.0f});

        float projSpeed = 40.0f;
        m_Registry->AddComponent<PhysicsComponent>(
            arrow, {forwardX * projSpeed, forwardY * projSpeed, 0.0f,
                    0.0f, false, false, 0.0f, 0.0f});
      } else { // Melee
        bool hitSomething = false;
        auto &enemies = m_Registry->View<EnemyComponent>();

        for (auto &pair : enemies) {
          auto *et = m_Registry->GetComponent<Transform3DComponent>(pair.first);
          auto *eu = m_Registry->GetComponent<BattleUnitComponent>(pair.first);

          if (et && eu && eu->hp > 0) {
            float edx = et->x - t->x;
            float edy = et->y - t->y;
            float dist = sqrt(edx * edx + edy * edy);

            float angleToEnemy = atan2(edy, edx);
            // Forward check based on model rotation
            float angleDiff =
                fmod(angleToEnemy - (t->rot - 1.57f) + M_PI * 3, M_PI * 2) -
                M_PI;

            if (dist < 2.5f && abs(angleDiff) < 1.2f) {
              eu->hp -= 15;
              eu->flashAmount = 1.0f;
              m_HitmarkerTimer = 0.15f;
              hitSomething = true;
              // Don't destroy immediately - let enemy AI section fade them out
              break;
            }
          }
        }

        if (hitSomething) {
          if (m_SfxSwordHit)
            Mix_PlayChannel(-1, m_SfxSwordHit, 0);
        } else {
          if (m_SfxSwordMiss)
            Mix_PlayChannel(-1, m_SfxSwordMiss, 0);
        }
      }
    }
  }

  // 6. Enemy AI Update (Chase player) and cleanup dead enemies
  auto &enemies = m_Registry->View<EnemyComponent>();
  std::vector<Entity> enemiesToRemove;
  
  for (auto &pair : enemies) {
    auto *et = m_Registry->GetComponent<Transform3DComponent>(pair.first);
    auto *eu = m_Registry->GetComponent<BattleUnitComponent>(pair.first);
    auto *ephys = m_Registry->GetComponent<PhysicsComponent>(pair.first);
    auto *anim = m_Registry->GetComponent<SkeletalAnimationComponent>(pair.first);
    auto *mesh = m_Registry->GetComponent<MeshComponent>(pair.first);
    auto &enemy = pair.second;
    
    // Check if enemy is dead
    if (eu && eu->hp <= 0) {
      // Play death/idle animation
      if (anim && mesh) {
        RenderMesh *rm = m_Renderer->GetRenderMesh(mesh->meshName);
        if (rm) {
          int idleIdx = -1;
          for (size_t i = 0; i < rm->animations.size(); i++) {
            if (rm->animations[i].name.find("Idle") != std::string::npos) {
              idleIdx = (int)i;
              break;
            }
          }
          if (idleIdx != -1 && anim->animationIndex != idleIdx) {
            anim->animationIndex = idleIdx;
            anim->currentTime = 0.0f;
          }
        }
      }
      
      // Decay the flash amount over time
      if (eu->flashAmount > 0.0f) {
        eu->flashAmount -= dt * 3.0f;
        if (eu->flashAmount < 0.0f) {
          eu->flashAmount = 0.0f;
        }
      }
      
      // After flash fades, remove the entity
      if (eu->flashAmount <= 0.0f) {
        enemiesToRemove.push_back(pair.first);
      }
      continue;  // Don't update dead enemies
    }
    
    // Decay flash for living enemies too
    if (eu && eu->flashAmount > 0.0f) {
      eu->flashAmount -= dt * 5.0f;
      if (eu->flashAmount < 0.0f) {
        eu->flashAmount = 0.0f;
      }
    }
    
    if (et && ephys) {
      float edx = t->x - et->x;
      float edy = t->y - et->y;
      float dist = sqrt(edx * edx + edy * edy);
      
      bool isMoving = false;
      bool isAttacking = false;
      
      // Attack if close enough
      if (dist < 2.0f) {
        isAttacking = true;
        // TODO: Actually damage player
      } else if (dist > 1.5f) {
        isMoving = true;
        
        // Check collision with other enemies and avoid
        float collisionAvoidX = 0.0f, collisionAvoidY = 0.0f;
        int nearbyEnemies = 0;
        auto &allEnemies = m_Registry->View<EnemyComponent>();
        for (auto &other : allEnemies) {
          if (other.first == pair.first) continue;
          auto *oet = m_Registry->GetComponent<Transform3DComponent>(other.first);
          if (oet) {
            float dx = et->x - oet->x;
            float dy = et->y - oet->y;
            float d = sqrt(dx * dx + dy * dy);
            if (d < 0.8f && d > 0.01f) {  // Stronger collision avoidance
              float pushStr = 1.5f;
              float pushX = (dx / d) * pushStr;
              float pushY = (dy / d) * pushStr;
              collisionAvoidX += pushX;
              collisionAvoidY += pushY;
              nearbyEnemies++;
            }
          }
        }
        
        // Apply collision avoidance
        if (nearbyEnemies > 0) {
          et->x += collisionAvoidX * dt;
          et->y += collisionAvoidY * dt;
        }
        
        // Move towards player
        float angle = atan2(edy, edx);
        et->rot = angle + 1.57f;
        et->x += cos(angle) * enemy.speed * dt;
        et->y += sin(angle) * enemy.speed * dt;
      }
      
      // Update animation based on state
      if (anim && mesh) {
        RenderMesh *rm = m_Renderer->GetRenderMesh(mesh->meshName);
        if (rm) {
          std::string targetAnimName = isAttacking ? "Attack" : (isMoving ? "Walk" : "Idle");
          
          int targetIdx = -1;
          for (size_t i = 0; i < rm->animations.size(); i++) {
            if (rm->animations[i].name.find(targetAnimName) != std::string::npos) {
              targetIdx = (int)i;
              break;
            }
          }
          
          if (targetIdx != -1 && anim->animationIndex != targetIdx) {
            anim->animationIndex = targetIdx;
            anim->currentTime = 0.0f;
          }
        }
      }
    }
  }
  
  // Remove dead enemies that have faded
  for (auto e : enemiesToRemove) {
    m_Registry->DestroyEntity(e);
  }
}

void SiegeMode::RaycastCursor() {
  int mx, my;
  Input::GetMousePosition(mx, my);
  if (!m_Camera)
    return;

  // Assuming 720p resolution as per original code context
  float aspect = 1280.0f / 720.0f;
  float ndcX = (2.0f * mx) / 1280.0f - 1.0f;
  float ndcY = 1.0f - (2.0f * my) / 720.0f;

  // FOV should ideally match the renderer settings (approx 60 deg here)
  float tanFov = tan(1.04f / 2.0f);
  float vx = ndcX * aspect * tanFov;
  float vy = ndcY * tanFov;

  float cy = cos(m_Camera->yaw);
  float sy = sin(m_Camera->yaw);
  float cp = cos(m_Camera->pitch);
  float sp = sin(m_Camera->pitch);

  // Camera Basis vectors
  float dx = cy * cp;
  float dy = sy * cp;
  float dz = sp;

  Vec3 F_gl = {dx, dz, -dy};
  Vec3 R_gl = Vec3::Normalize(Vec3::Cross(F_gl, {0, 1, 0}));
  Vec3 U_gl = Vec3::Normalize(Vec3::Cross(R_gl, F_gl));

  Vec3 RayDir_gl = Vec3::Normalize(R_gl * vx + U_gl * vy + F_gl);
  // Note: Verify coordinate system (Y-up vs Z-up) matches your engine
  // conventions
  Vec3 RayOrigin_gl = {m_Camera->x, m_Camera->z, -m_Camera->y};

  if (abs(RayDir_gl.y) > 0.001f) {
    float t = -RayOrigin_gl.y / RayDir_gl.y;
    if (t > 0) {
      Vec3 Hit_gl = RayOrigin_gl + RayDir_gl * t;
      m_Cursor.x = Hit_gl.x;
      m_Cursor.y = -Hit_gl.z;
      m_Cursor.z = 0;
      m_Cursor.active = true;
    }
  }
}

void SiegeMode::RenderUI(PixelsEngine::GLRenderer *ren, PixelsEngine::TextRenderer *tr,
                int w, int h, PixelsEngine::Entity playerEntity) {
  // Crosshair
  ren->DrawRect2D(w / 2 - 1, h / 2 - 10, 2, 20, {255, 255, 255, 150});
  ren->DrawRect2D(w / 2 - 10, h / 2 - 1, 20, 2, {255, 255, 255, 150});

  // Hitmarker
  if (m_HitmarkerTimer > 0) {
    ren->DrawRect2D(w / 2 - 15, h / 2 - 15, 5, 5, {255, 0, 0, 255});
    ren->DrawRect2D(w / 2 + 10, h / 2 - 15, 5, 5, {255, 0, 0, 255});
    ren->DrawRect2D(w / 2 - 15, h / 2 + 10, 5, 5, {255, 0, 0, 255});
    ren->DrawRect2D(w / 2 + 10, h / 2 + 10, 5, 5, {255, 0, 0, 255});
  }

  // HP Bar
  auto view = m_Registry->View<BattleUnitComponent>();
  for (auto &pair : view) {
    if (pair.second.team == BattleUnitComponent::Player) {
      int hp = pair.second.hp;
      int maxHp = pair.second.maxHp;

      // Prevent div by zero
      if (maxHp <= 0)
        maxHp = 1;

      ren->DrawRect2D(20, h - 40, 200, 20, {50, 50, 50, 200});
      ren->DrawRect2D(22, h - 38, (int)(196 * ((float)hp / maxHp)), 16,
                      {255, 50, 50, 255});
      tr->RenderText(ren, "HP: " + std::to_string(hp), 20, h - 70,
                     {255, 255, 255, 255});
      break;
    }
  }

  // Show nearby doors and interactions
  auto *playerTransform = m_Registry->GetComponent<Transform3DComponent>(playerEntity);
  if (playerTransform) {
    auto &doors = m_Registry->View<DoorComponent>();
    
    int doorCount = 0;
    for (auto &pair : doors) {
      auto *door = &pair.second;
      auto *doorTransform = m_Registry->GetComponent<Transform3DComponent>(pair.first);
      if (!doorTransform) continue;
      
      float dx = playerTransform->x - doorTransform->x;
      float dy = playerTransform->y - doorTransform->y;
      float dist = sqrt(dx * dx + dy * dy);
      
      if (dist < 3.5f) {
        std::string doorStatus = door->isOpen ? "OPEN" : "CLOSED";
        tr->RenderText(ren, "Door: " + doorStatus + " (Distance: " + std::to_string((int)dist) + "m)",
                       20, 60 + (doorCount * 25),
                       door->isOpen ? SDL_Color{100, 255, 100, 255} : SDL_Color{255, 100, 100, 255});
        doorCount++;
      }
    }
  }
  
  // Draw health bars above enemies in 3D space
  if (playerTransform) {
    auto &enemies = m_Registry->View<EnemyComponent>();
    for (auto &pair : enemies) {
      auto *et = m_Registry->GetComponent<Transform3DComponent>(pair.first);
      auto *eu = m_Registry->GetComponent<BattleUnitComponent>(pair.first);
      
      if (et && eu && eu->maxHp > 0) {
        // Only show health bar if damaged
        if (eu->hp < eu->maxHp) {
          // Convert 3D position to screen space (simplified)
          float barWidth = 50.0f;
          float barHeight = 6.0f;
          float hpPercent = (float)eu->hp / (float)eu->maxHp;
          
          // Position health bar above enemy (approximate screen position)
          // This is a rough approximation - ideally would project 3D to 2D
          int screenX = w / 2 + (int)((et->x - playerTransform->x) * 30);
          int screenY = h / 2 - (int)((et->y - playerTransform->y) * 30) - 80;
          
          // Clamp to screen bounds
          screenX = std::max(10, std::min(w - 60, screenX));
          screenY = std::max(50, std::min(h - 50, screenY));
          
          // Background
          ren->DrawRect2D(screenX, screenY, (int)barWidth, (int)barHeight, {50, 50, 50, 200});
          // HP bar
          ren->DrawRect2D(screenX + 1, screenY + 1, (int)(barWidth - 2) * hpPercent, (int)barHeight - 2, {255, 50, 50, 255});
        }
      }
    }
  }

  tr->RenderText(ren, "SIEGE MODE: 1 - SWORD, 2 - CROSSBOW | WASD - MOVE | SPACE - JUMP", 20, 20,
                 {255, 255, 255, 255});
}

float SiegeMode::GetCameraYaw() const {
  if (m_CameraController) {
    return m_CameraController->GetYaw();
  }
  return 0.0f;
}