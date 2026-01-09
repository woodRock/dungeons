#include "StealthMode.h"
#include "../engine/TextRenderer.h"
#include "../engine/Input.h"
#include "../engine/Components.h"
#include "../engine/CharacterFactory.h"
#include "../engine/AssetManager.h"
#include "../engine/UIHelper.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <algorithm>
#include <fstream>

namespace PixelsEngine {

StealthMode::StealthMode(Registry* registry, GLRenderer* renderer)
    : m_Registry(registry), m_Renderer(renderer) {
}

StealthMode::~StealthMode() = default;

void StealthMode::Init(Camera* camera, Entity& playerEntity) {
    m_Active = true;
    m_Camera = camera;
    m_GameTime = 0.0f;
    
    // Load balance config
    LoadBalanceConfig();
    
    // Apply balance settings
    m_MaxDetectionDistance = m_BalanceSettings.maxDetectionDistance;
    m_ViewAngle = m_BalanceSettings.viewAngle;
    m_SneakVisibilityMultiplier = m_BalanceSettings.sneakVisibilityMultiplier;
    
    std::cout << "DEBUG: StealthMode::Init called" << std::endl;
    
    // Load character assets
    auto assetManager = std::make_unique<AssetManager>(m_Renderer);
    assetManager->LoadCharacter("Knight",
                               "assets/adventurers/Characters/gltf/Knight.glb",
                               "assets/adventurers/Textures/knight_texture.png");
    
    // Load skeleton warrior assets for enemies
    assetManager->LoadCharacter("skel_warrior",
                               "assets/skeletons/characters/gltf/Skeleton_Warrior.glb",
                               "assets/skeletons/characters/gltf/skeleton_texture.png");
    
    // Load Dagger
    m_Renderer->LoadMesh("Dagger", "assets/adventurers/Assets/gltf/dagger.gltf");
    
    // Load Sound
    m_SfxSwordHit = Mix_LoadWAV("assets/sword_hit.wav");
    
    // Load animation assets
    m_Renderer->LoadMesh("CharacterAnimations",
                        "assets/adventurers/Animations/gltf/Rig_Medium/Rig_Medium_General.glb");
    m_Renderer->LoadMesh("KnightMovement",
                        "assets/adventurers/Animations/gltf/Rig_Medium/Rig_Medium_MovementBasic.glb");
    m_Renderer->LoadMesh("KnightSneak",
                        "assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_MovementAdvanced.glb");
    m_Renderer->LoadMesh("PlayerCombat",
                        "assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_CombatMelee.glb");
    
    // Load special animations for skeletons (Death)
    m_Renderer->LoadMesh("SkeletonsSpecial",
                        "assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_Special.glb");
    
    LoadLevel("assets/saves/my_dungeon.map");
    
    // Initialize spawn editor and load spawn config BEFORE spawning enemies
    m_SpawnEditor = std::make_unique<StealthSpawnEditor>();
    m_SpawnEditor->LoadFromFile("assets/saves/spawn_config.txt");
    
    // Find player spawn
    float playerX = 36.0f;
    float playerY = -20.0f;
    float playerRot = 0.0f;
    bool playerSpawnFound = GetSpawnLocation(SpawnType::Player, playerX, playerY, playerRot);

    // Spawn player using CharacterFactory
    m_PlayerEntity = CharacterFactory::CreatePlayer(m_Registry, m_Renderer, playerX, playerY, 0.5f);
    playerEntity = m_PlayerEntity;
    
    // Apply rotation if found
    if (playerSpawnFound) {
         auto* transform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
         if (transform) {
             transform->rot = playerRot;
         }
    }
    
    // Change attachment to Dagger
    auto* attach = m_Registry->GetComponent<AttachmentComponent>(m_PlayerEntity);
    if (attach) {
        attach->meshName = "Dagger";
    }
    
    // Spawn enemies in the level
    SpawnEnemies();
    
    // Spawn objective
    SpawnObjective();
    
    // Create camera controller
    m_CameraController = std::make_unique<ThirdPersonCamera>(
        m_Camera, m_Registry, m_PlayerEntity);
    m_CameraController->SetDistance(3.0f);      
    m_CameraController->SetShoulder(0.0f);      
    m_CameraController->SetHeight(1.6f);        
    m_CameraController->SetYaw(-M_PI / 2);      
    
    // Create movement controller
    m_MovementController = std::make_unique<ThirdPersonMovementController>(m_Registry, m_PlayerEntity);
    
    // Setup animations for knight
    RenderMesh* charAnimMesh = m_Renderer->GetRenderMesh("CharacterAnimations");
    RenderMesh* knightMoveMesh = m_Renderer->GetRenderMesh("KnightMovement");
    RenderMesh* knightSneakMesh = m_Renderer->GetRenderMesh("KnightSneak");
    RenderMesh* knightCombatMesh = m_Renderer->GetRenderMesh("PlayerCombat");
    RenderMesh* knightMesh = m_Renderer->GetRenderMesh("Knight");
    
    if (knightMesh) {
        if (charAnimMesh) knightMesh->animations = charAnimMesh->animations;
        if (knightMoveMesh) {
            for (const auto& anim : knightMoveMesh->animations) knightMesh->animations.push_back(anim);
        }
        if (knightSneakMesh) {
            for (const auto& anim : knightSneakMesh->animations) knightMesh->animations.push_back(anim);
        }
        if (knightCombatMesh) {
            for (const auto& anim : knightCombatMesh->animations) knightMesh->animations.push_back(anim);
        }
    }
    
    // Setup animations for Skeleton
    RenderMesh* skelSpecialMesh = m_Renderer->GetRenderMesh("SkeletonsSpecial");
    RenderMesh* skelMesh = m_Renderer->GetRenderMesh("skel_warrior");
    
    if (skelSpecialMesh && skelMesh) {
        for (const auto& anim : skelSpecialMesh->animations) skelMesh->animations.push_back(anim);
    }
    
    // Initialize player animation to Idle
    auto* playerAnim = m_Registry->GetComponent<SkeletalAnimationComponent>(m_PlayerEntity);
    if (playerAnim && knightMesh) {
        for (size_t i = 0; i < knightMesh->animations.size(); ++i) {
            if (knightMesh->animations[i].name == "Idle_A") {
                playerAnim->animationIndex = i;
                break;
            }
        }
    }
    
    // Initialize console and visual spawn editor
    m_Console = std::make_unique<Console>();
    m_VisualSpawnEditor = std::make_unique<VisualSpawnEditor>(m_Registry, m_Renderer);
    
    auto spawnLocations = m_SpawnEditor->GetSpawnLocations();
    m_VisualSpawnEditor->SetSpawnLocations(spawnLocations);
    
    // Register console commands
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
            auto locs = m_VisualSpawnEditor->GetSpawnLocations();
            m_SpawnEditor->SetSpawnLocations(locs);
            m_SpawnEditor->SaveToFile("assets/saves/spawn_config.txt");
        }
    });
    
    m_Console->RegisterCommand("/balance", [this](const std::vector<std::string>& args) {
        m_BalanceMenuOpen = !m_BalanceMenuOpen;
        if (m_BalanceMenuOpen) {
            m_Console->Close();
            m_Console->SetInputEnabled(false);
            SDL_SetRelativeMouseMode(SDL_FALSE); // Need mouse for UI
        }
    });
    
    m_Console->RegisterCommand("/save", [this](const std::vector<std::string>& args) {
        if (m_VisualSpawnEditor->IsActive()) {
            auto locs = m_VisualSpawnEditor->GetSpawnLocations();
            m_SpawnEditor->SetSpawnLocations(locs);
        }
        m_SpawnEditor->SaveToFile("assets/saves/spawn_config.txt");
    });
    
    m_Console->RegisterCommand("/load", [this](const std::vector<std::string>& args) {
        m_SpawnEditor->LoadFromFile("assets/saves/spawn_config.txt");
    });
}

