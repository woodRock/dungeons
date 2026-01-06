#include "../engine/Components.h"
#include "../engine/TextureManager.h"
#include "../engine/GLTFLoader.h"
#include "DungeonsGame.h"
#include <SDL2/SDL.h>
#include <iostream>

using namespace PixelsEngine;

DungeonsGame::DungeonsGame()
    : Application("Dungeons - Animation Test", 1280, 720) {}

DungeonsGame::~DungeonsGame() {}

void DungeonsGame::OnStart() {
  m_GLRenderer.Init(m_Width, m_Height);
  m_TextRenderer = std::make_unique<TextRenderer>("assets/font.ttf", 24);

  // Load basic dungeon texture
  m_GLRenderer.LoadTexture("dungeon", "assets/dungeons/Assets/textures/dungeon_texture.png");

  // Load floor mesh
  m_GLRenderer.LoadMesh("floor", "assets/dungeons/Assets/obj/floor_tile_large.obj");

  InitGame();

  // Start directly in Playing state for the test
  m_State = GameState::Playing;
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void DungeonsGame::InitSiege() {
    // Siege mode skipped for this test
}

void DungeonsGame::InitGame() {
  m_Registry = Registry();
  m_IsGrappling = false;
  m_GameFinished = false;
  m_RunTimer = 0.0f;
  m_TargetsDestroyed = 0;

  m_Editor.Init(&m_Registry, &m_GLRenderer);

  // 1. Load Knight Assets (GLB)
  if (!m_GLRenderer.LoadMesh("knight", "assets/adventurers/Characters/gltf/Knight.glb")) {
      std::cerr << "Failed to load knight mesh!" << std::endl;
  }
  if (!m_GLRenderer.LoadTexture("knight_tex", "assets/adventurers/Textures/knight_texture.png")) {
      std::cerr << "Failed to load knight texture!" << std::endl;
  }

  // 2. Load Animations
  std::vector<SkeletalAnimation> anims = GLTFLoader::LoadAnimations("assets/adventurers/Animations/gltf/Rig_Medium/Rig_Medium_General.glb");
  RenderMesh* kRm = m_GLRenderer.GetRenderMesh("knight");
  if (kRm) {
      kRm->animations = anims;
      kRm->isSkinned = true;
  }

  // 3. Spawn Floor Tile
  Entity floor = m_Registry.CreateEntity();
  m_Registry.AddComponent<Transform3DComponent>(floor, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
  m_Registry.AddComponent<MeshComponent>(floor, {"floor", "dungeon", 1.0f, 1.0f, 1.0f});

  // 4. Spawn Animated Knight
  Entity knight = m_Registry.CreateEntity();
  m_Registry.AddComponent<Transform3DComponent>(knight, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
  m_Registry.AddComponent<MeshComponent>(knight, {"knight", "knight_tex", 1.0f, 1.0f, 1.0f});
  
  int idleIdx = 0;
  if (kRm && !kRm->animations.empty()) {
      for (size_t i = 0; i < kRm->animations.size(); ++i) {
          if (kRm->animations[i].name.find("Idle_A") != std::string::npos) {
              idleIdx = (int)i; break;
          }
      }
  }
  m_Registry.AddComponent<SkeletalAnimationComponent>(knight, {idleIdx, 0.0f, 1.0f});

  // 5. Setup Camera / Player
  m_PlayerEntity = m_Registry.CreateEntity();
  m_Registry.AddComponent<Transform3DComponent>(m_PlayerEntity, {0.0f, -5.0f, 2.0f, M_PI * 0.5f, -0.2f});
  m_Registry.AddComponent<PlayerControlComponent>(m_PlayerEntity, {10.0f, 0.003f, 6.0f});
  m_Registry.AddComponent<PhysicsComponent>(m_PlayerEntity, {0, 0, 0, 15.0f, true, true, 0.0f, 5.0f}); // Flying mode
  m_Registry.AddComponent<WeaponComponent>(m_PlayerEntity, {0.0f, 0.0f, false});

  m_Camera->x = 0.0f; m_Camera->y = -5.0f; m_Camera->z = 2.0f;
  m_Camera->yaw = M_PI * 0.5f; m_Camera->pitch = -0.2f;
}