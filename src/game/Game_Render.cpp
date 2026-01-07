#include "../engine/Components.h"
#include "DungeonsGame.h"
#include <cmath>

using namespace PixelsEngine;

void DungeonsGame::OnRender() {
  // 1. Setup clear color based on game state
  if (m_State == GameState::Dungeon && m_DungeonMode) {
    // Dungeon uses black skybox
    m_GLRenderer.SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  } else if (m_State == GameState::MainMenu || m_State == GameState::MapSelect || m_State == GameState::FloorSelect) {
    // Menu or MapSelect specific clear
    m_GLRenderer.SetClearColor(0.1f, 0.1f, 0.2f, 1.0f);
  } else {
    // Default clear color (sky blue)
    m_GLRenderer.SetClearColor(0.53f, 0.81f, 0.92f, 1.0f);
  }

  // 2. Setup Viewport (crucial for HighDPI and avoiding artifacts)
  int dw, dh;
  SDL_GL_GetDrawableSize(m_Window, &dw, &dh);
  glViewport(0, 0, dw, dh);

  // 2. Render 3D Scene (OpenGL), don't swap yet if we have UI
  if (m_State != GameState::MainMenu && m_State != GameState::MapSelect && m_State != GameState::FloorSelect) {
    // m_GLRenderer.Render already sets viewport and clears,
    // but we called it above too. That's fine.
    // We pass false to prevent internal swap.
    m_GLRenderer.Render(m_Window, *m_Camera, m_Registry, false);

    if (m_State == GameState::Battle && m_BattleMode) {
      m_BattleMode->RenderWorld(&m_GLRenderer);
    }

    if (m_State == GameState::Creative) {
      m_CreativeMode.RenderWorld(&m_GLRenderer);
    }
  }

  // 2. Render UI (using GLRenderer 2D methods)
  if (m_State == GameState::MainMenu) {
    RenderMainMenu();
  } else if (m_State == GameState::Creative) {
    m_CreativeMode.RenderUI(&m_GLRenderer, m_TextRenderer.get(), m_Width, m_Height);
    if (m_Minimap) {
      m_Minimap->Render(&m_GLRenderer, &m_Registry, m_PlayerEntity, m_Camera.get(), m_Width, m_Height);
    }
  } else if (m_State == GameState::Paused) {
    RenderPauseMenu();
  } else if (m_State == GameState::MapSelect) {
    RenderMapSelect();
  } else if (m_State == GameState::FloorSelect) {
    RenderFloorSelect();
  } else if (m_State == GameState::Playing || m_State == GameState::Battle ||
             m_State == GameState::Siege || m_State == GameState::Dungeon) {
    if (m_State == GameState::Dungeon && m_DungeonMode) {
        m_DungeonMode->RenderUI(&m_GLRenderer, m_TextRenderer.get(), m_Width, m_Height);
    }
    RenderUI();
  }

  // 3. Swap OpenGL buffers
  SDL_GL_SwapWindow(m_Window);
  glFlush();
}

// RenderGameplay was not found in Game_UI.cpp, so we keep a stub here.
void DungeonsGame::RenderGameplay() {}