void StealthMode::LoadLevel(const std::string& mapFile) {
    m_Guards.clear();
    m_GuardStates.clear();
    m_GuardAlertLevels.clear();
    m_HideSpots.clear();
    MapLoader::LoadFromFile(mapFile, m_Registry, m_Renderer);
}

void StealthMode::Update(float dt, Entity& playerEntity) {
    if (!m_Active || !m_Camera)
        return;
        
    m_GameTime += dt;
    
    // Sync balance settings
    m_MaxDetectionDistance = m_BalanceSettings.maxDetectionDistance;
    m_ViewAngle = m_BalanceSettings.viewAngle;
    m_SneakVisibilityMultiplier = m_BalanceSettings.sneakVisibilityMultiplier;
    
    // Handle Balance Menu
    if (m_BalanceMenuOpen) {
        HandleBalanceMenuInput();
        return; // Pause game while in menu? Or just block input. Let's just block input.
    }
    
    // If detected
    if (m_Detected) {
        if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
            m_Active = false;
            return;
        }
        if (Input::IsKeyPressed(SDL_SCANCODE_R)) {
            ResetLevel();
            return;
        }
        return;
    }
    
    // Update console
    if (m_Console) {
        if (Input::IsKeyPressed(SDL_SCANCODE_GRAVE)) {
            if (m_VisualSpawnEditor->IsActive()) {
                m_Console->Toggle();
                m_Console->SetInputEnabled(m_Console->IsOpen());
                if (m_Console->IsOpen()) Input::StartTextInput(); else Input::StopTextInput();
            } else {
                m_Console->Update();
            }
        } else if (!m_VisualSpawnEditor->IsActive()) {
            m_Console->Update();
        } else if (m_Console->IsOpen()) {
            m_Console->ProcessInput();
        }
    }
    if (m_SpawnEditor) m_SpawnEditor->Update();
    
    if (m_Console && m_Console->IsOpen()) return;
    
    if (m_VisualSpawnEditor) m_VisualSpawnEditor->Update(dt, m_Camera, m_ScreenWidth, m_ScreenHeight);
    if (m_VisualSpawnEditor && m_VisualSpawnEditor->IsActive()) return;

    if (m_MovementController) m_MovementController->HandleInput(dt);

    auto* playerPhys = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
    auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    
    if (!playerPhys || !playerTransform) return;
    
    if (m_CameraController) {
        m_CameraController->HandleInput(dt);
        m_CameraController->Update(dt);
    }

    if (Input::IsKeyPressed(SDL_SCANCODE_LSHIFT)) {
        m_PlayerSneaking = !m_PlayerSneaking;
    }
    
    UpdateGuardAI(dt);
    CheckObjectives();
    CheckTakedowns();
    CheckPlayerDetection();
    
    float speed = sqrt(playerPhys->velX * playerPhys->velX + playerPhys->velY * playerPhys->velY);
    if (speed < 0.2f && m_PlayerAlertLevel > 0.0f) {
        m_PlayerAlertLevel = std::max(0.0f, m_PlayerAlertLevel - m_AlertDecayRate * dt);
    }
    
    UpdatePlayerAnimation(speed, playerPhys->isGrounded);
    
    if (m_CameraController) {
        float player_rotaion_offset = M_PI + M_PI / 2.0f;
        playerTransform->rot = -m_CameraController->GetCameraYaw() + player_rotaion_offset;
    }
}

