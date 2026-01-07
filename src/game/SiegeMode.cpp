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
  // Initialize pointers to nullptr for safety
  m_SfxShoot = nullptr;
  m_SfxHit = nullptr;
  m_SfxSwordHit = nullptr;
  m_SfxSwordMiss = nullptr;
}

SiegeMode::~SiegeMode() {
  // FIX: Free audio resources to prevent memory leaks
  if (m_SfxShoot)
    Mix_FreeChunk(m_SfxShoot);
  if (m_SfxHit)
    Mix_FreeChunk(m_SfxHit);
  if (m_SfxSwordHit)
    Mix_FreeChunk(m_SfxSwordHit);
  if (m_SfxSwordMiss)
    Mix_FreeChunk(m_SfxSwordMiss);
}

void SiegeMode::Init(Camera *camera, Entity &playerEntity) {
  m_Camera = camera;

  // 1. Load Audio
  m_SfxShoot = Mix_LoadWAV("assets/bow_shoot.wav");
  m_SfxHit = Mix_LoadWAV("assets/bow_hit.wav");
  m_SfxSwordHit = Mix_LoadWAV("assets/sword_hit.wav");
  m_SfxSwordMiss = Mix_LoadWAV("assets/sword_miss.wav");

  // Check for audio load failures
  if (!m_SfxShoot)
    std::cerr << "Failed to load bow_shoot.wav: " << Mix_GetError()
              << std::endl;

  // 2. Load Assets
  m_Renderer->LoadTexture("knight_tex",
                          "assets/adventurers/Textures/knight_texture.png");
  m_Renderer->LoadTexture("skeleton_tex",
                          "assets/skeletons/texture/skeleton_texture.png");

  m_Renderer->LoadMesh("Knight",
                       "assets/adventurers/Characters/gltf/Knight.glb");
  m_Renderer->LoadMesh("Sword",
                       "assets/adventurers/Assets/gltf/sword_1handed.gltf");
  m_Renderer->LoadMesh("Crossbow",
                       "assets/adventurers/Assets/gltf/crossbow_1handed.gltf");
  m_Renderer->LoadMesh("Arrow",
                       "assets/adventurers/Assets/gltf/arrow_bow.gltf");

  m_Renderer->LoadMesh("skel_minion",
                       "assets/skeletons/characters/gltf/Skeleton_Minion.glb");
  m_Renderer->LoadMesh("skel_warrior",
                       "assets/skeletons/characters/gltf/Skeleton_Warrior.glb");

  // Load and assign skeletal animations
  auto playerAnims = GLTFLoader::LoadAnimations(
      "assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_General.glb");
  auto combatMelee =
      GLTFLoader::LoadAnimations("assets/animations/Animations/gltf/Rig_Medium/"
                                 "Rig_Medium_CombatMelee.glb");
  auto combatRanged =
      GLTFLoader::LoadAnimations("assets/animations/Animations/gltf/Rig_Medium/"
                                 "Rig_Medium_CombatRanged.glb");
  auto movementBasic =
      GLTFLoader::LoadAnimations("assets/animations/Animations/gltf/Rig_Medium/"
                                 "Rig_Medium_MovementBasic.glb");

  auto SetupMesh = [&](const std::string &name,
                       const std::vector<SkeletalAnimation> &baseAnims) {
    RenderMesh *rm = m_Renderer->GetRenderMesh(name);
    if (rm) {
      rm->animations = baseAnims;
      rm->animations.insert(rm->animations.end(), combatMelee.begin(),
                            combatMelee.end());
      rm->animations.insert(rm->animations.end(), combatRanged.begin(),
                            combatRanged.end());
      rm->animations.insert(rm->animations.end(), movementBasic.begin(),
                            movementBasic.end());
      rm->isSkinned = true;
    }
  };
  SetupMesh("Knight", playerAnims);
  SetupMesh("skel_minion", playerAnims);
  SetupMesh("skel_warrior", playerAnims);

  // 3. Load Map
  LoadMap("assets/dungeons/my_dungeon.map");

  // 4. Spawn Player
  SpawnPlayer(playerEntity);

  // 5. Create camera controller AFTER spawning the player
  m_CameraController = std::make_unique<ThirdPersonCamera>(
      m_Camera, m_Registry, playerEntity);
  m_CameraController->SetDistance(3.0f);      // Distance behind player
  m_CameraController->SetShoulder(0.0f);     // Center behind, not on shoulder
  m_CameraController->SetHeight(1.6f);       // Eye level height

  // Create movement controller for player input handling
  m_MovementController = std::make_unique<ThirdPersonMovementController>(
      m_Registry, playerEntity);

  // 5. Spawn Initial Enemies
  SpawnEnemy("skel_minion", 12.0f, 10.0f, CharacterComponent::SkeletonMinion);
  SpawnEnemy("skel_warrior", 15.0f, 12.0f, CharacterComponent::SkeletonWarrior);
  SpawnEnemy("skel_minion", 8.0f, 15.0f, CharacterComponent::SkeletonMinion);
}

