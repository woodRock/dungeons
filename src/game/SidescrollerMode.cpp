#include "SidescrollerMode.h"
#include "../engine/Components.h"
#include "../engine/Input.h"
#include "../engine/TextRenderer.h"
#include "../engine/AssetManager.h"
#include "../engine/CharacterFactory.h"
#include <iostream>

namespace PixelsEngine {

SidescrollerMode::SidescrollerMode(Registry* registry, GLRenderer* renderer)
    : m_Registry(registry), m_Renderer(renderer), m_Skybox(std::make_unique<Skybox>()) {}

SidescrollerMode::~SidescrollerMode() {}

void SidescrollerMode::Init(Camera* camera, Entity& playerEntity) {
    m_Camera = camera;
    m_Active = true;
    
    std::cout << "DEBUG: SidescrollerMode::Init called, camera pointer = " << (void*)m_Camera << std::endl;
    
    // Set sidescroller skybox to black
    m_Skybox->SetColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Pre-load character assets
    auto assetManager = std::make_unique<AssetManager>(m_Renderer);
    assetManager->LoadCharacter("Knight",
                                 "assets/adventurers/Characters/gltf/Knight.glb",
                                 "assets/adventurers/Textures/knight_texture.png");

    // Load the sidescroller map and create player
    LoadLevel("assets/saves/2D_SIDESCROLLER.map");
    
    // Create movement controller AFTER player is created
    if (m_PlayerEntity != PixelsEngine::INVALID_ENTITY) {
        m_MovementController = std::make_unique<TopDownMovementController>(
            m_Registry, m_PlayerEntity);
        std::cout << "DEBUG: Created movement controller for player entity" << std::endl;
    } else {
        std::cerr << "ERROR: Failed to create player entity" << std::endl;
    }
    
    playerEntity = m_PlayerEntity;
}

// Helper to lazily cache animation indices for the player
static int FindAnimationIndex(RenderMesh* rm, const std::string& pattern) {
    if (!rm) return -1;
    for (size_t i = 0; i < rm->animations.size(); ++i) {
        if (rm->animations[i].name.find(pattern) != std::string::npos) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void SidescrollerMode::LoadLevel(const std::string& mapFile) {
    // Load the map first
    int entitiesLoaded = MapLoader::LoadFromFileWithPath(
        mapFile, m_Registry, m_Renderer, "assets/dungeons/Assets/obj/");
    
    if (entitiesLoaded > 0) {
        std::cout << "Loaded sidescroller map: " << mapFile 
                  << " with " << entitiesLoaded << " entities" << std::endl;
    } else {
        std::cerr << "Failed to load sidescroller map: " << mapFile << std::endl;
    }

    // Find spawn point from player_spawn marker in the map
    float spawnX = 4.0f;  // Default fallback
    float spawnY = 0.0f;
    float spawnZ = 2.0f;
    float playerRot = M_PI * 0.5f;
    Entity spawnMarkerEntity = PixelsEngine::INVALID_ENTITY;
    
    auto& meshes = m_Registry->View<MeshComponent>();
    for (auto& pair : meshes) {
        if (pair.second.meshName == "player_spawn") {
            auto* t = m_Registry->GetComponent<Transform3DComponent>(pair.first);
            if (t) {
                spawnX = t->x;
                spawnY = t->y;
                spawnZ = t->z;
                playerRot = t->rot;
                spawnMarkerEntity = pair.first;
                std::cout << "Found player_spawn at X=" << spawnX << ", Y=" << spawnY << ", Z=" << spawnZ << std::endl;
                break;
            }
        }
    }

    // Create the player if it doesn't exist - use CharacterFactory for proper setup
    if (m_PlayerEntity == PixelsEngine::INVALID_ENTITY) {
        m_PlayerEntity = CharacterFactory::CreatePlayer(m_Registry, m_Renderer, spawnX, spawnY, spawnZ);
        
        // Update player rotation to match spawn marker
        auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
        if (playerTransform) {
            playerTransform->rot = playerRot;
        }
        
        // Remove the player_spawn marker entity since we don't need to render it
        if (spawnMarkerEntity != PixelsEngine::INVALID_ENTITY) {
            m_Registry->DestroyEntity(spawnMarkerEntity);
            std::cout << "Removed player_spawn marker entity" << std::endl;
        }
        
        if (m_Camera) {
            m_Camera->x = spawnX - 8.0f;  // 8 units to the left (rotated 90° counter-clockwise)
            m_Camera->y = spawnY;
            m_Camera->z = spawnZ + 3.0f;
            m_Camera->yaw = M_PI * 0.5f;  // Fixed camera angle for sidescroller
            m_Camera->pitch = -0.3f;
            std::cout << "Camera positioned at X=" << m_Camera->x << ", Y=" << m_Camera->y << ", Z=" << m_Camera->z << std::endl;
        }
    }
}

void SidescrollerMode::Update(float dt, Entity& playerEntity) {
    if (!m_Active || !m_Camera)
        return;

    // Handle custom WASD input for sidescroller (rotated 90 degrees)
    // W: Left (negative X), A: Back (negative Y), S: Right (positive X), D: Forward (positive Y)
    auto *phys = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
    if (phys) {
        float accel = 80.0f * dt;  // Acceleration
        float maxSpeed = 8.0f;     // Max speed
        float friction = 0.92f;    // Friction coefficient
        
        float dx = 0, dy = 0;
        
        // Rotated input mapping for sidescroller
        if (Input::IsKeyDown(SDL_SCANCODE_W)) dx += 1.0f;  // W = Move Right (+X)
        if (Input::IsKeyDown(SDL_SCANCODE_A)) dy += 1.0f;  // A = Move Forward (+Y)
        if (Input::IsKeyDown(SDL_SCANCODE_S)) dx -= 1.0f;  // S = Move Left (-X)
        if (Input::IsKeyDown(SDL_SCANCODE_D)) dy -= 1.0f;  // D = Move Back (-Y)
        
        if (dx != 0 || dy != 0) {
            // Normalize
            float len = sqrt(dx * dx + dy * dy);
            dx /= len;
            dy /= len;
            
            // Apply acceleration
            float newVelX = phys->velX + dx * accel;
            float newVelY = phys->velY + dy * accel;
            
            // Clamp to max speed
            float speed = sqrt(newVelX * newVelX + newVelY * newVelY);
            if (speed > maxSpeed) {
                float scale = maxSpeed / speed;
                newVelX *= scale;
                newVelY *= scale;
            }
            
            phys->velX = newVelX;
            phys->velY = newVelY;
        } else {
            // Apply friction when no input
            phys->velX *= friction;
            phys->velY *= friction;
        }
        
        // Rotate player to face movement direction
        if (dx != 0 || dy != 0) {
            auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
            if (playerTransform) {
                // Calculate rotation from velocity direction
                // Negate dx to fix left/right orientation
                float targetYaw = atan2(dy, -dx);
                // Apply rotation offset for debugging
                float rotationOffset = m_PlayerRotationOffset * M_PI * 0.5f;
                playerTransform->rot = targetYaw + rotationOffset;
            }
        }
        
        // Handle jump input with double jump
        bool spacePressed = Input::IsKeyDown(SDL_SCANCODE_SPACE);
        if (spacePressed && !m_WasSpacePressed) {
            if (phys->isGrounded) {
                // First jump from ground
                phys->velZ = m_JumpVelocity;
                phys->isGrounded = false;
                m_HasDoubleJumped = false;
            } else if (!m_HasDoubleJumped) {
                // Double jump in air
                phys->velZ = m_JumpVelocity;
                m_HasDoubleJumped = true;
            }
        }
        
        // Reset double jump when grounded
        if (phys->isGrounded) {
            m_HasDoubleJumped = false;
        }
        
        m_WasSpacePressed = spacePressed;
    }
    
    // Update camera to follow player
    auto* playerTransform = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    if (playerTransform && m_Camera) {
        // Calculate camera offset based on rotation angle
        float offsetDist = 8.0f;
        float offsetX = 0.0f, offsetY = 0.0f;
        
        // Apply rotation: 0=left(-X), 1=back(-Y), 2=right(+X), 3=front(+Y)
        // Camera positioned around player and facing towards player
        switch (m_CameraRotation) {
            case 0:  // 0°: Left side (-X)
                offsetX = -offsetDist;
                offsetY = 0.0f;
                m_Camera->yaw = 0.0f;  // Look towards +X (at player)
                break;
            case 1:  // 90°: Back side (-Y)
                offsetX = 0.0f;
                offsetY = -offsetDist;
                m_Camera->yaw = M_PI * 0.5f;  // Look towards +Y (at player)
                break;
            case 2:  // 180°: Right side (+X)
                offsetX = offsetDist;
                offsetY = 0.0f;
                m_Camera->yaw = M_PI;  // Look towards -X (at player)
                break;
            case 3:  // 270°: Front side (+Y)
                offsetX = 0.0f;
                offsetY = offsetDist;
                m_Camera->yaw = -M_PI * 0.5f;  // Look towards -Y (at player)
                break;
        }
        
        m_Camera->x = playerTransform->x + offsetX;
        m_Camera->y = playerTransform->y + offsetY;
        m_Camera->z = playerTransform->z + 3.0f;  // 3 units up
        m_Camera->pitch = -0.3f;
    }

    // Update player animation state (idle / run / jump)
    if (playerTransform) {
        auto* phys = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
        auto* anim = m_Registry->GetComponent<SkeletalAnimationComponent>(m_PlayerEntity);
        auto* mesh = m_Registry->GetComponent<MeshComponent>(m_PlayerEntity);
        if (anim && mesh && m_Renderer) {
            // Lazy init animation indices
            if (m_AnimIdle == -1 || m_AnimRun == -1 || m_AnimJump == -1) {
                RenderMesh* rm = m_Renderer->GetRenderMesh(mesh->meshName);
                if (rm) {
                    m_AnimIdle = FindAnimationIndex(rm, "Idle");
                    m_AnimRun  = FindAnimationIndex(rm, "Run");
                    if (m_AnimRun == -1) m_AnimRun = FindAnimationIndex(rm, "Walk");
                    m_AnimJump = FindAnimationIndex(rm, "Jump");
                }
            }

            float horizontalSpeed = 0.0f;
            if (phys) {
                horizontalSpeed = std::sqrt(phys->velX * phys->velX + phys->velY * phys->velY);
            }

            int desiredAnim = anim->animationIndex;
            bool desiredLoop = true;

            if (phys && !phys->isGrounded) {
                if (m_AnimJump != -1) {
                    desiredAnim = m_AnimJump;
                    desiredLoop = false;
                }
            } else if (horizontalSpeed > 0.2f) {
                if (m_AnimRun != -1) desiredAnim = m_AnimRun;
                desiredLoop = true;
            } else {
                if (m_AnimIdle != -1) desiredAnim = m_AnimIdle;
                desiredLoop = true;
            }

            if (desiredAnim != anim->animationIndex) {
                anim->animationIndex = desiredAnim;
                anim->currentTime = 0.0f;
                anim->loop = desiredLoop;
            } else {
                anim->loop = desiredLoop;
            }
        }
    }
}

void SidescrollerMode::RenderUI(GLRenderer* renderer, TextRenderer* textRenderer, int width, int height) {
    if (!m_Active)
        return;

    // Render basic HUD for sidescroller
    textRenderer->RenderText(renderer, "Sidescroller Mode", 20, 20, {255, 255, 255, 255});
    textRenderer->RenderText(renderer, "WASD: Move | Space: Jump | ESC: Back", 20, 50, {255, 255, 255, 255});
}

} // namespace PixelsEngine