void StealthMode::UpdatePlayerAnimation(float speed, bool isGrounded) {
    auto* playerAnim = m_Registry->GetComponent<SkeletalAnimationComponent>(m_PlayerEntity);
    RenderMesh* knightMesh = m_Renderer->GetRenderMesh("Knight");
    if (!playerAnim || !knightMesh) return;
    
    std::string targetAnim = "Idle_A";
    if (!isGrounded && speed > 0.5f) {
        targetAnim = "Jump_Full_Long";
    } else if (speed > 1.0f) {
        targetAnim = m_PlayerSneaking ? "sneaking" : "Running_A";
    } else if (speed > 0.2f) {
        targetAnim = m_PlayerSneaking ? "sneaking" : "Walking_A";
    } else {
        targetAnim = m_PlayerSneaking ? "Crouching" : "Idle_A";
    }
    
    int targetIdx = -1;
    for (size_t i = 0; i < knightMesh->animations.size(); ++i) {
        if (knightMesh->animations[i].name == targetAnim) {
            targetIdx = i;
            break;
        }
    }
    
    if (targetIdx >= 0 && (int)playerAnim->animationIndex != targetIdx) {
        playerAnim->animationIndex = targetIdx;
        playerAnim->currentTime = 0.0f;
    }
}

void StealthMode::UpdateGuardAI(float dt) {
    auto* playerPhys = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
    auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    
    bool playerMakingNoise = false;
    if (playerPhys && !m_PlayerSneaking) {
        float speed = sqrt(playerPhys->velX * playerPhys->velX + playerPhys->velY * playerPhys->velY);
        if (speed > m_BalanceSettings.noiseSpeedThreshold) playerMakingNoise = true;
    }
    if (m_GameTime < m_BalanceSettings.gracePeriod) playerMakingNoise = false;

    for (size_t i = 0; i < m_Guards.size(); ++i) {
        if (m_GuardStates[i] == GuardState::DEAD) continue;
        
        auto* guardTransform = m_Registry->GetComponent<Transform3DComponent>(m_Guards[i]);
        auto* guardPhys = m_Registry->GetComponent<PhysicsComponent>(m_Guards[i]);
        auto* patrol = m_Registry->GetComponent<PatrolComponent>(m_Guards[i]);
        
        if (!guardTransform || !playerTransform) continue;
        
        float distToPlayer = sqrt(pow(guardTransform->x - playerTransform->x, 2) + pow(guardTransform->y - playerTransform->y, 2));
        
        if (m_PlayerAlertLevel > 0.8f) {
            m_GuardStates[i] = GuardState::ALERT;
        } else if (m_GuardStates[i] != GuardState::ALERT) {
            bool heardNoise = playerMakingNoise && (distToPlayer < m_BalanceSettings.hearingDistance);
            bool sawGlimpse = m_PlayerAlertLevel > 0.3f;
            if (heardNoise || sawGlimpse) {
                if (m_GuardStates[i] != GuardState::SEARCHING) {
                    m_GuardStates[i] = GuardState::SEARCHING;
                    m_GuardSearchTimers[i] = 5.0f;
                }
                m_GuardSearchTargets[i] = {playerTransform->x, playerTransform->y};
            }
        }
        
        if (m_GuardStates[i] == GuardState::ALERT) {
            float dirX = playerTransform->x - guardTransform->x;
            float dirY = playerTransform->y - guardTransform->y;
            float len = sqrt(dirX * dirX + dirY * dirY);
            if (len > 0.5f) {
                dirX /= len; dirY /= len;
                if (guardPhys) {
                    guardPhys->velX = dirX * 6.0f;
                    guardPhys->velY = dirY * 6.0f;
                    guardTransform->rot = atan2(dirY, dirX) + M_PI / 2.0f;
                }
            } else if (guardPhys) { guardPhys->velX = 0; guardPhys->velY = 0; }
        } 
        else if (m_GuardStates[i] == GuardState::SEARCHING) {
            float targetX = m_GuardSearchTargets[i].first;
            float targetY = m_GuardSearchTargets[i].second;
            float dirX = targetX - guardTransform->x;
            float dirY = targetY - guardTransform->y;
            float dist = sqrt(dirX * dirX + dirY * dirY);
            if (dist < 1.0f) {
                if (guardPhys) { guardPhys->velX = 0; guardPhys->velY = 0; }
                m_GuardSearchTimers[i] -= dt;
                if (m_GuardSearchTimers[i] <= 0.0f) m_GuardStates[i] = GuardState::IDLE;
                guardTransform->rot += 1.0f * dt;
            } else {
                dirX /= dist; dirY /= dist;
                if (guardPhys) {
                    guardPhys->velX = dirX * 3.0f;
                    guardPhys->velY = dirY * 3.0f;
                    float targetRot = atan2(dirY, dirX) + M_PI / 2.0f;
                    float currentRot = guardTransform->rot;
                    while (targetRot - currentRot > M_PI) targetRot -= 2 * M_PI;
                    while (targetRot - currentRot < -M_PI) targetRot += 2 * M_PI;
                    guardTransform->rot = currentRot + (targetRot - currentRot) * 5.0f * dt;
                }
            }
        }
        else if (m_GuardStates[i] == GuardState::IDLE) {
            if (patrol && !patrol->waypoints.empty()) {
                if (patrol->waitTimer > 0.0f) {
                    patrol->waitTimer -= dt;
                    if (guardPhys) { guardPhys->velX = 0; guardPhys->velY = 0; }
                } else {
                    float targetX = patrol->waypoints[patrol->currentWaypointIndex].first;
                    float targetY = patrol->waypoints[patrol->currentWaypointIndex].second;
                    float dirX = targetX - guardTransform->x;
                    float dirY = targetY - guardTransform->y;
                    float dist = sqrt(dirX * dirX + dirY * dirY);
                    if (dist < 0.5f) {
                        patrol->waitTimer = patrol->waitDuration;
                        patrol->currentWaypointIndex = (patrol->currentWaypointIndex + 1) % patrol->waypoints.size();
                        if (guardPhys) { guardPhys->velX = 0; guardPhys->velY = 0; }
                    } else {
                        dirX /= dist; dirY /= dist;
                        if (guardPhys) {
                            guardPhys->velX = dirX * 2.0f;
                            guardPhys->velY = dirY * 2.0f;
                            float targetRot = atan2(dirY, dirX) + M_PI / 2.0f;
                            float currentRot = guardTransform->rot;
                            while (targetRot - currentRot > M_PI) targetRot -= 2 * M_PI;
                            while (targetRot - currentRot < -M_PI) targetRot += 2 * M_PI;
                            guardTransform->rot = currentRot + (targetRot - currentRot) * 5.0f * dt;
                        }
                    }
                }
            } else if (guardPhys) { guardPhys->velX = 0; guardPhys->velY = 0; }
        }
        
        auto* anim = m_Registry->GetComponent<SkeletalAnimationComponent>(m_Guards[i]);
        RenderMesh* mesh = m_Renderer->GetRenderMesh("skel_warrior");
        if (anim && mesh) {
            bool isMoving = (guardPhys && (fabs(guardPhys->velX) > 0.1f || fabs(guardPhys->velY) > 0.1f));
            std::string animName = "Idle_A";
            if (m_GuardStates[i] == GuardState::ALERT) animName = "Running_A";
            else if (isMoving) animName = "Walking_A";
            for (size_t k = 0; k < mesh->animations.size(); ++k) {
                if (mesh->animations[k].name == animName) {
                    if (anim->animationIndex != (int)k) { anim->animationIndex = k; anim->currentTime = 0.0f; }
                    break;
                }
            }
        }
    }
}

