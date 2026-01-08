#include "DungeonMode.h"
#include "../engine/Components.h"
#include "../engine/Input.h"
#include "../engine/TextRenderer.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace PixelsEngine {

DungeonMode::DungeonMode(Registry* registry, GLRenderer* renderer)
    : m_Registry(registry), m_Renderer(renderer), m_Skybox(std::make_unique<Skybox>()) {}

DungeonMode::~DungeonMode() {}

void DungeonMode::Init(Camera* camera, Entity& playerEntity) {
    m_Camera = camera;
    m_AudioManager = std::make_unique<AudioManager>();
    m_SfxShoot = m_AudioManager->LoadWAV("assets/bow_shoot.wav");
    m_SfxHit = m_AudioManager->LoadWAV("assets/bow_hit.wav");
    m_SfxDoor = m_AudioManager->LoadWAV("assets/door.wav");
    m_SfxSwordHit = m_AudioManager->LoadWAV("assets/sword_hit.wav");
    m_SfxSwordMiss = m_AudioManager->LoadWAV("assets/sword_miss.wav");
    
    // Load ambient flute music
    m_AmbienceMusic = m_AudioManager->LoadMusic("assets/ambience_flute.mp3");
    
    // Set dungeon skybox to black
    m_Skybox->SetColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Pre-load character assets
    auto assetManager = std::make_unique<AssetManager>(m_Renderer);
    assetManager->LoadCharacter("Knight",
                                 "assets/adventurers/Characters/gltf/Knight.glb",
                                 "assets/adventurers/Textures/knight_texture.png");
    assetManager->LoadCharacter("Skeleton_Warrior",
                                 "assets/skeletons/characters/gltf/Skeleton_Warrior.glb",
                                 "assets/skeletons/texture/skeleton_texture.png");
    assetManager->LoadCharacter("Skeleton_Minion",
                                 "assets/skeletons/characters/gltf/Skeleton_Minion.glb",
                                 "assets/skeletons/texture/skeleton_texture.png");
    assetManager->LoadCharacter("Skeleton_Mage",
                                 "assets/skeletons/characters/gltf/Skeleton_Mage.glb",
                                 "assets/skeletons/texture/skeleton_texture.png");

    // Load weapon meshes
    m_Renderer->LoadMesh("Sword", "assets/adventurers/Assets/gltf/sword_1handed.gltf");
    m_Renderer->LoadMesh("Crossbow", "assets/adventurers/Assets/gltf/crossbow_1handed.gltf");
    m_Renderer->LoadMesh("Arrow", "assets/adventurers/Assets/gltf/arrow_bow.gltf");

    // Load all animation files for the player character
    m_Renderer->LoadMesh("CharacterAnimations_General", "assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_General.glb");
    m_Renderer->LoadMesh("CharacterAnimations_Movement", "assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_MovementBasic.glb");
    m_Renderer->LoadMesh("CharacterAnimations_Combat", "assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_CombatMelee.glb");
    m_Renderer->LoadMesh("CharacterAnimations_Advanced", "assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_MovementAdvanced.glb");
    m_Renderer->LoadMesh("SkeletonAnimations", "assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_Special.glb");

    // Copy animations from loaded animation files to character and skeleton meshes
    RenderMesh* knightMesh = m_Renderer->GetRenderMesh("Knight");
    if (knightMesh) {
      // Merge all animation files into the knight mesh
      RenderMesh* generalAnims = m_Renderer->GetRenderMesh("CharacterAnimations_General");
      RenderMesh* movementAnims = m_Renderer->GetRenderMesh("CharacterAnimations_Movement");
      RenderMesh* combatAnims = m_Renderer->GetRenderMesh("CharacterAnimations_Combat");
      RenderMesh* advancedAnims = m_Renderer->GetRenderMesh("CharacterAnimations_Advanced");
      
      if (generalAnims) {
        knightMesh->animations.insert(knightMesh->animations.end(), 
                                      generalAnims->animations.begin(), 
                                      generalAnims->animations.end());
      }
      if (movementAnims) {
        knightMesh->animations.insert(knightMesh->animations.end(), 
                                      movementAnims->animations.begin(), 
                                      movementAnims->animations.end());
      }
      if (combatAnims) {
        knightMesh->animations.insert(knightMesh->animations.end(), 
                                      combatAnims->animations.begin(), 
                                      combatAnims->animations.end());
      }
      if (advancedAnims) {
        knightMesh->animations.insert(knightMesh->animations.end(), 
                                      advancedAnims->animations.begin(), 
                                      advancedAnims->animations.end());
      }
    }
    
    RenderMesh* skelAnimMesh = m_Renderer->GetRenderMesh("SkeletonAnimations");
    if (skelAnimMesh) {
      RenderMesh* skelWarriorMesh = m_Renderer->GetRenderMesh("Skeleton_Warrior");
      RenderMesh* skelMinionMesh = m_Renderer->GetRenderMesh("Skeleton_Minion");
      RenderMesh* skelMageMesh = m_Renderer->GetRenderMesh("Skeleton_Mage");
      
      if (skelWarriorMesh) skelWarriorMesh->animations = skelAnimMesh->animations;
      if (skelMinionMesh) skelMinionMesh->animations = skelAnimMesh->animations;
      if (skelMageMesh) skelMageMesh->animations = skelAnimMesh->animations;
    }

    // Start with a default level list if none provided
    if (m_LevelList.empty()) {
        m_LevelList = {"my_dungeon"};
    }
    
    m_CurrentLevelIdx = 0;
    LoadLevel(m_LevelList[m_CurrentLevelIdx]);
    
    // Start ambient music on loop
    if (m_AmbienceMusic) {
        Mix_PlayMusic(m_AmbienceMusic, -1);  // -1 = infinite loop
    }
    
    playerEntity = m_PlayerEntity;
}