void SiegeMode::LoadMap(const std::string &path) {
  std::ifstream in(path);
  if (!in.is_open()) {
    std::cerr << "Failed to load map: " << path << std::endl;
    return;
  }
  std::string m, t;
  float x, y, z, r;
  while (in >> m >> t >> x >> y >> z >> r) {
    // Only load if not already loaded
    if (!m_Renderer->GetRenderMesh(m)) {
      m_Renderer->LoadMesh(m, "assets/dungeons/Assets/obj/" + m + ".obj");
    }
    auto e = m_Registry->CreateEntity();
    m_Registry->AddComponent<Transform3DComponent>(e, {x, y, z, r, 0});
    m_Registry->AddComponent<MeshComponent>(e, {m, t, 1, 1, 1});
  }
}

void SiegeMode::SpawnPlayer(Entity &playerEntity) {
  playerEntity = m_Registry->CreateEntity();
  m_Registry->AddComponent<Transform3DComponent>(
      playerEntity, {12.0f, 0.0f, 0.0f, 0.0f, 0.0f});
  m_Registry->AddComponent<MeshComponent>(playerEntity,
                                          {"Knight", "knight_tex", 1, 1, 1});
  m_Registry->AddComponent<PlayerControlComponent>(playerEntity,
                                                   {5.0f, 0.003f, 6.0f});
  m_Registry->AddComponent<PhysicsComponent>(
      playerEntity, {0, 0, 0, 15.0f, true, false, 0.0f, 1.0f});
  m_Registry->AddComponent<WeaponComponent>(playerEntity, {0.0f, 0.0f, false});
  m_Registry->AddComponent<BattleUnitComponent>(
      playerEntity, {BattleUnitComponent::Player, 100, 100, 15, 0});

  // Find Idle animation
  int idleIdx = 0;
  RenderMesh *rm = m_Renderer->GetRenderMesh("Knight");
  if (rm) {
    for (size_t i = 0; i < rm->animations.size(); i++) {
      if (rm->animations[i].name.find("Idle") != std::string::npos) {
        idleIdx = (int)i;
        break;
      }
    }
  }
  m_Registry->AddComponent<SkeletalAnimationComponent>(playerEntity,
                                                       {idleIdx, 0.0f, 1.0f});

  // Sword by default
  AttachmentComponent attach;
  attach.meshName = "Sword";
  attach.textureName = "knight_tex";
  attach.boneName = "hand.r";
  attach.rotX = -7.63f;
  attach.rotY = 18.148f;
  attach.rotZ = 1.702f;
  m_Registry->AddComponent<AttachmentComponent>(playerEntity, attach);
}

