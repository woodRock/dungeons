#include "../engine/Components.h"
#include "DungeonsGame.h"
#include <cmath>

using namespace PixelsEngine;

void DungeonsGame::OnRender() {
  // 1. Setup Viewport and Clear (Crucial for HighDPI and avoiding artifacts)
  int dw, dh;
  SDL_GL_GetDrawableSize(m_Window, &dw, &dh);
  glViewport(0, 0, dw, dh);

  // Default clear color (sky blue)
  glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 2. Render 3D Scene (OpenGL), don't swap yet if we have UI
  if (m_State != GameState::MainMenu) {
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
  } else {
    // Menu specific clear if different
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
  } else if (m_State == GameState::Playing || m_State == GameState::Battle ||
             m_State == GameState::Siege) {
    RenderUI();
  }

  // 3. Swap OpenGL buffers
  SDL_GL_SwapWindow(m_Window);
  glFlush();
}

// RenderGameplay was not found in Game_UI.cpp, so we keep a stub here.
void DungeonsGame::RenderGameplay() {}

// The following are defined in Game_UI.cpp, so we do NOT define them here.
// void DungeonsGame::RenderMainMenu() {}
// void DungeonsGame::RenderUI() {}
// void DungeonsGame::RenderPauseMenu() {}
// void DungeonsGame::DrawButton(...) {}