void DungeonMode::StartDungeon(const std::vector<std::string>& levels) {
    m_LevelList = levels;
    m_CurrentLevelIdx = 0;
    m_Active = true;
}

void DungeonMode::LoadDungeonFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Failed to open dungeon file: " << filename << std::endl;
        return;
    }

    m_LevelList.clear();
    std::string levelName;
    while (in >> levelName) {
        if (!levelName.empty()) {
            m_LevelList.push_back(levelName);
        }
    }
    in.close();

    if (!m_LevelList.empty()) {
        m_CurrentLevelIdx = 0;
        m_Active = true;
        std::cout << "Loaded dungeon with " << m_LevelList.size() << " levels." << std::endl;
    }
}

void DungeonMode::LoadLevel(const std::string& levelName) {
    // Clear registry by destroying all entities
    auto& meshView = m_Registry->View<MeshComponent>();
    std::vector<Entity> toKill;
    for (auto& pair : meshView) toKill.push_back(pair.first);
    for (auto e : toKill) m_Registry->DestroyEntity(e);
    
    // Load map
    std::string path = "assets/saves/" + levelName + ".map";
    MapLoader::LoadFromFile(path, m_Registry, m_Renderer);
    
    // Find spawn point: Priority 1: player_spawn, Priority 2: stairs
    float spawnX = 0, spawnY = 0, spawnZ = 0;
    float playerRot = 0;
    bool foundSpawn = false;
    Entity spawnMarker = PixelsEngine::INVALID_ENTITY;
    
    // Re-fetch mesh view after load
    auto& meshes = m_Registry->View<MeshComponent>();
    
    // 1. Check for explicit player_spawn
    for (auto& pair : meshes) {
        std::cout << "DEBUG: Checking mesh: " << pair.second.meshName << std::endl;
        if (pair.second.meshName == "player_spawn") {
            auto* t = m_Registry->GetComponent<Transform3DComponent>(pair.first);
            if (t) {
                spawnX = t->x;
                spawnY = t->y;
                spawnZ = t->z;
                playerRot = t->rot;
                foundSpawn = true;
                spawnMarker = pair.first;
                std::cout << "DEBUG: Found player_spawn at " << spawnX << ", " << spawnY << std::endl;
                break;
            }
        }
    }

    // 2. Fallback to stairs
    if (!foundSpawn) {
        for (auto& pair : meshes) {
            if (pair.second.meshName.find("stairs") != std::string::npos) {
                auto* t = m_Registry->GetComponent<Transform3DComponent>(pair.first);
                if (t) {
                    float spawnOffset = 4.0f;
                    spawnX = t->x + cos(t->rot) * spawnOffset;
                    spawnY = t->y + sin(t->rot) * spawnOffset;
                    spawnZ = t->z; 
                    playerRot = t->rot + M_PI; // Face the stairs
                    foundSpawn = true;
                    break;
                }
            }
        }
    }
    
    if (!foundSpawn) {
        spawnX = 12.0f; spawnY = 0.0f; spawnZ = 0.0f;
    }

    // Store spawn coordinates for respawn
    m_SpawnX = spawnX;
    m_SpawnY = spawnY;
    m_SpawnZ = spawnZ;

    // Remove the spawn marker if it exists
    if (spawnMarker != PixelsEngine::INVALID_ENTITY) {
        m_Registry->DestroyEntity(spawnMarker);
    }
    
    m_PlayerEntity = CharacterFactory::CreatePlayer(m_Registry, m_Renderer, spawnX, spawnY, spawnZ);
    auto* pt = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    if (pt) pt->rot = playerRot;
    
    // Setup Controllers: Top-down view and world-space movement
    m_CameraController = std::make_unique<TopDownCamera>(m_Camera, m_Registry, m_PlayerEntity);
    m_CameraController->SetHeight(12.0f);
    m_CameraController->SetDistance(8.0f);
    
    m_MovementController = std::make_unique<TopDownMovementController>(m_Registry, m_PlayerEntity);
    m_MovementController->SetAcceleration(80.0f);   // Moderate acceleration
    m_MovementController->SetFriction(0.92f);       // High friction for immediate stop
    m_MovementController->SetMaxSpeed(8.0f);        // Slower for dungeon feel
    m_MovementController->SetMap(m_Map);            // Set map for wall collision detection
    
    // Spawn skeleton enemies at skeleton spawn points
    auto skeletonView = m_Registry->View<MeshComponent>();
    std::vector<Entity> skeletonsToReplace;
    for (auto& pair : skeletonView) {
      auto* mesh = m_Registry->GetComponent<MeshComponent>(pair.first);
      if (mesh && (mesh->meshName.find("Skeleton") != std::string::npos)) {
        skeletonsToReplace.push_back(pair.first);
      }
    }
    
    // Replace skeleton meshes with actual enemy characters using CharacterFactory
    for (auto skeletonEntity : skeletonsToReplace) {
      auto* t = m_Registry->GetComponent<Transform3DComponent>(skeletonEntity);
      if (t) {
        // Use CharacterFactory to create properly configured skeleton
        auto skeleton = CharacterFactory::CreateSkeleton(m_Registry, m_Renderer, "Skeleton_Warrior", t->x, t->y, t->z);
        
        // Add weapon attachment (sword for warrior)
        AttachmentComponent attach;
        attach.meshName = "Sword";
        attach.textureName = "skeleton_texture";
        attach.boneName = "hand.r";
        attach.rotX = -7.63f;
        attach.rotY = 18.148f;
        attach.rotZ = 1.702f;
        attach.scale = 1.0f;
        m_Registry->AddComponent<AttachmentComponent>(skeleton, attach);
        
        // Add collider for physics interactions
        m_Registry->AddComponent<ColliderComponent>(skeleton, {0.5f, 1.8f, true});
      }
      m_Registry->DestroyEntity(skeletonEntity);
    }
    
    m_DoorUnlocked = false;
    m_LevelComplete = false;
    m_FirstFrame = true;
    m_StatusMessage = "FLOOR " + std::to_string(m_CurrentLevelIdx + 1) + ": CLEAR ALL ENEMIES";
    m_MessageTimer = 3.0f;
}