void StealthMode::CheckPlayerDetection() {
    auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    if (!playerTransform || IsPlayerInHideSpot()) {
        if (IsPlayerInHideSpot()) m_PlayerAlertLevel = std::max(0.0f, m_PlayerAlertLevel - m_BalanceSettings.alertDecayRate * 0.016f); // Use balance setting
        return;
    }
    for (size_t i = 0; i < m_Guards.size(); ++i) {
        if (m_GuardStates[i] == GuardState::DEAD) continue;
        bool canSee = false;
        CheckLineOfSight(m_Guards[i], m_PlayerEntity, canSee);
        if (canSee) {
            float detectionIncrease = 0.3f;
            if (m_PlayerSneaking) detectionIncrease *= m_SneakVisibilityMultiplier;
            m_PlayerAlertLevel = std::min(1.0f, m_PlayerAlertLevel + detectionIncrease);
            m_GuardStates[i] = GuardState::ALERT;
        }
    }
    m_Detected = m_PlayerAlertLevel > 0.7f;
}

bool RayAABBIntersection(float startX, float startY, float endX, float endY, float minX, float maxX, float minY, float maxY, float& outDistance) {
    float dirX = endX - startX; float dirY = endY - startY;
    float rayLength = sqrt(dirX * dirX + dirY * dirY);
    if (rayLength < 0.001f) return false;
    float dirNormX = dirX / rayLength; float dirNormY = dirY / rayLength;
    float tMin = 0.0f; float tMax = rayLength;
    if (fabs(dirNormX) > 0.001f) {
        float t1 = (minX - startX) / dirNormX; float t2 = (maxX - startX) / dirNormX;
        if (t1 > t2) std::swap(t1, t2);
        tMin = std::max(tMin, t1); tMax = std::min(tMax, t2);
    } else if (startX < minX || startX > maxX) return false;
    if (fabs(dirNormY) > 0.001f) {
        float t1 = (minY - startY) / dirNormY; float t2 = (maxY - startY) / dirNormY;
        if (t1 > t2) std::swap(t1, t2);
        tMin = std::max(tMin, t1); tMax = std::min(tMax, t2);
    } else if (startY < minY || startY > maxY) return false;
    if (tMin > tMax || tMax < 0.001f) return false;
    outDistance = std::max(0.0f, tMin);
    return true;
}

bool StealthMode::RayHitsCollider(float startX, float startY, float endX, float endY, float& outDistance) {
    outDistance = std::numeric_limits<float>::max();
    auto& hitboxes = m_Registry->View<HitboxComponent>();
    bool foundCollision = false;
    for (auto& pair : hitboxes) {
        Entity entity = pair.first; HitboxComponent& hitbox = pair.second;
        if (!hitbox.isSolid || entity == m_PlayerEntity) continue;
        bool isGuard = false;
        for (const auto& guard : m_Guards) if (guard == entity) { isGuard = true; break; }
        if (isGuard) continue;
        auto* transform = m_Registry->GetComponent<Transform3DComponent>(entity);
        if (!transform) continue;
        float minX, maxX, minY, maxY, minZ, maxZ;
        hitbox.GetBounds(transform->x, transform->y, transform->z, transform->rot, minX, maxX, minY, maxY, minZ, maxZ);
        float distance = 0.0f;
        if (RayAABBIntersection(startX, startY, endX, endY, minX, maxX, minY, maxY, distance)) {
            foundCollision = true;
            if (distance < outDistance) outDistance = distance;
        }
    }
    return foundCollision;
}

