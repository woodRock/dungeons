#include "../engine/Components.h"
#include "../engine/TextureManager.h"
#include "DungeonsGame.h"
#include <cmath>

using namespace PixelsEngine;

void DungeonsGame::UpdatePhysics(float dt) {
  if (m_State != GameState::Playing && m_State != GameState::Creative &&
      m_State != GameState::Siege && m_State != GameState::Dungeon)
    return;

  auto &physicsView = m_Registry.View<PhysicsComponent>();
  
  for (auto &physPair : physicsView) {
    Entity entity = physPair.first;
    auto *phys = &physPair.second;
    auto *t = m_Registry.GetComponent<Transform3DComponent>(entity);

    if (!t) continue;

    // Special handling for player grappling
    if (entity == m_PlayerEntity && m_IsGrappling) {
      float dx = m_GrapplePoint.x - t->x;
      float dy = m_GrapplePoint.y - t->y;
      float dz = m_GrapplePoint.z - t->z;
      float dist = sqrt(dx * dx + dy * dy + dz * dz);

      if (dist < 1.0f) {
        m_IsGrappling = false;
        phys->velZ = 5.0f;
      } else {
        float speed = 20.0f;
        phys->velX = (dx / dist) * speed;
        phys->velY = (dy / dist) * speed;
        phys->velZ = (dz / dist) * speed;
      }
    }

    // Floor Detection (Improved)
    float currentFloorHeight = -100.0f; // Default to falling
    bool overFloor = false;

    // Check against all tiles/meshes for floor
    auto &meshes = m_Registry.View<MeshComponent>();
    for (auto &mPair : meshes) {
        std::string lowName = mPair.second.meshName;
        std::transform(lowName.begin(), lowName.end(), lowName.begin(), ::tolower);
        
        if (lowName.find("floor") != std::string::npos || lowName.find("stairs") != std::string::npos) {
            auto *ft = m_Registry.GetComponent<Transform3DComponent>(mPair.first);
            if (ft) {
                // Tiles are 4x4
                float halfSize = 2.0f;
                if (t->x >= ft->x - halfSize && t->x <= ft->x + halfSize &&
                    t->y >= ft->y - halfSize && t->y <= ft->y + halfSize) {
                    currentFloorHeight = std::max(currentFloorHeight, ft->z);
                    overFloor = true;
                }
            }
        }
    }

    // Base eye height adjustment
    float eyeOffset = 0.0f;
    if (entity == m_PlayerEntity && m_State != GameState::Siege && m_State != GameState::Dungeon) {
        eyeOffset = phys->isSliding ? 0.75f : 1.5f;
    }
    
    float targetZ = currentFloorHeight + eyeOffset;

    // Apply Gravity
    if (!phys->isGrounded && !phys->isWallRunning && !phys->isFlying) {
      phys->velZ -= phys->gravity * dt;
    }

    t->z += phys->velZ * dt;

    // Floor collision
    if (overFloor && t->z < targetZ) {
      bool wasInAir = !phys->isGrounded && phys->velZ < -1.0f;
      t->z = targetZ;
      phys->velZ = 0;
      phys->isGrounded = true;
      if (wasInAir && entity == m_PlayerEntity && m_SfxJump) {
        Mix_PlayChannel(-1, m_SfxJump, 0);
      }
    } else if (t->z > targetZ + 0.05f) {
      phys->isGrounded = false;
    }

    // Wall Run Logic (Player Only)
    if (entity == m_PlayerEntity && !phys->isGrounded && phys->wallRunTimer > 0.0f) {
        // ... existing wall run logic ...
        float checkDist = 1.0f;
        float leftX = t->x + cos(t->rot - M_PI / 2.0f) * checkDist;
        float leftY = t->y + sin(t->rot - M_PI / 2.0f) * checkDist;
        float rightX = t->x + cos(t->rot + M_PI / 2.0f) * checkDist;
        float rightY = t->y + sin(t->rot + M_PI / 2.0f) * checkDist;
        bool wallLeft = (m_Map.Get((int)leftX, (int)leftY) == 2);
        bool wallRight = (m_Map.Get((int)rightX, (int)rightY) == 2);
        if (wallLeft || wallRight) {
            float speed = sqrt(phys->velX * phys->velX + phys->velY * phys->velY);
            if (speed > 0.5f) {
                phys->isWallRunning = true;
                phys->wallRunTimer -= dt;
                phys->velZ = 0.0f;
                phys->doubleJumpCount = 0;
            } else phys->isWallRunning = false;
        } else phys->isWallRunning = false;
    } else {
        phys->isWallRunning = false;
    }

    // Kill Plane Check
    if (t->z < -20.0f) {
      if (entity == m_PlayerEntity) {
          t->z = 5.0f; t->x = 12.0f; t->y = 0.0f; // Reset player
          phys->velX = phys->velY = phys->velZ = 0;
      } else {
          m_Registry.DestroyEntity(entity); // Destroy enemies/projectiles that fall
          continue;
      }
    }

    // Horizontal velocity
    t->x += phys->velX * dt;
    t->y += phys->velY * dt;

    // Drag
    float dragBase = phys->isGrounded ? phys->friction : (phys->isFlying ? 10.0f : 1.0f);
    float drag = 1.0f - (dragBase * dt);
    if (drag < 0) drag = 0;
    phys->velX *= drag;
    phys->velY *= drag;
    if (phys->isFlying) phys->velZ *= drag;
  }
}

