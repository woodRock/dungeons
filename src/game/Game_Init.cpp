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

  if (!m_ExplorationMode) {
      m_ExplorationMode = std::make_unique<ExplorationMode>(&m_Registry, &m_GLRenderer, &m_Editor);
  }
  m_ExplorationMode->Init(m_Camera.get(), m_PlayerEntity);
  
  m_State = GameState::Playing; 
}

void DungeonsGame::InitSiege() {
    m_Registry = Registry();
    m_IsGrappling = false;
    m_GameFinished = false;
    m_State = GameState::Siege;
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    if (!m_SiegeMode) {
        m_SiegeMode = std::make_unique<SiegeMode>(&m_Registry, &m_GLRenderer);
    }
    m_SiegeMode->Init(m_Camera.get(), m_PlayerEntity);
}