void DungeonMode::Update(float dt, Entity& playerEntity) {
    if (!m_Active) return;

    // Sync input parameter with current internal state (crucial for transitions)
    playerEntity = m_PlayerEntity;

    if (m_MessageTimer > 0) m_MessageTimer -= dt;
    if (m_AttackTimer > 0) m_AttackTimer -= dt;

    // Update Controllers
    if (m_CameraController) m_CameraController->HandleInput(dt);
    if (m_CameraController) m_CameraController->Update(dt);
    if (m_MovementController) m_MovementController->HandleInput(dt);

    auto* t = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    auto* phys = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
    auto* anim = m_Registry->GetComponent<SkeletalAnimationComponent>(m_PlayerEntity);

    if (t && m_CameraController) {
        if (m_FirstFrame) {
            m_FirstFrame = false;
        }
    }

    // 1. Weapon Swapping & Jumping & Sneaking
    if (Input::IsKeyPressed(SDL_SCANCODE_1)) m_CurrentWeapon = 2; // Sword
    if (Input::IsKeyPressed(SDL_SCANCODE_2)) m_CurrentWeapon = 3; // Bow
    m_IsSneaking = Input::IsKeyDown(SDL_SCANCODE_LSHIFT) || Input::IsKeyDown(SDL_SCANCODE_RSHIFT);
    
    if (Input::IsKeyPressed(SDL_SCANCODE_SPACE) && phys && phys->isGrounded) {
        phys->velZ = 8.0f; // Jump force (approx 1 tile height)
        phys->isGrounded = false;
    }

    // 2. Combat handling
    HandleCombat(dt);

    // 3. Player Animation Management
    if (anim && m_PlayerEntity != PixelsEngine::INVALID_ENTITY) {
        RenderMesh* rm = m_Renderer->GetRenderMesh("Knight");
        if (rm) {
            auto* unit = m_Registry->GetComponent<BattleUnitComponent>(m_PlayerEntity);
            std::string targetAnim = "Idle_A";
            
            // Check if movement keys are being pressed
            bool isMoving = Input::IsKeyDown(SDL_SCANCODE_W) || Input::IsKeyDown(SDL_SCANCODE_A) ||
                           Input::IsKeyDown(SDL_SCANCODE_S) || Input::IsKeyDown(SDL_SCANCODE_D);
            
            // Death animation takes priority
            if (unit && unit->hp <= 0) {
                targetAnim = "Death_A";
                anim->loop = false;  // Death animation should not loop
            } else if (m_AttackTimer > 0) {
                // Attack animation takes priority
                if (m_CurrentWeapon == 3) {
                    // Crossbow/ranged attack
                    targetAnim = "Ranged_Bow_Shoot";
                } else {
                    // Melee attack
                    targetAnim = "Melee_1H_Attack_Chop";
                }
                anim->loop = false;  // Attack animations should not loop
            } else if (!phys || !phys->isGrounded) {
                // Jumping or falling
                targetAnim = "Jump_Full_Long";
                anim->loop = false;
            } else if (m_IsSneaking) {
                // Sneaking animation
                targetAnim = "Sneaking";
                anim->loop = true;
            } else if (isMoving) {
                // Walking animation - use input state instead of velocity
                targetAnim = "Walking_A";
                anim->loop = true;
            } else {
                // Idle animation
                targetAnim = "Idle_A";
                anim->loop = true;
            }

            int bestIdx = -1;
            // First try exact match
            for (size_t i = 0; i < rm->animations.size(); i++) {
                if (rm->animations[i].name == targetAnim) {
                    bestIdx = (int)i;
                    break;
                }
            }
            // If no exact match, try substring match
            if (bestIdx == -1) {
                for (size_t i = 0; i < rm->animations.size(); i++) {
                    if (rm->animations[i].name.find(targetAnim) != std::string::npos) {
                        bestIdx = (int)i;
                        break;
                    }
                }
            }
            if (bestIdx != -1 && anim->animationIndex != bestIdx) {
                anim->animationIndex = bestIdx;
                anim->currentTime = 0.0f;
            }
        }
    }

    // 4. Enemy Animation Management
    auto& enemiesView = m_Registry->View<EnemyComponent>();
    for (auto& pair : enemiesView) {
        auto* eAnim = m_Registry->GetComponent<SkeletalAnimationComponent>(pair.first);
        auto* eMesh = m_Registry->GetComponent<MeshComponent>(pair.first);
        auto* eUnit = m_Registry->GetComponent<BattleUnitComponent>(pair.first);
        if (eAnim && eMesh) {
            RenderMesh* rm = m_Renderer->GetRenderMesh(eMesh->meshName);
            if (rm) {
                std::string targetAnim = "Idle";
                
                // Death animation for skeletons
                if (eUnit && eUnit->hp <= 0) {
                    if (eMesh->meshName.find("Skeleton") != std::string::npos) {
                        targetAnim = "Skeletons_Death";
                    } else {
                        targetAnim = "Death_A";
                    }
                } else if (eMesh->meshName.find("Skeleton") != std::string::npos) {
                    targetAnim = "Skeletons_Idle";
                } else {
                    targetAnim = "Idle";
                }
                
                int bestIdx = -1;
                // First try exact match
                for (size_t i = 0; i < rm->animations.size(); i++) {
                    if (rm->animations[i].name == targetAnim) {
                        bestIdx = (int)i;
                        break;
                    }
                }
                // If no exact match, try substring match
                if (bestIdx == -1) {
                    for (size_t i = 0; i < rm->animations.size(); i++) {
                        if (rm->animations[i].name.find(targetAnim) != std::string::npos) {
                            bestIdx = (int)i;
                            break;
                        }
                    }
                }
                if (bestIdx != -1 && eAnim->animationIndex != bestIdx) {
                    eAnim->animationIndex = bestIdx;
                    eAnim->currentTime = 0.0f;
                    // Non-death animations should loop
                    if (eUnit && eUnit->hp > 0) {
                        eAnim->loop = true;
                    }
                }
            }
        }
    }

    // 5. Update Flash Effects (decrease over time)
    auto& allUnits = m_Registry->View<BattleUnitComponent>();
    for (auto& pair : allUnits) {
        if (pair.second.flashAmount > 0.0f) {
            pair.second.flashAmount -= dt * 5.0f;
            if (pair.second.flashAmount < 0.0f) {
                pair.second.flashAmount = 0.0f;
            }
        }
    }

    // 6. Update Weapon Visuals
    auto* attach = m_Registry->GetComponent<AttachmentComponent>(m_PlayerEntity);
    if (attach) {
        if (m_CurrentWeapon == 3) {
            attach->meshName = "Crossbow";
            attach->rotX = -16.896f; attach->rotY = 30.052f; attach->rotZ = 11.404f;
        } else {
            attach->meshName = "Sword";
            attach->rotX = -7.63f; attach->rotY = 18.148f; attach->rotZ = 1.702f;
        }
    }

    // 7. Check for player death and respawn
    auto* playerUnit = m_Registry->GetComponent<BattleUnitComponent>(m_PlayerEntity);
    if (playerUnit && playerUnit->hp <= 0) {
        // Respawn player at spawn point (using stored coordinates)
        auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
        auto* playerPhys = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
        if (playerTransform && playerPhys) {
            // Set position to stored spawn point
            playerTransform->x = m_SpawnX;
            playerTransform->y = m_SpawnY;
            playerTransform->z = m_SpawnZ;
            playerTransform->rot = 0.0f;
            playerTransform->pitch = 0.0f;
            
            // Reset physics
            playerPhys->velX = 0.0f;
            playerPhys->velY = 0.0f;
            playerPhys->velZ = 0.0f;
            playerPhys->isGrounded = true;
            
            // Restore health
            playerUnit->hp = playerUnit->maxHp;
            playerUnit->flashAmount = 0.0f;
            
            // Reset animation
            auto* playerAnim = m_Registry->GetComponent<SkeletalAnimationComponent>(m_PlayerEntity);
            if (playerAnim) {
                playerAnim->animationIndex = 0;
                playerAnim->currentTime = 0.0f;
                playerAnim->loop = true;
            }
        }
    }

    CheckLevelCompletion();

    // Check for exit interaction
    if (m_DoorUnlocked && t) {
        auto& meshView = m_Registry->View<MeshComponent>();
        for (auto& pair : meshView) {
            if (pair.second.meshName.find("door") != std::string::npos) {
                auto* doorT = m_Registry->GetComponent<Transform3DComponent>(pair.first);
                if (doorT && t) {
                    float dx = t->x - doorT->x;
                    float dy = t->y - doorT->y;
                    float dist = sqrt(dx * dx + dy * dy);
                    if (dist < 2.5f) {
                        NextLevel();
                        break;
                    }
                }
            }
        }
    }
}

