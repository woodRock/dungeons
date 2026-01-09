#include "../engine/Components.h"
#include "../engine/TextureManager.h"
#include "DungeonsGame.h"
#include <cmath>
#include <algorithm>

using namespace PixelsEngine;

namespace {
    // Helper to transform AABB from Mesh Space to Game World Space
    AABB GetWorldAABB(const AABB& local, const Transform3DComponent& t, float scaleX, float scaleY, float scaleZ) {
        float minX = 1e9, minY = 1e9, minZ = 1e9;
        float maxX = -1e9, maxY = -1e9, maxZ = -1e9;
        
        // 8 corners of local AABB
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
            float mx = corners[i][0];
            float my = corners[i][1];
            float mz = corners[i][2];
            
            // Apply Scaling (Note: GLRenderer maps Game Scale Z -> Mesh Y, Game Scale Y -> Mesh Z)
            float smx = mx * scaleX;
            float smy = my * scaleZ; 
            float smz = mz * scaleY; 
            
            // Apply Rotation (RotateY)
            float gl_x = smx * cosR + smz * sinR;
            float gl_y = smy;
            float gl_z = -smx * sinR + smz * cosR;
            
            // Apply Translation and Map to Game Space
            // Game X = GL X + t.x
            // Game Y = -GL Z + t.y
            // Game Z = GL Y + t.z
            
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
        
        return AABB(minX, minY, minZ, maxX, maxY, maxZ);
    }
}

