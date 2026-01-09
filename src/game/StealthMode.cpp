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

namespace PixelsEngine {

StealthMode::StealthMode(Registry* registry, GLRenderer* renderer)
    : m_Registry(registry), m_Renderer(renderer) {
}

StealthMode::~StealthMode() = default;

void StealthMode::Init(Camera* camera, Entity& playerEntity) {
    m_Active = true;
    m_Camera = camera;
    
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
    
    // Load animation assets for knight directly via renderer
    m_Renderer->LoadMesh("CharacterAnimations",
                        "assets/adventurers/Animations/gltf/Rig_Medium/Rig_Medium_General.glb");
    m_Renderer->LoadMesh("KnightMovement",
                        "assets/adventurers/Animations/gltf/Rig_Medium/Rig_Medium_MovementBasic.glb");
    
    LoadLevel("assets/saves/my_dungeon.map");
    
    // Initialize spawn editor and load spawn config BEFORE spawning enemies
    m_SpawnEditor = std::make_unique<StealthSpawnEditor>();
    m_SpawnEditor->LoadFromFile("assets/saves/spawn_config.txt");
    
    // Spawn player using CharacterFactory at a valid floor location in the dungeon
    // Moved back from box_stacked obstacle to avoid collision issues
    m_PlayerEntity = CharacterFactory::CreatePlayer(m_Registry, m_Renderer, 36.0f, -20.0f, 0.5f);
    playerEntity = m_PlayerEntity;
    
    // Spawn enemies in the level (uses loaded spawn config)
    SpawnEnemies();
    
    // Create camera controller AFTER spawning the player
    m_CameraController = std::make_unique<ThirdPersonCamera>(
        m_Camera, m_Registry, m_PlayerEntity);
    m_CameraController->SetDistance(3.0f);      // Distance behind player
    m_CameraController->SetShoulder(0.0f);      // Center behind, not on shoulder
    m_CameraController->SetHeight(1.6f);        // Eye level height
    m_CameraController->SetYaw(-M_PI / 2);      // Look from behind
    
    // Create movement controller for player input
    m_MovementController = std::make_unique<ThirdPersonMovementController>(m_Registry, m_PlayerEntity);
    
    // Setup animations for knight
    RenderMesh* charAnimMesh = m_Renderer->GetRenderMesh("CharacterAnimations");
    RenderMesh* knightMoveMesh = m_Renderer->GetRenderMesh("KnightMovement");
    RenderMesh* knightMesh = m_Renderer->GetRenderMesh("Knight");
    
    if (charAnimMesh && knightMesh) {
        knightMesh->animations = charAnimMesh->animations;
    }
    if (knightMoveMesh && knightMesh) {
        // Merge movement animations into knight mesh
        for (const auto& anim : knightMoveMesh->animations) {
            knightMesh->animations.push_back(anim);
        }
    }
    
    // Initialize player animation to Idle
    auto* playerAnim = m_Registry->GetComponent<SkeletalAnimationComponent>(m_PlayerEntity);
    if (playerAnim && knightMesh) {
        int idleIdx = -1;
        for (size_t i = 0; i < knightMesh->animations.size(); ++i) {
            if (knightMesh->animations[i].name == "Idle_A") {
                idleIdx = i;
                break;
            }
        }
        if (idleIdx >= 0) {
            playerAnim->animationIndex = idleIdx;
            playerAnim->currentTime = 0.0f;
        }
    }
    
    // Initialize console and visual spawn editor
    m_Console = std::make_unique<Console>();
    m_VisualSpawnEditor = std::make_unique<VisualSpawnEditor>(m_Registry, m_Renderer);
    
    // Load spawn locations into visual editor
    auto spawnLocations = m_SpawnEditor->GetSpawnLocations();
    m_VisualSpawnEditor->SetSpawnLocations(spawnLocations);
    // Editor is closed by default, open with /edit command
    
    // Register console commands
    m_Console->RegisterCommand("/edit", [this](const std::vector<std::string>& args) {
        bool wasActive = m_VisualSpawnEditor->IsActive();
        m_VisualSpawnEditor->Activate(!wasActive);
        if (!wasActive) {
            // Save console visibility state before closing
            m_ConsoleWasOpen = m_Console->IsOpen();
            m_Console->Close();  // Hide console when entering editor
            
            // Disable console input but allow ~ to toggle it
            m_Console->SetInputEnabled(false);
            SDL_SetRelativeMouseMode(SDL_TRUE);
            m_Console->AddLog("Visual spawn editor activated.");
            m_Console->AddLog("Mouse look to rotate camera | WASD to move");
            m_Console->AddLog("SPACE/SHIFT for up/down | Click+Drag spawn");
        } else {
            // Re-enable console input and disable relative mouse mode
            m_Console->SetInputEnabled(true);
            SDL_SetRelativeMouseMode(SDL_FALSE);
            
            // Restore console visibility state
            if (m_ConsoleWasOpen) {
                m_Console->Open();
            }
            
            // Save and close
            auto locs = m_VisualSpawnEditor->GetSpawnLocations();
            m_SpawnEditor->SetSpawnLocations(locs);
            m_SpawnEditor->SaveToFile("assets/saves/spawn_config.txt");
            m_Console->AddLog("Visual spawn editor closed. Changes saved.");
        }
    });
    
    m_Console->RegisterCommand("/spawn", [this](const std::vector<std::string>& args) {
        if (args.size() >= 3) {
            try {
                float x = std::stof(args[1]);
                float y = std::stof(args[2]);
                m_SpawnEditor->AddSpawn(x, y);
                m_Console->AddLog("Spawn added at (" + std::to_string((int)x) + ", " + std::to_string((int)y) + ")");
            } catch (...) {
                m_Console->AddLog("Usage: /spawn <x> <y>");
            }
        } else {
            m_Console->AddLog("Usage: /spawn <x> <y>");
        }
    });
    
    m_Console->RegisterCommand("/save", [this](const std::vector<std::string>& args) {
        // If in editor mode, sync visual editor positions to spawn editor
        if (m_VisualSpawnEditor->IsActive()) {
            auto locs = m_VisualSpawnEditor->GetSpawnLocations();
            m_SpawnEditor->SetSpawnLocations(locs);
        }
        m_SpawnEditor->SaveToFile("assets/saves/spawn_config.txt");
        m_Console->AddLog("Spawns saved to spawn_config.txt");
    });
    
    m_Console->RegisterCommand("/load", [this](const std::vector<std::string>& args) {
        m_SpawnEditor->LoadFromFile("assets/saves/spawn_config.txt");
        m_Console->AddLog("Spawns loaded from spawn_config.txt");
    });
    
    m_Console->RegisterCommand("/delete_all", [this](const std::vector<std::string>& args) {
        if (m_VisualSpawnEditor->IsActive()) {
            // Clear from visual editor using ClearAllSpawns
            m_VisualSpawnEditor->ClearAllSpawns();
            m_Console->AddLog("All spawned skeletons deleted from editor.");
        } else {
            // Clear from main spawn editor
            m_SpawnEditor->SetSpawnLocations({});
            m_Console->AddLog("All spawned skeletons deleted.");
        }
    });
    
    m_Console->RegisterCommand("/help", [this](const std::vector<std::string>& args) {
        m_Console->AddLog("Commands: /edit /spawn <x> <y> /save /load /delete_all /help");
    });
}

void StealthMode::LoadLevel(const std::string& mapFile) {
    // Clear existing guards and hide spots
    m_Guards.clear();
    m_GuardStates.clear();
    m_GuardAlertLevels.clear();
    m_HideSpots.clear();
    
    // Load entities from map file
    MapLoader::LoadFromFile(mapFile, m_Registry, m_Renderer);
    
    std::cout << "Loaded stealth map: " << mapFile << std::endl;
    
    // Scan for guards and hide spots by searching all entities
    // Since we don't have direct access to loaded entities, we'll need to scan by component
    // For now, we'll set up basic detection - guards will be identified by name conventions
}

void StealthMode::Update(float dt, Entity& playerEntity) {
    if (!m_Active || !m_Camera)
        return;
    
    // If detected, disable normal input and wait for ESC to exit
    if (m_Detected && Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_Active = false;
        return;
    }
    
    if (m_Detected) {
        // Game over - disable game input
        return;
    }
    
    // Update console and spawn editor
    if (m_Console) {
        // Handle console toggle with ~ even during editor mode
        if (Input::IsKeyPressed(SDL_SCANCODE_GRAVE)) {
            if (m_VisualSpawnEditor->IsActive()) {
                // Allow toggling console during editor mode
                m_Console->Toggle();
                if (m_Console->IsOpen()) {
                    // Enable input when opening console in editor
                    m_Console->SetInputEnabled(true);
                    Input::StartTextInput();
                } else {
                    // Disable input when closing console in editor
                    m_Console->SetInputEnabled(false);
                    Input::StopTextInput();
                }
            } else {
                // Normal console toggle
                m_Console->Update();
            }
        } else if (!m_VisualSpawnEditor->IsActive()) {
            m_Console->Update();
        } else if (m_Console->IsOpen()) {
            // In editor mode with console open: process console input
            m_Console->ProcessInput();
        }
    }
    if (m_SpawnEditor) m_SpawnEditor->Update();
    
    static int frameCount = 0;
    frameCount++;
    if (frameCount % 30 == 0) {
        std::cout << "DEBUG StealthMode::Update - VisualSpawnEditor active: " << (m_VisualSpawnEditor ? m_VisualSpawnEditor->IsActive() : false) << std::endl;
    }
    
    // If console is open, don't process game or editor input
    bool consoleOpen = m_Console && m_Console->IsOpen();
    if (consoleOpen) {
        return;
    }
    
    if (m_VisualSpawnEditor) m_VisualSpawnEditor->Update(dt, m_Camera, m_ScreenWidth, m_ScreenHeight);
    
    // If visual spawn editor is active, skip normal game updates (camera/player control)
    if (m_VisualSpawnEditor && m_VisualSpawnEditor->IsActive()) {
        return;
    }

    // Update movement controller
    if (m_MovementController) {
        m_MovementController->HandleInput(dt);
    }

    auto* playerPhys = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
    auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    
    if (!playerPhys || !playerTransform)
        return;
    
    // Update camera controller
    if (m_CameraController) {
        m_CameraController->HandleInput(dt);
        m_CameraController->Update(dt);  // Update camera position to follow player
    }

    // Debug: Rotate camera 90 degrees with R key
    if (Input::IsKeyPressed(SDL_SCANCODE_R)) {
        if (m_CameraController) {
            float currentYaw = m_CameraController->GetCameraYaw();
            float newYaw = currentYaw + (M_PI / 2.0f);  // Add 90 degrees
            m_CameraController->SetYaw(newYaw);
            
            // Debug print current rotation
            std::cout << "DEBUG: Camera rotated 90 degrees. New Yaw: " << newYaw 
                      << " radians (" << (newYaw * 180.0f / M_PI) << " degrees)" << std::endl;
        }
    }
    
    // Toggle sneak mode with Shift key
    if (Input::IsKeyPressed(SDL_SCANCODE_LSHIFT)) {
        m_PlayerSneaking = !m_PlayerSneaking;
        std::cout << "DEBUG: Sneak mode " << (m_PlayerSneaking ? "ENABLED" : "DISABLED") << std::endl;
    }
    
    // Update guards
    UpdateGuardAI(dt);
    
    // Check if player is detected
    CheckPlayerDetection();
    
    // Get current movement speed
    float speed = sqrt(playerPhys->velX * playerPhys->velX + playerPhys->velY * playerPhys->velY);
    
    // Decrease alert level over time if not moving
    if (speed < 0.2f && m_PlayerAlertLevel > 0.0f) {
        m_PlayerAlertLevel = std::max(0.0f, m_PlayerAlertLevel - m_AlertDecayRate * dt);
    }
    
    // Update animation based on player state
    UpdatePlayerAnimation(speed, playerPhys->isGrounded);
    
    // Update player rotation to face camera direction
    if (m_CameraController) {
        float player_rotaion_offset = M_PI + M_PI / 2.0f; // Adjust based on model orientation
        playerTransform->rot = -m_CameraController->GetCameraYaw() + player_rotaion_offset;
    }
    
    // Update camera to follow player (handled by m_CameraController->HandleInput)
    // The camera controller manages camera position and rotation automatically
}

void StealthMode::UpdatePlayerAnimation(float speed, bool isGrounded) {
    auto* playerAnim = m_Registry->GetComponent<SkeletalAnimationComponent>(m_PlayerEntity);
    RenderMesh* knightMesh = m_Renderer->GetRenderMesh("Knight");
    
    if (!playerAnim || !knightMesh) return;
    
    std::string targetAnim = "Idle_A";
    
    // Determine animation based on state
    if (!isGrounded && speed > 0.5f) {
        targetAnim = "Jump_Full_Long";
    } else if (speed > 1.0f) {
        // Moving - use walk or run animation
        // Check if player is crouching (moving slowly for stealth)
        if (speed < 3.0f && m_PlayerAlertLevel < 0.3f) {
            // Sneak/slow walk - use slower walk animation
            targetAnim = "Walking_A";
        } else {
            // Normal/fast movement
            targetAnim = "Running_A";
        }
    } else if (speed > 0.2f) {
        targetAnim = "Walking_A";
    } else {
        targetAnim = "Idle_A";
    }
    
    // Find animation index
    int targetIdx = -1;
    for (size_t i = 0; i < knightMesh->animations.size(); ++i) {
        if (knightMesh->animations[i].name == targetAnim) {
            targetIdx = i;
            break;
        }
    }
    
    // Switch animation if needed
    if (targetIdx >= 0 && (int)playerAnim->animationIndex != targetIdx) {
        playerAnim->animationIndex = targetIdx;
        playerAnim->currentTime = 0.0f;
    }
}

void StealthMode::UpdateGuardAI(float dt) {
    for (size_t i = 0; i < m_Guards.size(); ++i) {
        auto* guardTransform = m_Registry->GetComponent<Transform3DComponent>(m_Guards[i]);
        auto* guardPhys = m_Registry->GetComponent<PhysicsComponent>(m_Guards[i]);
        auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
        
        if (!guardTransform || !playerTransform)
            continue;
        
        // Simple guard patrol behavior
        float distToPlayer = sqrt(pow(guardTransform->x - playerTransform->x, 2) + 
                                pow(guardTransform->y - playerTransform->y, 2));
        
        if (m_PlayerAlertLevel > 0.5f) {
            m_GuardStates[i] = GuardState::ALERT;
            
            // Guard moves toward player
            float dirX = playerTransform->x - guardTransform->x;
            float dirY = playerTransform->y - guardTransform->y;
            float len = sqrt(dirX * dirX + dirY * dirY);
            if (len > 0) {
                dirX /= len;
                dirY /= len;
                
                if (guardPhys) {
                    guardPhys->velX = dirX * 6.0f;
                    guardPhys->velY = dirY * 6.0f;
                }
            }
        } else {
            m_GuardStates[i] = GuardState::IDLE;
            if (guardPhys) {
                guardPhys->velX = 0;
                guardPhys->velY = 0;
            }
        }
    }
}

void StealthMode::CheckPlayerDetection() {
    auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    if (!playerTransform)
        return;
    
    // Check if player is in a hide spot
    if (IsPlayerInHideSpot()) {
        m_PlayerAlertLevel = std::max(0.0f, m_PlayerAlertLevel - 0.05f);
        return;
    }
    
    // Check visibility by each guard
    for (size_t i = 0; i < m_Guards.size(); ++i) {
        bool canSee = false;
        CheckLineOfSight(m_Guards[i], m_PlayerEntity, canSee);
        
        if (canSee) {
            // Apply sneak multiplier - reduces detection increase when sneaking
            float detectionIncrease = 0.3f;
            if (m_PlayerSneaking) {
                detectionIncrease *= m_SneakVisibilityMultiplier;
            }
            m_PlayerAlertLevel = std::min(1.0f, m_PlayerAlertLevel + detectionIncrease);
            m_GuardStates[i] = GuardState::ALERT;
        }
    }
    
    m_Detected = m_PlayerAlertLevel > 0.7f;
}

// Helper function for ray-AABB intersection using parametric approach
// Returns true if collision detected, sets outDistance to the distance from start to collision point
bool RayAABBIntersection(float startX, float startY, float endX, float endY,
                         float minX, float maxX, float minY, float maxY,
                         float& outDistance) {
    float dirX = endX - startX;
    float dirY = endY - startY;
    float rayLength = sqrt(dirX * dirX + dirY * dirY);
    
    if (rayLength < 0.001f) return false;  // Ray too short
    
    // Normalize direction
    float dirNormX = dirX / rayLength;
    float dirNormY = dirY / rayLength;
    
    float tMin = 0.0f;
    float tMax = rayLength;
    
    // Check X axis
    if (fabs(dirNormX) > 0.001f) {
        float t1 = (minX - startX) / dirNormX;
        float t2 = (maxX - startX) / dirNormX;
        if (t1 > t2) std::swap(t1, t2);
        tMin = std::max(tMin, t1);
        tMax = std::min(tMax, t2);
    } else {
        // Ray is parallel to X axis, check if it's within bounds
        if (startX < minX || startX > maxX) return false;
    }
    
    // Check Y axis
    if (fabs(dirNormY) > 0.001f) {
        float t1 = (minY - startY) / dirNormY;
        float t2 = (maxY - startY) / dirNormY;
        if (t1 > t2) std::swap(t1, t2);
        tMin = std::max(tMin, t1);
        tMax = std::min(tMax, t2);
    } else {
        // Ray is parallel to Y axis, check if it's within bounds
        if (startY < minY || startY > maxY) return false;
    }
    
    // Check if there's a valid intersection
    if (tMin > tMax) return false;
    if (tMax < 0.001f) return false;  // Collision is behind the ray start
    
    // Use the entry point, ensure it's not behind start
    float collisionT = std::max(0.0f, tMin);
    outDistance = collisionT;
    
    return true;
}

bool StealthMode::RayHitsCollider(float startX, float startY, float endX, float endY, float& outDistance) {
    outDistance = std::numeric_limits<float>::max();
    
    // Iterate through all entities with hitboxes
    auto& hitboxes = m_Registry->View<HitboxComponent>();
    
    static int callCount = 0;
    static bool firstPrint = true;
    callCount++;
    
    bool foundCollision = false;
    for (auto& pair : hitboxes) {
        Entity entity = pair.first;
        HitboxComponent& hitbox = pair.second;
        
        // Only check solid objects
        if (!hitbox.isSolid) continue;
        
        // Skip guards (don't collide with other guards)
        bool isGuard = false;
        for (const auto& guard : m_Guards) {
            if (guard == entity) {
                isGuard = true;
                break;
            }
        }
        if (isGuard) continue;
        
        // Skip the player entity
        if (entity == m_PlayerEntity) continue;
        
        // Get the entity's transform
        auto* transform = m_Registry->GetComponent<Transform3DComponent>(entity);
        if (!transform) continue;
        
        // Get AABB bounds
        float minX, maxX, minY, maxY, minZ, maxZ;
        hitbox.GetBounds(transform->x, transform->y, transform->z, transform->rot,
                        minX, maxX, minY, maxY, minZ, maxZ);
        
        // Check if ray intersects this AABB
        float distance = 0.0f;
        if (RayAABBIntersection(startX, startY, endX, endY, minX, maxX, minY, maxY, distance)) {
            foundCollision = true;
            if (firstPrint && callCount < 10) {
                std::cout << "FIRST COLLISION FOUND: dist=" << distance << " ray_len=" << sqrt((endX-startX)*(endX-startX) + (endY-startY)*(endY-startY)) << std::endl;
                firstPrint = false;
            }
            if (distance < outDistance) {
                outDistance = distance;
            }
        }
    }
    
    return foundCollision;
}

void StealthMode::CheckLineOfSight(Entity guardEntity, Entity playerEntity, bool& canSee) {
    auto* guardTransform = m_Registry->GetComponent<Transform3DComponent>(guardEntity);
    auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(playerEntity);
    
    if (!guardTransform || !playerTransform) {
        canSee = false;
        return;
    }
    
    // Distance check
    float distX = playerTransform->x - guardTransform->x;
    float distY = playerTransform->y - guardTransform->y;
    float distance = sqrt(distX * distX + distY * distY);
    
    if (distance > m_MaxDetectionDistance) {
        canSee = false;
        return;
    }
    
    // Direction from guard to player
    float playerAngle = atan2(distY, distX);
    float guardFacing = guardTransform->rot;
    
    // Calculate angle difference
    float angleDiff = fabs(playerAngle - guardFacing);
    while (angleDiff > M_PI) angleDiff = 2 * M_PI - angleDiff;
    
    // Check if within field of view
    float viewAngleRad = m_ViewAngle * M_PI / 360.0f;  // Half angle
    
    if (angleDiff >= viewAngleRad) {
        canSee = false;
        return;
    }
    
    // Check for collision obstacles between guard and player
    float collisionDist = 0.0f;
    if (RayHitsCollider(guardTransform->x, guardTransform->y,
                        playerTransform->x, playerTransform->y, collisionDist)) {
        // If collision is closer than player, line of sight is blocked
        if (collisionDist < distance) {
            canSee = false;
            return;
        }
    }
    
    canSee = true;
}

bool StealthMode::IsPlayerInHideSpot() {
    auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    if (!playerTransform)
        return false;
    
    for (auto& spot : m_HideSpots) {
        float distX = playerTransform->x - spot.x;
        float distY = playerTransform->y - spot.y;
        float distance = sqrt(distX * distX + distY * distY);
        
        if (distance < spot.radius) {
            return true;
        }
    }
    
    return false;
}

float StealthMode::GetPlayerVisibility() {
    // Returns 0-1 based on movement speed and position
    auto* playerPhys = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
    if (!playerPhys)
        return 0.5f;
    
    float speed = sqrt(playerPhys->velX * playerPhys->velX + playerPhys->velY * playerPhys->velY);
    return std::min(1.0f, speed / 4.0f);  // Normalized by max speed
}

void StealthMode::RenderUI(GLRenderer* renderer, TextRenderer* textRenderer, int width, int height) {
    if (!m_Active)
        return;
    
    // Cache screen dimensions for spawn editor mouse input
    m_ScreenWidth = width;
    m_ScreenHeight = height;

    // Render basic HUD
    textRenderer->RenderText(renderer, "Stealth Mode", 20, 20, {255, 255, 255, 255});
    textRenderer->RenderText(renderer, "WASD: Move | Space: Jump | ESC: Back", 20, 50, {255, 255, 255, 255});
    
    // Alert level indicator
    std::string alertText = "Alert Level: " + std::to_string((int)(m_PlayerAlertLevel * 100)) + "%";
    SDL_Color alertColor;
    if (m_PlayerAlertLevel < 0.3f) {
        alertColor = {0, 255, 0, 255};  // Green
    } else if (m_PlayerAlertLevel < 0.7f) {
        alertColor = {255, 255, 0, 255};  // Yellow
    } else {
        alertColor = {255, 0, 0, 255};  // Red
    }
    textRenderer->RenderText(renderer, alertText, 20, 80, alertColor);
    
    // Hide spot indicator
    if (IsPlayerInHideSpot()) {
        textRenderer->RenderText(renderer, "HIDDEN", 20, 110, {0, 255, 0, 255});
    }
    
    // Guard count
    std::string guardText = "Guards Nearby: " + std::to_string(m_Guards.size());
    textRenderer->RenderText(renderer, guardText, 20, 140, {200, 200, 200, 255});
    
    // Sneak indicator
    if (m_PlayerSneaking) {
        textRenderer->RenderText(renderer, "SNEAKING (Shift to toggle)", 20, 170, {150, 200, 150, 255});
    }
    
    // Render minimap
    RenderMinimap(renderer, textRenderer, width, height);
    
    // Render guard line of sight cones
    RenderGuardLineOfSight();
    
    // Render game over message
    if (m_Detected) {
        textRenderer->RenderText(renderer, "DETECTED! Game Over. Press ESC to return.", width/2 - 200, height/2, {255, 0, 0, 255});
    }
    
    // Render console and spawn editors
    if (m_Console) m_Console->Render(renderer, textRenderer, width, height);
    if (m_SpawnEditor) m_SpawnEditor->Render(renderer, textRenderer, width, height);
    if (m_VisualSpawnEditor) m_VisualSpawnEditor->Render(renderer, textRenderer, m_Camera, width, height);
}

void StealthMode::RenderMinimap(GLRenderer* renderer, TextRenderer* textRenderer, int screenWidth, int screenHeight) {
    // Position minimap in top right corner
    int minimapX = screenWidth - m_MinimapWidth - 10;
    int minimapY = 10;
    
    // Draw minimap background panel
    UIHelper::DrawPanel(renderer, minimapX, minimapY, m_MinimapWidth, m_MinimapHeight, {30, 30, 30, 220});
    
    // Draw minimap border (using 2D rectangles for lines)
    renderer->DrawRect2D(minimapX - 1, minimapY - 1, m_MinimapWidth + 2, 2, {255, 255, 100, 255});
    renderer->DrawRect2D(minimapX + m_MinimapWidth - 1, minimapY - 1, 2, m_MinimapHeight + 2, {255, 255, 100, 255});
    renderer->DrawRect2D(minimapX - 1, minimapY + m_MinimapHeight - 1, m_MinimapWidth + 2, 2, {255, 255, 100, 255});
    renderer->DrawRect2D(minimapX - 1, minimapY - 1, 2, m_MinimapHeight + 2, {255, 255, 100, 255});
    
    // Get player position
    auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    if (!playerTransform) return;
    
    // Calculate minimap center in screen space
    float minimapCenterX = minimapX + m_MinimapWidth * 0.5f;
    float minimapCenterY = minimapY + m_MinimapHeight * 0.5f;
    
    // Get camera yaw to rotate minimap view
    float cameraYaw = m_CameraController ? m_CameraController->GetCameraYaw() : 0.0f;
    
    // Draw player on minimap (green square at center, rotated indicator showing up)
    float playerScreenX = minimapCenterX;
    float playerScreenY = minimapCenterY;
    renderer->DrawRect2D(playerScreenX - 4, playerScreenY - 4, 8, 8, {0, 255, 0, 255});
    
    // Draw player direction indicator (small line pointing in direction of camera view)
    // Point upward on minimap (towards camera's forward direction)
    float dirLineLen = 8.0f;
    float playerFacingX = playerScreenX;
    float playerFacingY = playerScreenY - dirLineLen;  // Point up
    renderer->DrawLine(playerScreenX, playerScreenY, 0.5f, 
                      playerFacingX, playerFacingY, 0.5f, {0, 255, 100, 255});
    
    // Draw enemies and their vision cones
    for (Entity guardEntity : m_Guards) {
        auto* guardTransform = m_Registry->GetComponent<Transform3DComponent>(guardEntity);
        if (!guardTransform) continue;
        
        // Calculate relative position to player
        float relX = guardTransform->x - playerTransform->x;
        float relY = guardTransform->y - playerTransform->y;
        
        // Rotate relative position by camera yaw (so minimap rotates with camera view)
        float rotatedRelX = relX * cos(-cameraYaw) - relY * sin(-cameraYaw);
        float rotatedRelY = relX * sin(-cameraYaw) + relY * cos(-cameraYaw);
        
        // Scale to minimap coordinates
        float guardScreenX = minimapCenterX + rotatedRelX * m_MinimapScale;
        float guardScreenY = minimapCenterY + rotatedRelY * m_MinimapScale;
        
        // Check if within minimap bounds
        bool offscreen = false;
        if (guardScreenX < minimapX + 5 || guardScreenX > minimapX + m_MinimapWidth - 5 ||
            guardScreenY < minimapY + 5 || guardScreenY > minimapY + m_MinimapHeight - 5) {
            offscreen = true;
        }
        
        if (!offscreen) {
            renderer->DrawRect2D(guardScreenX - 3, guardScreenY - 3, 6, 6, {255, 0, 0, 255});
            
            // Draw enemy facing direction (rotated by camera yaw)
            float enemyAbsoluteDir = -guardTransform->rot - cameraYaw;
            float enemyFacingX = guardScreenX + cos(enemyAbsoluteDir) * 6.0f;
            float enemyFacingY = guardScreenY + sin(enemyAbsoluteDir) * 6.0f;
            renderer->DrawLine(guardScreenX, guardScreenY, 0.5f,
                              enemyFacingX, enemyFacingY, 0.5f, {255, 100, 0, 255});
        }
        
        // Draw vision cone (as lines from enemy position)
        float viewAngleRad = m_ViewAngle * 3.14159f / 180.0f;  // Convert degrees to radians
        int coneSegments = 8;
        
        // Left and right edges of vision cone (rotated by camera yaw)
        float guardAbsoluteRot = guardTransform->rot + cameraYaw;
        float leftAngle = guardAbsoluteRot - viewAngleRad * 0.5f;
        float rightAngle = guardAbsoluteRot + viewAngleRad * 0.5f;
        
        // Draw cone edges
        float coneRadiusScreenUnits = m_MaxDetectionDistance * m_MinimapScale;
        
        float leftEdgeX = guardScreenX + cos(leftAngle) * coneRadiusScreenUnits;
        float leftEdgeY = guardScreenY + sin(leftAngle) * coneRadiusScreenUnits;
        float rightEdgeX = guardScreenX + cos(rightAngle) * coneRadiusScreenUnits;
        float rightEdgeY = guardScreenY + sin(rightAngle) * coneRadiusScreenUnits;
        
        // Draw cone edges in semi-transparent red
        renderer->DrawLine(guardScreenX, guardScreenY, 0.5f,
                          leftEdgeX, leftEdgeY, 0.5f, {255, 100, 100, 150});
        renderer->DrawLine(guardScreenX, guardScreenY, 0.5f,
                          rightEdgeX, rightEdgeY, 0.5f, {255, 100, 100, 150});
        
        // Draw cone arc (approximate with line segments)
        float prevX = guardScreenX + cos(leftAngle) * coneRadiusScreenUnits;
        float prevY = guardScreenY + sin(leftAngle) * coneRadiusScreenUnits;
        
        for (int i = 1; i <= coneSegments; ++i) {
            float angle = leftAngle + (rightAngle - leftAngle) * i / coneSegments;
            float nextX = guardScreenX + cos(angle) * coneRadiusScreenUnits;
            float nextY = guardScreenY + sin(angle) * coneRadiusScreenUnits;
            renderer->DrawLine(prevX, prevY, 0.5f,
                              nextX, nextY, 0.5f, {255, 100, 100, 150});
            prevX = nextX;
            prevY = nextY;
        }
    }
    
    // Draw minimap title
    textRenderer->RenderText(renderer, "MINIMAP", minimapX + 10, minimapY + m_MinimapHeight + 5, {255, 255, 100, 255});
}

void StealthMode::SpawnEnemies() {
    // Load spawn locations from the spawn editor's saved config
    std::vector<SpawnLocation> spawnLocations;
    
    if (!m_SpawnEditor) {
        return;
    }
    
    spawnLocations = m_SpawnEditor->GetSpawnLocations();
    
    for (const auto& loc : spawnLocations) {
        Entity enemy = CharacterFactory::CreateSkeleton(m_Registry, m_Renderer, 
                                                        "skel_warrior", loc.x, loc.y, 0.5f);
        if (enemy != INVALID_ENTITY) {
            // Apply rotation
            auto* transform = m_Registry->GetComponent<Transform3DComponent>(enemy);
            if (transform) {
                transform->rot = loc.rotation;
            }
            
            m_Guards.push_back(enemy);
            m_GuardStates.push_back(GuardState::IDLE);
            m_GuardAlertLevels.push_back(0.0f);
        }
    }
    
    // DEBUG: Print hitbox information
    auto& hitboxes = m_Registry->View<HitboxComponent>();
    std::cout << "DEBUG: Total hitboxes in scene: " << hitboxes.size() << std::endl;
    int solidCount = 0;
    for (auto& pair : hitboxes) {
        HitboxComponent& hitbox = pair.second;
        if (hitbox.isSolid) solidCount++;
    }
    std::cout << "DEBUG: Solid hitboxes: " << solidCount << std::endl;
}

void StealthMode::RenderGuardLineOfSight() {
    // Render vision cones for all guards as semi-transparent red cones
    if (!m_Renderer) {
        return;
    }
    
    for (size_t i = 0; i < m_Guards.size(); ++i) {
        auto* guardTransform = m_Registry->GetComponent<Transform3DComponent>(m_Guards[i]);
        if (!guardTransform) {
            continue;
        }
        
        // Draw a cone representing the guard's field of view
        const int coneSegments = 16;
        const float viewAngleRad = (m_ViewAngle / 2.0f) * M_PI / 180.0f;
        
        // Determine cone color based on guard state
        SDL_Color coneColor;
        if (m_GuardStates[i] == GuardState::ALERT) {
            coneColor = {255, 0, 0, 100};  // Red with transparency when alert
        } else {
            coneColor = {200, 100, 100, 80};  // Dark red with more transparency when idle
        }
        
        // Draw cone segments (triangles from guard to cone edge)
        for (int j = 0; j < coneSegments; ++j) {
            float angle1 = guardTransform->rot - M_PI/2 - viewAngleRad + (2.0f * viewAngleRad * j / coneSegments);
            float angle2 = guardTransform->rot - M_PI/2 - viewAngleRad + (2.0f * viewAngleRad * (j + 1) / coneSegments);
            
            // Cone edge points - calculate max distance considering collisions
            float distToEdge1 = m_MaxDetectionDistance;
            float distToEdge2 = m_MaxDetectionDistance;
            
            // Check for collisions along ray 1
            float x1_max = guardTransform->x + cos(angle1) * m_MaxDetectionDistance;
            float y1_max = guardTransform->y + sin(angle1) * m_MaxDetectionDistance;
            float collisionDist1 = 0.0f;
            if (RayHitsCollider(guardTransform->x, guardTransform->y, x1_max, y1_max, collisionDist1)) {
                distToEdge1 = collisionDist1;
            }
            
            // Check for collisions along ray 2
            float x2_max = guardTransform->x + cos(angle2) * m_MaxDetectionDistance;
            float y2_max = guardTransform->y + sin(angle2) * m_MaxDetectionDistance;
            float collisionDist2 = 0.0f;
            if (RayHitsCollider(guardTransform->x, guardTransform->y, x2_max, y2_max, collisionDist2)) {
                distToEdge2 = collisionDist2;
            }
            
            // Adjust points to collision distance
            float x1 = guardTransform->x + cos(angle1) * distToEdge1;
            float y1 = guardTransform->y + sin(angle1) * distToEdge1;
            
            float x2 = guardTransform->x + cos(angle2) * distToEdge2;
            float y2 = guardTransform->y + sin(angle2) * distToEdge2;
            
            // Draw triangle from guard to two cone edge points
            float z = 0.5f;
            m_Renderer->DrawLine(guardTransform->x, guardTransform->y, z, x1, y1, z, coneColor);
            m_Renderer->DrawLine(x1, y1, z, x2, y2, z, coneColor);
        }
        
        // Draw closing lines with collision awareness
        float angleStart = guardTransform->rot - M_PI/2 - viewAngleRad;
        float angleEnd = guardTransform->rot - M_PI/2 + viewAngleRad;
        
        // Check collisions for closing lines
        float xStart_max = guardTransform->x + cos(angleStart) * m_MaxDetectionDistance;
        float yStart_max = guardTransform->y + sin(angleStart) * m_MaxDetectionDistance;
        float collisionDistStart = 0.0f;
        if (RayHitsCollider(guardTransform->x, guardTransform->y, xStart_max, yStart_max, collisionDistStart)) {
            xStart_max = guardTransform->x + cos(angleStart) * collisionDistStart;
            yStart_max = guardTransform->y + sin(angleStart) * collisionDistStart;
        }
        
        float xEnd_max = guardTransform->x + cos(angleEnd) * m_MaxDetectionDistance;
        float yEnd_max = guardTransform->y + sin(angleEnd) * m_MaxDetectionDistance;
        float collisionDistEnd = 0.0f;
        if (RayHitsCollider(guardTransform->x, guardTransform->y, xEnd_max, yEnd_max, collisionDistEnd)) {
            xEnd_max = guardTransform->x + cos(angleEnd) * collisionDistEnd;
            yEnd_max = guardTransform->y + sin(angleEnd) * collisionDistEnd;
        }
        
        m_Renderer->DrawLine(guardTransform->x, guardTransform->y, 0.5f, xStart_max, yStart_max, 0.5f, coneColor);
        m_Renderer->DrawLine(guardTransform->x, guardTransform->y, 0.5f, xEnd_max, yEnd_max, 0.5f, coneColor);
    }
}

} // namespace PixelsEngine