void SiegeMode::SpawnEnemy(const std::string &mesh, float x, float y,
                           CharacterComponent::Type type) {
  Entity e = m_Registry->CreateEntity();
  m_Registry->AddComponent<Transform3DComponent>(e, {x, y, 0.0f, 0.0f, 0.0f});
  m_Registry->AddComponent<MeshComponent>(e, {mesh, "skeleton_tex", 1, 1, 1});
  m_Registry->AddComponent<EnemyComponent>(e, {50.0f, 2.0f, -1});
  m_Registry->AddComponent<BattleUnitComponent>(
      e, {BattleUnitComponent::Enemy, 50, 50, 10, 0});
  m_Registry->AddComponent<ColliderComponent>(e, {0.5f, 1.8f, true});

  int idleIdx = 0;
  RenderMesh *rm = m_Renderer->GetRenderMesh(mesh);
  if (rm) {
    for (size_t i = 0; i < rm->animations.size(); i++) {
      if (rm->animations[i].name.find("Idle") != std::string::npos) {
        idleIdx = (int)i;
        break;
      }
    }
  }
  m_Registry->AddComponent<SkeletalAnimationComponent>(
      e, {idleIdx, (float)(rand() % 100) * 0.01f, 1.0f});
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

  // Raycast cursor for targeting
  RaycastCursor();
  if (m_Cursor.active) {
    // Player faces the mouse cursor on the ground
    t->rot = atan2(t->x - m_Cursor.x, t->y - m_Cursor.y);
  }

  // Update camera position (third-person follows player)
  m_CameraController->Update(dt);

  // Handle player movement with input controller
  if (m_MovementController) {
    m_MovementController->HandleInput(dt);
  }

  // Determine if player is moving for animation
  bool moving = phys && (phys->velX != 0 || phys->velY != 0);

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

  if (Input::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
    if (m_AttackTimer <= 0.0f) {

      std::string animName = (unit->preferredAction == 3)
                                 ? "Ranged_Bow_Shoot"
                                 : "Melee_1H_Attack_Chop";
      if (rm && anim) {
        for (size_t i = 0; i < rm->animations.size(); i++) {
          if (rm->animations[i].name.find(animName) != std::string::npos) {
            anim->animationIndex = (int)i;
            anim->currentTime = 0.0f;
            m_AttackTimer = rm->animations[i].duration;
            break;
          }
        }
      }

      if (unit->preferredAction == 3) { // Ranged
        if (m_SfxShoot)
          Mix_PlayChannel(-1, m_SfxShoot, 0);

        auto arrow = m_Registry->CreateEntity();
        m_Registry->AddComponent<Transform3DComponent>(
            arrow, {t->x, t->y, t->z + 1.2f, t->rot, m_CameraController->GetCameraPitch()});
        m_Registry->AddComponent<MeshComponent>(
            arrow, {"Arrow", "knight_tex", 1, 1, 1});
        m_Registry->AddComponent<ProjectileComponent>(
            arrow, {ProjectileComponent::Arrow, 25.0f, true, 5.0f});

        // Shoot in player's facing direction
        float shootForwardX = -sin(t->rot);
        float shootForwardY = -cos(t->rot);

        float projSpeed = 40.0f;
        m_Registry->AddComponent<PhysicsComponent>(
            arrow, {shootForwardX * projSpeed, shootForwardY * projSpeed, 0.0f,
                    0.0f, false, false, 0.0f, 0.0f});
      } else { // Melee
        bool hitSomething = false;
        auto &enemies = m_Registry->View<EnemyComponent>();
        std::vector<Entity> deadEnemies;

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
              if (eu->hp <= 0)
                deadEnemies.push_back(pair.first);
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

        for (auto e : deadEnemies)
          m_Registry->DestroyEntity(e);
      }
    }
  }

  // 6. Enemy AI Update (Chase player)
  auto &enemies = m_Registry->View<EnemyComponent>();
  for (auto &pair : enemies) {
    auto *et = m_Registry->GetComponent<Transform3DComponent>(pair.first);
    auto &enemy = pair.second;
    if (et) {
      float edx = t->x - et->x;
      float edy = t->y - et->y;
      float dist = sqrt(edx * edx + edy * edy);
      if (dist > 1.5f) {
        float angle = atan2(edy, edx);
        et->rot = angle + 1.57f;
        et->x += cos(angle) * enemy.speed * dt;
        et->y += sin(angle) * enemy.speed * dt;
      }
    }
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

void SiegeMode::RenderUI(GLRenderer *ren, TextRenderer *tr, int w, int h) {
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

  tr->RenderText(ren, "SIEGE MODE: 1 - SWORD, 2 - CROSSBOW", 20, 20,
                 {255, 255, 255, 255});
}

float SiegeMode::GetCameraYaw() const {
  if (m_CameraController) {
    return m_CameraController->GetYaw();
  }
  return 0.0f;
}