void DungeonMode::HandleCombat(float dt) {
    auto* t = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    auto* anim = m_Registry->GetComponent<SkeletalAnimationComponent>(m_PlayerEntity);
    if (!t || m_AttackTimer > 0) return;

    // Shove (F)
    if (Input::IsKeyPressed(SDL_SCANCODE_F)) {
        m_AttackTimer = 0.5f;
        float shoveRange = 2.0f;      // Reduced from 2.5f
        float shovePower = 5.0f;      // Reduced from 15.0f for controlled push
        
        // Trigger Player Push Animation (Interact)
        if (anim) {
            RenderMesh* rm = m_Renderer->GetRenderMesh("Knight");
            if (rm) {
                int pushIdx = -1;
                for (size_t i = 0; i < rm->animations.size(); i++) {
                    if (rm->animations[i].name == "Interact") {
                        pushIdx = (int)i;
                        break;
                    }
                }
                if (pushIdx != -1) {
                    anim->animationIndex = pushIdx;
                    anim->currentTime = 0.0f;
                }
            }
        }

        auto& enemies = m_Registry->View<EnemyComponent>();
        for (auto& pair : enemies) {
            auto* et = m_Registry->GetComponent<Transform3DComponent>(pair.first);
            auto* ephys = m_Registry->GetComponent<PhysicsComponent>(pair.first);
            if (et && ephys) {
                float dx = et->x - t->x;
                float dy = et->y - t->y;
                float dist = sqrt(dx * dx + dy * dy);
                if (dist < shoveRange) {
                    float angle = atan2(dy, dx);
                    ephys->velX = cos(angle) * shovePower;
                    ephys->velY = sin(angle) * shovePower;
                    ephys->isGrounded = false; // Allow falling if pushed off edge
                    
                    auto* eu = m_Registry->GetComponent<BattleUnitComponent>(pair.first);
                    if (eu) eu->flashAmount = 1.0f;
                    if (m_SfxSwordHit) Mix_PlayChannel(-1, m_SfxSwordHit, 0);
                }
            }
        }
    }

    // Attack (Left Click)
    if (Input::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
        if (m_CurrentWeapon == 3) { // Bow
            m_AttackTimer = 0.8f;
            if (m_SfxShoot) Mix_PlayChannel(-1, m_SfxShoot, 0);

            auto arrow = m_Registry->CreateEntity();
            // In top-down, model faces atan2(dx, dy) + PI
            // But projectile needs its own forward vector
            float forwardX = -sin(t->rot);
            float forwardY = -cos(t->rot);
            m_Registry->AddComponent<Transform3DComponent>(arrow, {t->x + forwardX * 0.5f, t->y + forwardY * 0.5f, t->z + 1.2f, t->rot, 0});
            m_Registry->AddComponent<MeshComponent>(arrow, {"Arrow", "Knight_tex", 1, 1, 1});
            m_Registry->AddComponent<ProjectileComponent>(arrow, {ProjectileComponent::Arrow, 25.0f, true, 5.0f});
            float projSpeed = 40.0f;
            m_Registry->AddComponent<PhysicsComponent>(arrow, {forwardX * projSpeed, forwardY * projSpeed, 0.0f, 0.0f, false, false, 0.0f, 0.0f});
        } else { // Sword
            m_AttackTimer = 0.5f;
            bool hit = false;
            auto& enemies = m_Registry->View<EnemyComponent>();
            for (auto& pair : enemies) {
                auto* et = m_Registry->GetComponent<Transform3DComponent>(pair.first);
                auto* eu = m_Registry->GetComponent<BattleUnitComponent>(pair.first);
                if (et && eu && eu->hp > 0) {
                    float dx = et->x - t->x;
                    float dy = et->y - t->y;
                    if (sqrt(dx * dx + dy * dy) < 2.5f) {
                        eu->hp -= 15;
                        eu->flashAmount = 1.0f;
                        hit = true;
                    }
                }
            }
            if (hit && m_SfxSwordHit) Mix_PlayChannel(-1, m_SfxSwordHit, 0);
            else if (m_SfxSwordMiss) Mix_PlayChannel(-1, m_SfxSwordMiss, 0);
        }
    }
}