void DungeonsGame::UpdatePhysics(float dt) {
  if (m_State != GameState::Playing && m_State != GameState::Creative &&
      m_State != GameState::Siege && m_State != GameState::Dungeon &&
      m_State != GameState::Sidescroller)
    return;

  auto &physicsView = m_Registry.View<PhysicsComponent>();
  
  for (auto &physPair : physicsView) {
    Entity entity = physPair.first;
    auto *phys = &physPair.second;
    auto *t = m_Registry.GetComponent<Transform3DComponent>(entity);

    if (!t) continue;



    // Floor Detection (Improved with AABB)
    float currentFloorHeight = -100.0f; // Default to falling
    bool overFloor = false;

    // Check against all tiles/meshes for floor
    auto &meshes = m_Registry.View<MeshComponent>();
    for (auto &mPair : meshes) {
        if (mPair.first == entity) continue;

        auto &mc = mPair.second;
        std::string lowName = mc.meshName;
        std::transform(lowName.begin(), lowName.end(), lowName.begin(), ::tolower);
        
        bool isWalkable = false;
        
        // 1. Name check
        if (lowName.find("floor") != std::string::npos || 
            lowName.find("stairs") != std::string::npos ||
            lowName.find("ground") != std::string::npos ||
            lowName.find("platform") != std::string::npos ||
            lowName.find("bridge") != std::string::npos) {
            isWalkable = true;
        }

        // 2. Hitbox check (climbable objects)
        if (!isWalkable) {
            auto *hc = m_Registry.GetComponent<HitboxComponent>(mPair.first);
            if (hc && hc->isClimbable) {
                isWalkable = true;
            }
        }
        
        if (isWalkable) {
            auto *ft = m_Registry.GetComponent<Transform3DComponent>(mPair.first);
            if (ft) {
                RenderMesh* rm = m_GLRenderer.GetRenderMesh(mc.meshName);
                if (rm) {
                    // Use transformed AABB for collision
                    AABB worldAABB = GetWorldAABB(rm->boundingBox, *ft, mc.scaleX, mc.scaleY, mc.scaleZ);
                    
                    // Check horizontal overlap with player (treating player as a point or small radius)
                    // We allow a small tolerance
                    if (t->x >= worldAABB.minX && t->x <= worldAABB.maxX &&
                        t->y >= worldAABB.minY && t->y <= worldAABB.maxY) {
                        
                        // Check vertical. We want the surface that is below or slightly above the player.
                        // Ideally, we want the highest surface that is <= t->z + stepHeight
                        
                        float floorZ = worldAABB.maxZ;
                        if (floorZ <= t->z + 1.5f) { // Allow stepping up/snapping
                             if (floorZ > currentFloorHeight) {
                                 currentFloorHeight = floorZ;
                                 overFloor = true;
                             }
                        }
                    }
                } else {
                    // Fallback to old logic if RenderMesh not found
                    float halfSize = 2.0f;
                    if (t->x >= ft->x - halfSize && t->x <= ft->x + halfSize &&
                        t->y >= ft->y - halfSize && t->y <= ft->y + halfSize) {
                        currentFloorHeight = std::max(currentFloorHeight, ft->z);
                        overFloor = true;
                    }
                }
            }
        }
    }

    // Base eye height adjustment
    float eyeOffset = 0.0f;
      if (entity == m_PlayerEntity && m_State == GameState::Sidescroller) {
        eyeOffset = 0.0f; // Keep the player feet on the platform in sidescroller
      } else if (entity == m_PlayerEntity && m_State != GameState::Siege && m_State != GameState::Dungeon) {
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

    // Ceiling collision (sidescroller mode: prevent jumping through bottom of tiles)
    if (m_State == GameState::Sidescroller && phys->velZ > 0.0f) {
      float playerHeight = 1.8f;
      float playerTop = t->z + playerHeight;
      float maxCeilingHeight = playerTop;

      auto &ceilingMeshes = m_Registry.View<MeshComponent>();
      for (auto &mPair : ceilingMeshes) {
        if (mPair.first == entity) continue;

        auto &mc = mPair.second;
        std::string lowName = mc.meshName;
        std::transform(lowName.begin(), lowName.end(), lowName.begin(), ::tolower);
        
        if (lowName.find("floor") != std::string::npos) {
          auto *ft = m_Registry.GetComponent<Transform3DComponent>(mPair.first);
          if (ft) {
            RenderMesh* rm = m_GLRenderer.GetRenderMesh(mc.meshName);
            if (rm) {
              AABB worldAABB = GetWorldAABB(rm->boundingBox, *ft, mc.scaleX, mc.scaleY, mc.scaleZ);
              
              // Check horizontal overlap
              if (t->x >= worldAABB.minX && t->x <= worldAABB.maxX &&
                  t->y >= worldAABB.minY && t->y <= worldAABB.maxY) {
                
                // Check if player is jumping into this ceiling
                float ceilingZ = worldAABB.minZ;
                if (playerTop > ceilingZ && playerTop < ceilingZ + 1.5f) {
                  t->z = ceilingZ - playerHeight;
                  phys->velZ = 0;
                }
              }
            }
          }
        }
      }
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

    // Horizontal Collision (Walls/Obstacles)
    float playerRadius = 0.4f;
    float playerHeight = 1.8f;

    // Check X Axis
    {
        float nextX = t->x + phys->velX * dt;
        AABB playerAABB(nextX - playerRadius, t->y - playerRadius, t->z,
                        nextX + playerRadius, t->y + playerRadius, t->z + playerHeight);
        
        bool collided = false;
        for (auto &mPair : meshes) {
            if (mPair.first == entity) continue;
            
            auto *hc = m_Registry.GetComponent<HitboxComponent>(mPair.first);
            if (!hc || !hc->isSolid) continue;

            auto *wt = m_Registry.GetComponent<Transform3DComponent>(mPair.first);
            if (!wt) continue;

            RenderMesh* rm = m_GLRenderer.GetRenderMesh(mPair.second.meshName);
            if (rm) {
                AABB wallAABB = GetWorldAABB(rm->boundingBox, *wt, mPair.second.scaleX, mPair.second.scaleY, mPair.second.scaleZ);
                if (playerAABB.Overlaps(wallAABB)) {
                    collided = true;
                    break;
                }
            }
        }
        if (collided) phys->velX = 0;
    }

    // Check Y Axis
    {
        float nextY = t->y + phys->velY * dt;
        // Note: We use t->x (current X) or updated X?
        // Usually separating axis allows sliding: Use the X that will apply if no collision, OR current X.
        // If we set velX=0 above, current X is effectively the "next" X.
        // So checking nextY with t->x is correct for independent axis resolution.
        
        AABB playerAABB(t->x - playerRadius, nextY - playerRadius, t->z,
                        t->x + playerRadius, nextY + playerRadius, t->z + playerHeight);
        
        bool collided = false;
        for (auto &mPair : meshes) {
            if (mPair.first == entity) continue;

            auto *hc = m_Registry.GetComponent<HitboxComponent>(mPair.first);
            if (!hc || !hc->isSolid) continue;

            auto *wt = m_Registry.GetComponent<Transform3DComponent>(mPair.first);
            if (!wt) continue;

            RenderMesh* rm = m_GLRenderer.GetRenderMesh(mPair.second.meshName);
            if (rm) {
                AABB wallAABB = GetWorldAABB(rm->boundingBox, *wt, mPair.second.scaleX, mPair.second.scaleY, mPair.second.scaleZ);
                if (playerAABB.Overlaps(wallAABB)) {
                    collided = true;
                    break;
                }
            }
        }
        if (collided) phys->velY = 0;
    }

    // Horizontal velocity application
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
    
    // Hitbox collision check (for procedural platforms)
    if (!hitWall) {
        auto &meshes = m_Registry.View<MeshComponent>();
        for (auto &mPair : meshes) {
            auto *hc = m_Registry.GetComponent<HitboxComponent>(mPair.first);
            auto *wt = m_Registry.GetComponent<Transform3DComponent>(mPair.first);
            if (hc && wt) {
                RenderMesh* rm = m_GLRenderer.GetRenderMesh(mPair.second.meshName);
                if (rm) {
                    AABB worldAABB = GetWorldAABB(rm->boundingBox, *wt, mPair.second.scaleX, mPair.second.scaleY, mPair.second.scaleZ);
                    if (worldAABB.Contains(t->x, t->y, t->z)) {
                        hitWall = true;
                        break;
                    }
                }
            }
        }
    }

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
            m_LastDungeonStats.enemiesKilled++;
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
      }
      toDestroy.push_back(entity);
      continue;
    }
  }

  for (auto e : toDestroy) {

    m_Registry.DestroyEntity(e);
  }
}