void StealthMode::CheckLineOfSight(Entity guardEntity, Entity playerEntity, bool& canSee) {
    auto* guardTransform = m_Registry->GetComponent<Transform3DComponent>(guardEntity);
    auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(playerEntity);
    if (!guardTransform || !playerTransform) { canSee = false; return; }
    float distX = playerTransform->x - guardTransform->x;
    float distY = playerTransform->y - guardTransform->y;
    float distance = sqrt(distX * distX + distY * distY);
    if (distance > m_MaxDetectionDistance) { canSee = false; return; }
    float playerAngle = atan2(distY, distX);
    float guardFacing = guardTransform->rot - M_PI / 2.0f;
    float angleDiff = fabs(playerAngle - guardFacing);
    while (angleDiff > M_PI) angleDiff = 2 * M_PI - angleDiff;
    if (angleDiff >= (m_ViewAngle * M_PI / 360.0f)) { canSee = false; return; }
    float collisionDist = 0.0f;
    if (RayHitsCollider(guardTransform->x, guardTransform->y, playerTransform->x, playerTransform->y, collisionDist)) {
        if (collisionDist < distance) { canSee = false; return; }
    }
    canSee = true;
}

bool StealthMode::IsPlayerInHideSpot() {
    auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    if (!playerTransform) return false;
    for (auto& spot : m_HideSpots) {
        float distX = playerTransform->x - spot.x; float distY = playerTransform->y - spot.y;
        if (sqrt(distX * distX + distY * distY) < spot.radius) return true;
    }
    return false;
}

float StealthMode::GetPlayerVisibility() {
    auto* playerPhys = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
    if (!playerPhys) return 0.5f;
    float speed = sqrt(playerPhys->velX * playerPhys->velX + playerPhys->velY * playerPhys->velY);
    return std::min(1.0f, speed / 4.0f);
}

void StealthMode::RenderUI(GLRenderer* renderer, TextRenderer* textRenderer, int width, int height) {
    if (!m_Active) return;
    m_ScreenWidth = width; m_ScreenHeight = height;
    
    // Balance Menu (Draw on top)
    if (m_BalanceMenuOpen) {
        RenderBalanceMenu(renderer, textRenderer, width, height);
        return;
    }
    
    textRenderer->RenderText(renderer, "Stealth Mode", 20, 20, {255, 255, 255, 255});
    textRenderer->RenderText(renderer, "WASD: Move | Space: Jump | ESC: Back", 20, 50, {255, 255, 255, 255});
    textRenderer->RenderText(renderer, "F: Takedown | P: Patrol Edit | /balance: Menu", 20, 65, {255, 200, 100, 255});
    
    std::string alertText = "Alert Level: " + std::to_string((int)(m_PlayerAlertLevel * 100)) + "%" ;
    SDL_Color alertColor = {0, 255, 0, 255};
    if (m_PlayerAlertLevel > 0.7f) alertColor = {255, 0, 0, 255};
    else if (m_PlayerAlertLevel > 0.3f) alertColor = {255, 255, 0, 255};
    textRenderer->RenderText(renderer, alertText, 20, 80, alertColor);
    
    if (IsPlayerInHideSpot()) textRenderer->RenderText(renderer, "HIDDEN", 20, 110, {0, 255, 0, 255});
    if (m_PlayerSneaking) textRenderer->RenderText(renderer, "SNEAKING (Shift to toggle)", 20, 170, {150, 200, 150, 255});
    
    RenderMinimap(renderer, textRenderer, width, height);
    RenderGuardLineOfSight();
    
    if (m_Detected) {
        textRenderer->RenderText(renderer, "DETECTED! Game Over. Press R to Replay or ESC to return.", width/2 - 250, height/2, {255, 0, 0, 255});
    }
    if (m_MissionComplete) {
        textRenderer->RenderText(renderer, "MISSION COMPLETE!", width/2 - 150, height/2 - 50, {0, 255, 0, 255});
    } else {
        RenderObjectiveMarker(renderer, textRenderer, width, height);
    }
    
    if (m_Console) m_Console->Render(renderer, textRenderer, width, height);
    if (m_SpawnEditor) m_SpawnEditor->Render(renderer, textRenderer, width, height);
    if (m_VisualSpawnEditor) m_VisualSpawnEditor->Render(renderer, textRenderer, m_Camera, width, height);
}

void StealthMode::RenderMinimap(GLRenderer* renderer, TextRenderer* textRenderer, int screenWidth, int screenHeight) {
    int minimapX = screenWidth - m_MinimapWidth - 10;
    int minimapY = 10;
    UIHelper::DrawPanel(renderer, minimapX, minimapY, m_MinimapWidth, m_MinimapHeight, {30, 30, 30, 220});
    auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    if (!playerTransform) return;
    float minimapCenterX = minimapX + m_MinimapWidth * 0.5f;
    float minimapCenterY = minimapY + m_MinimapHeight * 0.5f;
    float cameraYaw = m_CameraController ? m_CameraController->GetCameraYaw() : 0.0f;
    renderer->DrawRect2D(minimapCenterX - 4, minimapCenterY - 4, 8, 8, {0, 255, 0, 255});
    for (Entity guardEntity : m_Guards) {
        auto* guardTransform = m_Registry->GetComponent<Transform3DComponent>(guardEntity);
        if (!guardTransform) continue;
        float relX = guardTransform->x - playerTransform->x;
        float relY = guardTransform->y - playerTransform->y;
        float rotatedRelX = relX * cos(-cameraYaw) - relY * sin(-cameraYaw);
        float rotatedRelY = relX * sin(-cameraYaw) + relY * cos(-cameraYaw);
        float guardScreenX = minimapCenterX + rotatedRelX * m_MinimapScale;
        float guardScreenY = minimapCenterY + rotatedRelY * m_MinimapScale;
        if (guardScreenX > minimapX && guardScreenX < minimapX + m_MinimapWidth && guardScreenY > minimapY && guardScreenY < minimapY + m_MinimapHeight) {
            renderer->DrawRect2D(guardScreenX - 3, guardScreenY - 3, 6, 6, {255, 0, 0, 255});
        }
    }
}