void DungeonMode::CheckLevelCompletion() {
    auto enemies = m_Registry->View<EnemyComponent>();
    int count = 0;
    for (auto& pair : enemies) {
        auto* unit = m_Registry->GetComponent<BattleUnitComponent>(pair.first);
        if (unit && unit->hp > 0) count++;
    }
    
    if (count == 0 && !m_DoorUnlocked) {
        m_DoorUnlocked = true;
        m_StatusMessage = "DUNGEON DOOR UNLOCKED!";
        m_MessageTimer = 3.0f;
        if (m_SfxDoor) Mix_PlayChannel(-1, m_SfxDoor, 0);
    }
}

void DungeonMode::NextLevel() {
    m_CurrentLevelIdx++;
    if (m_CurrentLevelIdx < m_LevelList.size()) {
        LoadLevel(m_LevelList[m_CurrentLevelIdx]);
    } else {
        m_Active = false;
        m_StatusMessage = "DUNGEON CLEARED!";
        m_MessageTimer = 5.0f;
    }
}

void DungeonMode::RenderUI(GLRenderer* ren, TextRenderer* tr, int w, int h) {
    if (m_MessageTimer > 0) {
        tr->RenderText(ren, m_StatusMessage, w/2 - 100, 100, {255, 255, 100, 255});
    }
    
    tr->RenderText(ren, "Floor: " + std::to_string(m_CurrentLevelIdx + 1), 20, h - 30, {255, 255, 255, 255});
}

} // namespace PixelsEngine
