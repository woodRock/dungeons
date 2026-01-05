#include "../engine/Components.h"
#include "../engine/TextureManager.h"
#include "DungeonsGame.h"
#include <SDL2/SDL.h>

using namespace PixelsEngine;

DungeonsGame::DungeonsGame()
    : Application("Dungeons - Dungeon Renderer", 1280, 720) {}

DungeonsGame::~DungeonsGame() {
  // Cleanup logic
}

void DungeonsGame::OnStart() {
  m_GLRenderer.Init(m_Width, m_Height);

  m_TextRenderer = std::make_unique<TextRenderer>("assets/font.ttf", 24);
  std::string texPath = "assets/dungeons/Assets/textures/dungeon_texture.png";
  if (!m_GLRenderer.LoadTexture("dungeon", texPath)) {
    std::cerr << "Failed to load dungeon texture!" << std::endl;
  }

  std::string objPath = "assets/dungeons/Assets/obj/";
  m_GLRenderer.LoadMesh("floor", objPath + "floor_tile_large.obj");
  m_GLRenderer.LoadMesh("wall", objPath + "wall.obj");
  m_GLRenderer.LoadMesh("corner", objPath + "wall_corner.obj");
  m_GLRenderer.LoadMesh("door", objPath + "wall_doorway.obj");
  m_GLRenderer.LoadMesh("barrel", objPath + "barrel_small.obj");
  m_GLRenderer.LoadMesh("chest", objPath + "chest.obj");
  m_GLRenderer.LoadMesh("column", objPath + "column.obj");

  // Load Audio
  m_SfxShoot = Mix_LoadWAV("assets/shoot.wav");
  m_SfxHit = Mix_LoadWAV("assets/hit.wav");
  m_SfxJump = Mix_LoadWAV("assets/jump.wav");
  m_SfxGrapple = Mix_LoadWAV("assets/grapple.wav");
  m_SfxDoor = Mix_LoadWAV("assets/door.wav");
  m_SfxChestOpen = Mix_LoadWAV("assets/chest_open.wav");
  m_Ambience = Mix_LoadMUS("assets/ambience_flute.mp3");

  if (m_Ambience) {
    Mix_PlayMusic(m_Ambience, -1);
    Mix_VolumeMusic(64);
  }

  InitGame();

  // Start at Main Menu
  m_State = GameState::MainMenu;
  SDL_SetRelativeMouseMode(SDL_FALSE);
}

void DungeonsGame::InitGame() {
  m_Registry = Registry();

  m_IsGrappling = false;
  m_GameFinished = false;
  m_RunTimer = 0.0f;
  m_TargetsDestroyed = 0;

  // Init Editor
  m_Editor.Init(&m_Registry, &m_GLRenderer);

  // Create Player/Camera entity
  m_PlayerEntity = m_Registry.CreateEntity();
  m_Registry.AddComponent<Transform3DComponent>(m_PlayerEntity,
                                                {0.0f, 0.0f, 2.0f, 0.0f, 0.0f});
  m_Registry.AddComponent<PlayerControlComponent>(m_PlayerEntity,
                                                  {10.0f, 0.003f, 6.0f});
  m_Registry.AddComponent<PhysicsComponent>(
      m_PlayerEntity, {0, 0, 0, 15.0f, true, false, 0.0f, 5.0f});
  m_Registry.AddComponent<ColliderComponent>(m_PlayerEntity,
                                             {0.5f, 2.0f, true});
  m_Registry.AddComponent<WeaponComponent>(m_PlayerEntity, {0.0f, 0.0f, false});

  // Set initial camera
  m_Camera->x = 0.0f;
  m_Camera->y = 0.0f;
  m_Camera->z = 2.0f;
}