void StealthMode::SpawnEnemies() {
    std::vector<SpawnLocation> spawnLocations;
    if (!m_SpawnEditor) return;
    spawnLocations = m_SpawnEditor->GetSpawnLocations();
    for (const auto& loc : spawnLocations) {
        if (loc.type != SpawnType::Enemy) continue;
        Entity enemy = CharacterFactory::CreateSkeleton(m_Registry, m_Renderer, "skel_warrior", loc.x, loc.y, 0.5f);
        if (enemy != INVALID_ENTITY) {
            auto* transform = m_Registry->GetComponent<Transform3DComponent>(enemy);
            if (transform) transform->rot = loc.rotation;
            if (!loc.waypoints.empty()) {
                PatrolComponent patrol;
                patrol.waypoints = loc.waypoints;
                patrol.currentWaypointIndex = 0;
                patrol.waitTimer = 0.0f;
                m_Registry->AddComponent<PatrolComponent>(enemy, patrol);
            }
            m_Guards.push_back(enemy);
            m_GuardStates.push_back(GuardState::IDLE);
            m_GuardAlertLevels.push_back(0.0f);
            m_GuardSearchTimers.push_back(0.0f);
            m_GuardSearchTargets.push_back({0.0f, 0.0f});
        }
    }
}

void StealthMode::RenderGuardLineOfSight() {
    if (!m_Renderer) return;
    for (size_t i = 0; i < m_Guards.size(); ++i) {
        if (m_GuardStates[i] == GuardState::DEAD) continue;
        auto* guardTransform = m_Registry->GetComponent<Transform3DComponent>(m_Guards[i]);
        if (!guardTransform) continue;
        const int coneSegments = 16;
        const float viewAngleRad = (m_ViewAngle / 2.0f) * M_PI / 180.0f;
        SDL_Color coneColor = (m_GuardStates[i] == GuardState::ALERT) ? SDL_Color{255, 0, 0, 100} : SDL_Color{200, 100, 100, 80};
        for (int j = 0; j < coneSegments; ++j) {
            float angle1 = guardTransform->rot - M_PI/2 - viewAngleRad + (2.0f * viewAngleRad * j / coneSegments);
            float angle2 = guardTransform->rot - M_PI/2 - viewAngleRad + (2.0f * viewAngleRad * (j + 1) / coneSegments);
            float dist1 = m_MaxDetectionDistance, dist2 = m_MaxDetectionDistance;
            float collDist1, collDist2;
            if (RayHitsCollider(guardTransform->x, guardTransform->y, guardTransform->x + cos(angle1) * m_MaxDetectionDistance, guardTransform->y + sin(angle1) * m_MaxDetectionDistance, collDist1)) dist1 = collDist1;
            if (RayHitsCollider(guardTransform->x, guardTransform->y, guardTransform->x + cos(angle2) * m_MaxDetectionDistance, guardTransform->y + sin(angle2) * m_MaxDetectionDistance, collDist2)) dist2 = collDist2;
            float x1 = guardTransform->x + cos(angle1) * dist1, y1 = guardTransform->y + sin(angle1) * dist1;
            float x2 = guardTransform->x + cos(angle2) * dist2, y2 = guardTransform->y + sin(angle2) * dist2;
            m_Renderer->DrawLine(guardTransform->x, guardTransform->y, 0.5f, x1, y1, 0.5f, coneColor);
            m_Renderer->DrawLine(x1, y1, 0.5f, x2, y2, 0.5f, coneColor);
        }
    }
}

void StealthMode::SpawnObjective() {
    float objX = 10.0f, objY = 10.0f, objZ = 0.5f;
    float objRot = 0.0f;
    GetSpawnLocation(SpawnType::Objective, objX, objY, objRot);

    m_ObjectiveEntity = m_Registry->CreateEntity();
    m_Registry->AddComponent<Transform3DComponent>(m_ObjectiveEntity, {objX, objY, objZ, 0.0f, 0.0f});
    m_Registry->AddComponent<MeshComponent>(m_ObjectiveEntity, {"ChestGold", "assets/dungeons/Assets/gltf/chest_gold.gltf", 1.0f, 1.0f, 1.0f});
    ObjectiveComponent obj; obj.type = ObjectiveComponent::Exit; obj.radius = 2.0f;
    m_Registry->AddComponent<ObjectiveComponent>(m_ObjectiveEntity, obj);
    m_Renderer->LoadMesh("ChestGold", "assets/dungeons/Assets/gltf/chest_gold.gltf");
}

void StealthMode::CheckObjectives() {
    if (m_MissionComplete) return;
    auto* pT = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    auto* oT = m_Registry->GetComponent<Transform3DComponent>(m_ObjectiveEntity);
    if (!pT || !oT) return;
    if (sqrt(pow(pT->x - oT->x, 2) + pow(pT->y - oT->y, 2)) < 2.0f) m_MissionComplete = true;
}

void StealthMode::RenderObjectiveMarker(GLRenderer* renderer, TextRenderer* textRenderer, int screenWidth, int screenHeight) {
    if (m_MissionComplete || !m_Camera) return;
    auto* oT = m_Registry->GetComponent<Transform3DComponent>(m_ObjectiveEntity);
    if (!oT) return;
    float angleToObj = atan2(oT->y - m_Camera->y, oT->x - m_Camera->x) - m_Camera->yaw;
    while (angleToObj > M_PI) angleToObj -= 2 * M_PI;
    while (angleToObj < -M_PI) angleToObj += 2 * M_PI;
    if (fabs(angleToObj) < M_PI / 2.0f) {
        float drawX = (screenWidth / 2) - angleToObj * 200.0f;
        if (drawX > 20 && drawX < screenWidth - 20) textRenderer->RenderText(renderer, "OBJ", drawX, 35, {255, 215, 0, 255});
    }
}