void DungeonsGame::UpdateDoors(float dt) {
  auto *playerTransform = m_Registry.GetComponent<Transform3DComponent>(m_PlayerEntity);
  if (!playerTransform) return;
  
  auto &doors = m_Registry.View<DoorComponent>();
  for (auto &pair : doors) {
    auto *door = &pair.second;
    auto *doorTransform = m_Registry.GetComponent<Transform3DComponent>(pair.first);
    if (!doorTransform) continue;
    
    // Check if player is close to door (interaction range)
    float dx = playerTransform->x - doorTransform->x;
    float dy = playerTransform->y - doorTransform->y;
    float dist = sqrt(dx * dx + dy * dy);
    
    const float DOOR_INTERACT_RANGE = 2.5f;
    
    if (dist < DOOR_INTERACT_RANGE) {
      // Player is close, check if they want to interact (standing in front)
      // For now, auto-open doors when player is near
      if (!door->isOpen) {
        door->isOpen = true;
      }
    } else if (dist > 4.0f && door->isOpen) {
      // Player moved away, close door
      door->isOpen = false;
    }
    
    // Animate door opening/closing
    float targetOpenDistance = door->isOpen ? door->maxOpenDistance : 0.0f;
    float openSpeed = door->openSpeed * dt;
    
    if (door->currentOpenDistance < targetOpenDistance) {
      door->currentOpenDistance += openSpeed;
      if (door->currentOpenDistance > targetOpenDistance) {
        door->currentOpenDistance = targetOpenDistance;
      }
    } else if (door->currentOpenDistance > targetOpenDistance) {
      door->currentOpenDistance -= openSpeed;
      if (door->currentOpenDistance < targetOpenDistance) {
        door->currentOpenDistance = targetOpenDistance;
      }
    }
    
    // Update door position based on swing axis with hinge offset
    // For a left-hand hinge door, it swings outward from the hinge point
    switch (door->swingAxis) {
      case DoorComponent::X:
        // Door swings left/right from hinge on left side (hingeOffsetX < 0)
        doorTransform->x = door->originalX + door->hingeOffsetX + door->currentOpenDistance;
        break;
      case DoorComponent::Y:
        // Door swings forward/back from hinge
        doorTransform->y = door->originalY + door->hingeOffsetY + door->currentOpenDistance;
        break;
      case DoorComponent::Z:
        // Door swings up/down (unlikely but supported)
        doorTransform->z = door->originalZ + door->currentOpenDistance;
        break;
    }
  }
}

