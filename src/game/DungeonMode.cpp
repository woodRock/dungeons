#include "DungeonMode.h"
#include "../engine/Components.h"
#include "../engine/Input.h"
#include "../engine/TextRenderer.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace PixelsEngine {

DungeonMode::DungeonMode(Registry* registry, GLRenderer* renderer)
    : m_Registry(registry), m_Renderer(renderer) {}

DungeonMode::~DungeonMode() {}

void DungeonMode::Init(Camera* camera, Entity& playerEntity) {
    m_Camera = camera;
    m_AudioManager = std::make_unique<AudioManager>();
    m_SfxShoot = m_AudioManager->LoadWAV("assets/bow_shoot.wav");
    m_SfxHit = m_AudioManager->LoadWAV("assets/bow_hit.wav");
    m_SfxDoor = m_AudioManager->LoadWAV("assets/door.wav");

    // Pre-load character assets to fix skeleton loading issue
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

    // Start with a default level list if none provided
    if (m_LevelList.empty()) {
        m_LevelList = {"my_dungeon"};
    }
    
    m_CurrentLevelIdx = 0;
    LoadLevel(m_LevelList[m_CurrentLevelIdx]);
    
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
    std::string path = "assets/dungeons/" + levelName + ".map";
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

    // Update Controllers
    if (m_CameraController) m_CameraController->HandleInput(dt);
    if (m_CameraController) m_CameraController->Update(dt);
    if (m_MovementController) m_MovementController->HandleInput(dt);

    auto* t = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
    if (t && m_CameraController) {
        if (m_FirstFrame) {
            m_FirstFrame = false;
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