void StealthMode::CheckTakedowns() {
    if (Input::IsKeyPressed(SDL_SCANCODE_F)) {
        auto* pT = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
        if (!pT) return;
        for (size_t i = 0; i < m_Guards.size(); ++i) {
            if (m_GuardStates[i] == GuardState::DEAD || m_GuardStates[i] == GuardState::ALERT) continue;
            auto* gT = m_Registry->GetComponent<Transform3DComponent>(m_Guards[i]);
            if (!gT) continue;
            float dist = sqrt(pow(gT->x - pT->x, 2) + pow(gT->y - pT->y, 2));
            if (dist < 1.5f) {
                float pDirX = cos(pT->rot - M_PI/2), pDirY = sin(pT->rot - M_PI/2);
                float gDirX = cos(gT->rot - M_PI/2), gDirY = sin(gT->rot - M_PI/2);
                float dot = pDirX * gDirX + pDirY * gDirY;
                if (dot > 0.5f) {
                    // Execute Takedown
                    m_GuardStates[i] = GuardState::DEAD;
                    std::cout << "Takedown on Guard " << i << "!" << std::endl;
                    
                    // Play sound
                    if (m_SfxSwordHit) Mix_PlayChannel(-1, m_SfxSwordHit, 0);
                    
                    // Player animation
                    auto* pAnim = m_Registry->GetComponent<SkeletalAnimationComponent>(m_PlayerEntity);
                    RenderMesh* pMesh = m_Renderer->GetRenderMesh("Knight");
                    if (pAnim && pMesh) {
                        for (size_t k = 0; k < pMesh->animations.size(); ++k) {
                            if (pMesh->animations[k].name == "Melee_1H_Attack_Stab") {
                                pAnim->animationIndex = k;
                                pAnim->currentTime = 0.0f;
                                pAnim->loop = false;
                                break;
                            }
                        }
                    }
                    
                    // Play death animation
                    auto* anim = m_Registry->GetComponent<SkeletalAnimationComponent>(m_Guards[i]);
                    RenderMesh* mesh = m_Renderer->GetRenderMesh("skel_warrior");
                    if (anim && mesh) {
                        for (size_t k = 0; k < mesh->animations.size(); ++k) {
                            if (mesh->animations[k].name == "Skeletons_Death") {
                                anim->animationIndex = k;
                                anim->currentTime = 0.0f;
                                anim->loop = false; // Don't loop death
                                break;
                            }
                        }
                    }
                    
                    // Stop movement physics
                    auto* phys = m_Registry->GetComponent<PhysicsComponent>(m_Guards[i]);
                    if (phys) {
                        phys->velX = 0;
                        phys->velY = 0;
                    }
                    
                    // Disable hitbox (so we don't collide with body)
                    auto* hitbox = m_Registry->GetComponent<HitboxComponent>(m_Guards[i]);
                    if (hitbox) {
                        hitbox->isSolid = false;
                    }
                    
                    return; // One takedown per press
                }
            }
        }
    }
}

void StealthMode::ResetLevel() {
    m_Detected = false; m_MissionComplete = false; m_PlayerAlertLevel = 0.0f; m_GameTime = 0.0f; m_PlayerSneaking = false;
    for (Entity guard : m_Guards) if (guard != INVALID_ENTITY) m_Registry->DestroyEntity(guard);
    m_Guards.clear(); m_GuardStates.clear(); m_GuardAlertLevels.clear(); m_GuardSearchTimers.clear(); m_GuardSearchTargets.clear();
    if (m_ObjectiveEntity != INVALID_ENTITY) { m_Registry->DestroyEntity(m_ObjectiveEntity); m_ObjectiveEntity = INVALID_ENTITY; }
    
    float playerX = 36.0f;
    float playerY = -20.0f;
    float playerRot = 0.0f;
    bool playerSpawnFound = GetSpawnLocation(SpawnType::Player, playerX, playerY, playerRot);

    auto* transform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    auto* phys = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
    if (transform) { 
        transform->x = playerX; 
        transform->y = playerY; 
        transform->z = 0.5f; 
        if (playerSpawnFound) transform->rot = playerRot;
    }
    if (phys) { phys->velX = 0; phys->velY = 0; phys->velZ = 0; }
    SpawnEnemies(); SpawnObjective();
}

void StealthMode::LoadBalanceConfig() {
    std::ifstream file("assets/saves/balance_config.txt");
    if (file.is_open()) {
        file >> m_BalanceSettings.maxDetectionDistance;
        file >> m_BalanceSettings.viewAngle;
        file >> m_BalanceSettings.sneakVisibilityMultiplier;
        file >> m_BalanceSettings.noiseSpeedThreshold;
        file >> m_BalanceSettings.hearingDistance;
        file >> m_BalanceSettings.gracePeriod;
        file >> m_BalanceSettings.alertDecayRate;
        file.close();
        std::cout << "Loaded balance config." << std::endl;
    }
}

void StealthMode::SaveBalanceConfig() {
    std::ofstream file("assets/saves/balance_config.txt");
    if (file.is_open()) {
        file << m_BalanceSettings.maxDetectionDistance << "\n";
        file << m_BalanceSettings.viewAngle << "\n";
        file << m_BalanceSettings.sneakVisibilityMultiplier << "\n";
        file << m_BalanceSettings.noiseSpeedThreshold << "\n";
        file << m_BalanceSettings.hearingDistance << "\n";
        file << m_BalanceSettings.gracePeriod << "\n";
        file << m_BalanceSettings.alertDecayRate << "\n";
        file.close();
        std::cout << "Saved balance config." << std::endl;
    }
}