void DungeonsGame::UpdateProjectiles(float dt) {

  if (m_State != GameState::Playing && m_State != GameState::Siege && m_State != GameState::Dungeon)
    return;

  auto &projectiles = m_Registry.View<ProjectileComponent>();

  std::vector<Entity> toDestroy;

  for (auto &pair : projectiles) {

    Entity entity = pair.first;

    auto *p = &pair.second;

    if (!m_Registry.HasComponent<PhysicsComponent>(entity) ||
        !m_Registry.HasComponent<Transform3DComponent>(entity))
      continue;

    auto *phys = m_Registry.GetComponent<PhysicsComponent>(entity);

    auto *t = m_Registry.GetComponent<Transform3DComponent>(entity);

    t->x += phys->velX * dt;

    t->y += phys->velY * dt;

    t->z += phys->velZ * dt;

    phys->velZ -= 12.0f * dt; // Increased from 5.0

    p->lifeTime -= dt;

    bool hitWall = m_Map.Get(int(t->x), int(t->y)) > 0;
    bool hitFloor = t->z < 0;

    // Enemy Collision (Siege Mode)
    auto &enemies = m_Registry.View<EnemyComponent>();
    bool hitEnemy = false;
    for (auto &ePair : enemies) {
      Entity enemyEnt = ePair.first;
      auto &enemyComp = ePair.second;
      auto *et = m_Registry.GetComponent<Transform3DComponent>(enemyEnt);

      if (et) {
        float dx = t->x - et->x;
        float dy = t->y - et->y;
        float dz = t->z - (et->z + 1.0f); // Skeletons are tall
        float dist = sqrt(dx * dx + dy * dy + dz * dz);

        if (dist < 1.0f) {
          enemyComp.health -= p->damage;
          hitEnemy = true;
          m_HitmarkerTimer = 0.15f;
          if (m_SfxHit)
            Mix_PlayChannel(-1, m_SfxHit, 0);

          if (enemyComp.health <= 0) {
            // Death particles
            for (int i = 0; i < 10; i++) {
              auto frag = m_Registry.CreateEntity();
              m_Registry.AddComponent<Transform3DComponent>(
                  frag, {et->x, et->y, et->z + 1.0f, 0, 0});
              m_Registry.AddComponent<ParticleComponent>(
                  frag, {((rand() % 100) / 50.0f - 1.0f) * 3.0f,
                         ((rand() % 100) / 50.0f - 1.0f) * 3.0f,
                         ((rand() % 100) / 50.0f) * 3.0f,
                         1.0f,
                         1.0f,
                         {255, 0, 0, 255},
                         2.0f});
            }
            m_Registry.DestroyEntity(enemyEnt);
          }
          break;
        }
      }
    }

    if (hitEnemy) {
      toDestroy.push_back(entity);
      continue;
    }

    // Target Collision (Check BEFORE wall collision so we can hit targets on
    // walls/pillars)
    auto &targets = m_Registry.View<TargetComponent>();
    bool hitTarget = false;
    for (auto &tPair : targets) {
      Entity targetEnt = tPair.first;
      auto *tcomp = &tPair.second;

      if (!m_Registry.HasComponent<Transform3DComponent>(targetEnt) ||
          !m_Registry.HasComponent<ColliderComponent>(targetEnt))
        continue;

      auto *tt = m_Registry.GetComponent<Transform3DComponent>(targetEnt);
      auto *tc = m_Registry.GetComponent<ColliderComponent>(targetEnt);

      if (tcomp->isDestroyed)
        continue;

      float dist = sqrt(pow(t->x - tt->x, 2) + pow(t->y - tt->y, 2));
      if (dist < tc->radius && t->z < tt->z + 0.5f && t->z > tt->z - 0.5f) {
        // Hit
        tcomp->isDestroyed = true;
        m_TargetsDestroyed++;
        m_HitmarkerTimer = 0.15f;

        PlaySpatialSfx(m_SfxHit, tt->x, tt->y, tt->z);

        m_ShakeTimer = 0.3f;
        m_ShakeIntensity = 0.1f;

        auto *bill = m_Registry.GetComponent<BillboardComponent>(targetEnt);
        if (bill)
          bill->texture = TextureManager::LoadTexture(
              m_Renderer, "assets/target_broken.png");

        // Target explosion particles
        for (int i = 0; i < 15; i++) {
          auto frag = m_Registry.CreateEntity();
          m_Registry.AddComponent<Transform3DComponent>(
              frag, {tt->x, tt->y, tt->z + 0.2f, 0, 0});
          m_Registry.AddComponent<ParticleComponent>(
              frag, {((rand() % 100) / 50.0f - 1.0f) * 5.0f,
                     ((rand() % 100) / 50.0f - 1.0f) * 5.0f,
                     ((rand() % 100) / 50.0f) * 4.0f,
                     1.0f,
                     1.0f,
                     {255, 0, 0, 255},
                     3.0f});
        }

        hitTarget = true;
        break;
      }
    }

    if (hitTarget) {
      toDestroy.push_back(entity);
      continue;
    }

    if (p->lifeTime <= 0 || hitFloor || hitWall) {
      if (hitWall || hitFloor) {
        PlaySpatialSfx(m_SfxHit, t->x, t->y, t->z);
        // Spawn fragments
        for (int i = 0; i < 5; i++) {
          auto frag = m_Registry.CreateEntity();
          m_Registry.AddComponent<Transform3DComponent>(
              frag, {t->x, t->y, t->z, 0, 0});
          m_Registry.AddComponent<ParticleComponent>(
              frag, {((rand() % 100) / 50.0f - 1.0f) * 2.0f,
                     ((rand() % 100) / 50.0f - 1.0f) * 2.0f,
                     ((rand() % 100) / 100.0f) * 5.0f,
                     0.5f + (rand() % 100) / 100.0f,
                     1.0f,
                     {200, 150, 100, 255},
                     2.0f});
        }
        if (p->type == ProjectileComponent::Grapple) {
          m_IsGrappling = true;
          m_GrapplePoint = {t->x, t->y, t->z};
          m_ShakeTimer = 0.15f;
          m_ShakeIntensity = 0.05f;
        }
      }
      toDestroy.push_back(entity);
      continue;
    }
  }

  for (auto e : toDestroy) {

    m_Registry.DestroyEntity(e);
  }
}
