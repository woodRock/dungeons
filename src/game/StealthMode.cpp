#include "StealthMode.h"
#include "../engine/TextRenderer.h"
#include "../engine/Input.h"
#include "../engine/Components.h"
#include "../engine/CharacterFactory.h"
#include "../engine/AssetManager.h"
#include "../engine/UIHelper.h"
#include <cmath>
#include <iostream>

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
    
    // Spawn player using CharacterFactory at a valid floor location in the dungeon
    // Moved back from box_stacked obstacle to avoid collision issues
    m_PlayerEntity = CharacterFactory::CreatePlayer(m_Registry, m_Renderer, 36.0f, -20.0f, 0.5f);
    playerEntity = m_PlayerEntity;
    
    // Spawn enemies in the level
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
    
    // Initialize console and spawn editor
    m_Console = std::make_unique<Console>();
    m_SpawnEditor = std::make_unique<StealthSpawnEditor>();
    m_VisualSpawnEditor = std::make_unique<VisualSpawnEditor>(m_Registry, m_Renderer);
    
    // Load spawn locations from saved config
    m_SpawnEditor->LoadFromFile("assets/saves/spawn_config.txt");
    auto spawnLocations = m_SpawnEditor->GetSpawnLocations();
    
    // Initialize visual spawn editor with loaded spawn locations
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
    
    canSee = angleDiff < viewAngleRad;
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
    std::vector<std::pair<float, float>> spawnLocations;
    
    if (m_SpawnEditor) {
        spawnLocations = m_SpawnEditor->GetSpawnLocations();
    }
    
    std::cout << "DEBUG: SpawnEnemies called, attempting to spawn " << spawnLocations.size() << " enemies" << std::endl;
    
    for (const auto& loc : spawnLocations) {
        std::cout << "DEBUG: Creating skeleton at (" << loc.first << ", " << loc.second << ")" << std::endl;
        Entity enemy = CharacterFactory::CreateSkeleton(m_Registry, m_Renderer, 
                                                        "skel_warrior", loc.first, loc.second, 0.5f);
        std::cout << "DEBUG: CreateSkeleton returned entity ID: " << enemy << std::endl;
        if (enemy != INVALID_ENTITY) {
            m_Guards.push_back(enemy);
            m_GuardStates.push_back(GuardState::IDLE);
            m_GuardAlertLevels.push_back(0.0f);
            std::cout << "DEBUG: Spawned skeleton guard at (" << loc.first << ", " << loc.second << ")" << std::endl;
        } else {
            std::cout << "DEBUG: Failed to create skeleton - INVALID_ENTITY returned" << std::endl;
        }
    }
    std::cout << "DEBUG: SpawnEnemies completed. Total guards: " << m_Guards.size() << std::endl;
}

void StealthMode::RenderGuardLineOfSight() {
    // Render vision cones for all guards as semi-transparent red cones
    for (size_t i = 0; i < m_Guards.size(); ++i) {
        auto* guardTransform = m_Registry->GetComponent<Transform3DComponent>(m_Guards[i]);
        if (!guardTransform) continue;
        
        // Draw a simple cone representing the guard's field of view
        // The cone has: origin at guard position, opens in the direction they face,
        // with an angle of m_ViewAngle degrees and range of m_MaxDetectionDistance
        
        const int coneSegments = 16;
        const float viewAngleRad = (m_ViewAngle / 2.0f) * M_PI / 180.0f;
        
        // Guard facing direction (based on their rotation)
        float guardFacingX = -sin(guardTransform->rot);
        float guardFacingY = -cos(guardTransform->rot);
        
        // Draw cone perimeter lines
        for (int j = 0; j < coneSegments; ++j) {
            float angle1 = guardTransform->rot - viewAngleRad + (2.0f * viewAngleRad * j / coneSegments);
            float angle2 = guardTransform->rot - viewAngleRad + (2.0f * viewAngleRad * (j + 1) / coneSegments);
            
            // Cone edge points
            float x1 = guardTransform->x + cos(angle1) * m_MaxDetectionDistance;
            float y1 = guardTransform->y + sin(angle1) * m_MaxDetectionDistance;
            
            float x2 = guardTransform->x + cos(angle2) * m_MaxDetectionDistance;
            float y2 = guardTransform->y + sin(angle2) * m_MaxDetectionDistance;
            
            // Draw lines from guard to cone edge and between edges
            // These would be 3D lines in the actual renderer
            // For now, we just store this for visual debugging
        }
    }
}

} // namespace PixelsEngine