bool DoSlider(GLRenderer* renderer, TextRenderer* textRenderer, const std::string& label, float& value, float min, float max, int x, int y, int width, int& mouseY, int& mouseX, bool mouseDown) {
    textRenderer->RenderText(renderer, label, x, y, {200, 200, 200, 255});
    std::string valStr = std::to_string(value);
    valStr = valStr.substr(0, valStr.find(".") + 3);
    textRenderer->RenderText(renderer, valStr, x + width + 20, y, {255, 255, 255, 255});
    int slideX = x + 150; int slideW = width - 150; int slideH = 10; int slideY = y + 5;
    renderer->DrawRect2D(slideX, slideY, slideW, slideH, {50, 50, 50, 255});
    float t = (value - min) / (max - min);
    int handleX = slideX + (int)(t * slideW) - 5;
    renderer->DrawRect2D(handleX, slideY - 5, 10, 20, {200, 200, 200, 255});
    bool hovered = (mouseX >= slideX && mouseX <= slideX + slideW && mouseY >= slideY - 5 && mouseY <= slideY + 15);
    if (hovered || (mouseDown && mouseX >= slideX && mouseX <= slideX + slideW && abs(mouseY - slideY) < 30)) {
        if (mouseDown) {
            float newVal = (float)(mouseX - slideX) / (float)slideW;
            newVal = std::max(0.0f, std::min(1.0f, newVal));
            value = min + newVal * (max - min);
            return true;
        }
    }
    return false;
}

void StealthMode::RenderBalanceMenu(GLRenderer* renderer, TextRenderer* textRenderer, int width, int height) {
    if (!m_BalanceMenuOpen) return;
    int menuW = 500; int menuH = 400; int x = (width - menuW) / 2; int y = (height - menuH) / 2;
    UIHelper::DrawPanel(renderer, x, y, menuW, menuH, {20, 20, 30, 240});
    renderer->DrawRect2D(x, y, menuW, 2, {100, 200, 255, 255});
    textRenderer->RenderText(renderer, "STEALTH BALANCE", x + 20, y + 20, {100, 200, 255, 255});
    textRenderer->RenderText(renderer, "X", x + menuW - 30, y + 10, {255, 100, 100, 255});
    UIHelper::DrawPanel(renderer, x + 20, y + menuH - 40, 100, 30, {50, 150, 50, 200});
    textRenderer->RenderText(renderer, "SAVE CONFIG", x + 30, y + menuH - 32, {255, 255, 255, 255});
    int mx, my; Input::GetMousePosition(mx, my); bool mouseDown = Input::IsMouseButtonDown(1);
    int curY = y + 60; int gap = 40;
    DoSlider(renderer, textRenderer, "Detection Dist", m_BalanceSettings.maxDetectionDistance, 5.0f, 50.0f, x + 20, curY, 300, my, mx, mouseDown); curY += gap;
    DoSlider(renderer, textRenderer, "View Angle", m_BalanceSettings.viewAngle, 30.0f, 180.0f, x + 20, curY, 300, my, mx, mouseDown); curY += gap;
    DoSlider(renderer, textRenderer, "Sneak Vis Mult", m_BalanceSettings.sneakVisibilityMultiplier, 0.0f, 1.0f, x + 20, curY, 300, my, mx, mouseDown); curY += gap;
    DoSlider(renderer, textRenderer, "Noise Thresh", m_BalanceSettings.noiseSpeedThreshold, 1.0f, 10.0f, x + 20, curY, 300, my, mx, mouseDown); curY += gap;
    DoSlider(renderer, textRenderer, "Hear Dist", m_BalanceSettings.hearingDistance, 1.0f, 30.0f, x + 20, curY, 300, my, mx, mouseDown); curY += gap;
    DoSlider(renderer, textRenderer, "Grace Period", m_BalanceSettings.gracePeriod, 0.0f, 10.0f, x + 20, curY, 300, my, mx, mouseDown); curY += gap;
    DoSlider(renderer, textRenderer, "Decay Rate", m_BalanceSettings.alertDecayRate, 0.1f, 2.0f, x + 20, curY, 300, my, mx, mouseDown); curY += gap;
}

void StealthMode::HandleBalanceMenuInput() {
    if (!m_BalanceMenuOpen) return;
    if (Input::IsMouseButtonPressed(1)) {
        int width = m_ScreenWidth; int height = m_ScreenHeight;
        int menuW = 500; int menuH = 400; int x = (width - menuW) / 2; int y = (height - menuH) / 2;
        int mx, my; Input::GetMousePosition(mx, my);
        if (mx >= x + menuW - 35 && mx <= x + menuW - 5 && my >= y + 5 && my <= y + 35) {
            m_BalanceMenuOpen = false;
            if (m_Console) m_Console->SetInputEnabled(true);
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }
        int saveX = x + 20; int saveY = y + menuH - 40;
        if (mx >= saveX && mx <= saveX + 100 && my >= saveY && my <= saveY + 30) SaveBalanceConfig();
    }
}

bool StealthMode::GetSpawnLocation(SpawnType type, float& x, float& y, float& rot) {
    if (m_SpawnEditor) {
        auto spawnLocations = m_SpawnEditor->GetSpawnLocations();
        for (const auto& loc : spawnLocations) {
            if (loc.type == type) {
                x = loc.x;
                y = loc.y;
                rot = loc.rotation;
                return true;
            }
        }
    }
    return false;
}

} // namespace PixelsEngine