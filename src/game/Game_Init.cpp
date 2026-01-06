#include "../engine/Components.h"
#include "../engine/TextureManager.h"
#include "../engine/GLTFLoader.h"
#include "DungeonsGame.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

using namespace PixelsEngine;

DungeonsGame::DungeonsGame()
    : Application("Dungeons", 1280, 720) {}

DungeonsGame::~DungeonsGame() {}

void DungeonsGame::OnStart() {
  m_GLRenderer.Init(m_Width, m_Height);
  m_TextRenderer = std::make_unique<TextRenderer>("assets/font.ttf", 24);

  // Load basic dungeon texture
  if (!m_GLRenderer.LoadTexture("dungeon", "assets/dungeons/Assets/textures/dungeon_texture.png")) {
      std::cerr << "Failed to load dungeon texture" << std::endl;
  }

  // Load floor mesh
  if (!m_GLRenderer.LoadMesh("floor", "assets/dungeons/Assets/obj/floor_tile_large.obj")) {
      std::cerr << "Failed to load floor mesh" << std::endl;
  }
  
  // Load UI Assets or other common assets could go here
  
  InitMainMenu();
}

void DungeonsGame::InitMainMenu() {
    m_State = GameState::MainMenu;
    SDL_SetRelativeMouseMode(SDL_FALSE);
    m_MenuSelection = 0;
}

void DungeonsGame::InitGame() {
  m_Registry = Registry();
  m_IsGrappling = false;
  m_GameFinished = false;
  m_RunTimer = 0.0f;
  m_TargetsDestroyed = 0;

  m_Editor.Init(&m_Registry, &m_GLRenderer);

  // Spawn Floor Tile
  Entity floor = m_Registry.CreateEntity();
  m_Registry.AddComponent<Transform3DComponent>(floor, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
  m_Registry.AddComponent<MeshComponent>(floor, {"floor", "dungeon", 1.0f, 1.0f, 1.0f});

  // Setup Camera / Player
  m_PlayerEntity = m_Registry.CreateEntity();
  m_Registry.AddComponent<Transform3DComponent>(m_PlayerEntity, {0.0f, -5.0f, 2.0f, M_PI * 0.5f, -0.2f});
  m_Registry.AddComponent<PlayerControlComponent>(m_PlayerEntity, {10.0f, 0.003f, 6.0f});
  m_Registry.AddComponent<PhysicsComponent>(m_PlayerEntity, {0, 0, 0, 15.0f, true, true, 0.0f, 5.0f}); // Flying mode for creative/test
  m_Registry.AddComponent<WeaponComponent>(m_PlayerEntity, {0.0f, 0.0f, false});

  m_Camera->x = 0.0f; m_Camera->y = -5.0f; m_Camera->z = 2.0f;
  m_Camera->yaw = M_PI * 0.5f; m_Camera->pitch = -0.2f;
  
  m_State = GameState::Playing; // Or Creative if preferred
}

void DungeonsGame::InitSiege() {
    m_Registry = Registry();
    m_IsGrappling = false;
    m_GameFinished = false;
    
    // 1. Load Skeleton Assets
    // Load Texture
    if (!m_GLRenderer.GetRenderMesh("skel_minion")) { // Check if loaded to avoid reload
        if (!m_GLRenderer.LoadTexture("skeleton_tex", "assets/skeletons/texture/skeleton_texture.png")) {
             std::cerr << "Failed to load skeleton texture" << std::endl;
        }
        
        m_GLRenderer.LoadMesh("skel_minion", "assets/skeletons/characters/gltf/Skeleton_Minion.glb");
        m_GLRenderer.LoadMesh("skel_warrior", "assets/skeletons/characters/gltf/Skeleton_Warrior.glb");
        m_GLRenderer.LoadMesh("skel_mage", "assets/skeletons/characters/gltf/Skeleton_Mage.glb");
        
        // 2. Load and Assign Animations
        std::vector<SkeletalAnimation> anims = GLTFLoader::LoadAnimations("assets/skeletons/Animations/gltf/Rig_Medium/Rig_Medium_General.glb");
        
        auto SetupMesh = [&](const std::string& name) {
            RenderMesh* rm = m_GLRenderer.GetRenderMesh(name);
            if (rm) {
                rm->animations = anims;
                rm->isSkinned = true;
            }
        };
        SetupMesh("skel_minion");
        SetupMesh("skel_warrior");
        SetupMesh("skel_mage");
    }

    // 3. Spawn Floor
    Entity floor = m_Registry.CreateEntity();
    m_Registry.AddComponent<Transform3DComponent>(floor, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
    m_Registry.AddComponent<MeshComponent>(floor, {"floor", "dungeon", 1.0f, 1.0f, 1.0f});
    
    // 4. Spawn Player
    m_PlayerEntity = m_Registry.CreateEntity();
    m_Registry.AddComponent<Transform3DComponent>(m_PlayerEntity, {0.0f, 0.0f, 2.0f, 0.0f, 0.0f});
    m_Registry.AddComponent<PlayerControlComponent>(m_PlayerEntity, {10.0f, 0.003f, 6.0f});
    m_Registry.AddComponent<PhysicsComponent>(m_PlayerEntity, {0, 0, 0, 15.0f, true, false, 0.0f, 1.0f}); // Normal physics
    m_Registry.AddComponent<WeaponComponent>(m_PlayerEntity, {0.0f, 0.0f, false});
    m_Camera->x = 0.0f; m_Camera->y = 0.0f; m_Camera->z = 2.0f;
    m_Camera->yaw = 0; m_Camera->pitch = 0;

    // 5. Spawn Skeletons
    auto SpawnSkeleton = [&](const std::string& mesh, float x, float y, CharacterComponent::Type type) {
        Entity e = m_Registry.CreateEntity();
        float rot = atan2(-y, -x); // Face center
        m_Registry.AddComponent<Transform3DComponent>(e, {x, y, 0.0f, rot, 0.0f});
        m_Registry.AddComponent<MeshComponent>(e, {mesh, "skeleton_tex", 1.0f, 1.0f, 1.0f});
        
        // Find Idle animation
        int idleIdx = 0;
        RenderMesh* rm = m_GLRenderer.GetRenderMesh(mesh);
        if (rm && !rm->animations.empty()) {
             for(size_t i=0; i<rm->animations.size(); i++) {
                 if(rm->animations[i].name.find("Idle") != std::string::npos) { idleIdx=(int)i; break; }
             }
        }
        m_Registry.AddComponent<SkeletalAnimationComponent>(e, {idleIdx, (float)(rand()%100)*0.01f, 1.0f});
        m_Registry.AddComponent<EnemyComponent>(e, {100.0f, 1.5f + (float)(rand()%10)*0.1f, -1});
        m_Registry.AddComponent<CharacterComponent>(e, {type});
        m_Registry.AddComponent<ColliderComponent>(e, {0.5f, 1.8f, true});
    };
    
    // Circle of minions
    for(int i=0; i<8; i++) {
        float angle = i * (2*M_PI/8);
        SpawnSkeleton("skel_minion", cos(angle)*8.0f, sin(angle)*8.0f, CharacterComponent::SkeletonMinion);
    }
    // Some specialists
    SpawnSkeleton("skel_warrior", 12.0f, 0.0f, CharacterComponent::SkeletonWarrior);
    SpawnSkeleton("skel_mage", -12.0f, 0.0f, CharacterComponent::SkeletonMage);
}