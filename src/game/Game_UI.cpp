#include "../engine/Components.h"
#include "DungeonsGame.h"
#include <SDL2/SDL.h>

using namespace PixelsEngine;

void DungeonsGame::DrawButton(int x, int y, int w, int h,
                               const std::string &text, bool selected) {
  SDL_Color color =
      selected ? SDL_Color{200, 50, 50, 200} : SDL_Color{50, 50, 50, 200};

  m_GLRenderer.DrawRect2D(x, y, w, h, color);
  m_GLRenderer.DrawRect2D(x, y, w, 1, {255, 255, 255, 255}); // Border Top
  m_GLRenderer.DrawRect2D(x, y + h, w, 1,
                          {255, 255, 255, 255});             // Border Bottom
  m_GLRenderer.DrawRect2D(x, y, 1, h, {255, 255, 255, 255}); // Border Left
  m_GLRenderer.DrawRect2D(x + w, y, 1, h, {255, 255, 255, 255}); // Border Right

  m_TextRenderer->RenderTextCentered(&m_GLRenderer, text, x + w / 2, y + h / 2,
                                     {255, 255, 255, 255});
}

void DungeonsGame::RenderMainMenu() {
  int w = m_Width;
  int h = m_Height;

  // Background (Sky Blue)
  m_GLRenderer.DrawRect2D(0, 0, w, h, {135, 206, 235, 255});

  m_TextRenderer->RenderTextCentered(&m_GLRenderer, "DUNGEON CREATIVE", w / 2,
                                     100, {255, 255, 255, 255});

  int btnW = 240;
  int btnH = 50;
  int startY = 300;
  int gap = 70;

  if (!m_InOptions) {
    DrawButton(w / 2 - btnW / 2, startY, btnW, btnH, "PLAY",
               m_MenuSelection == 0);
    DrawButton(w / 2 - btnW / 2, startY + gap, btnW, btnH, "CREATIVE MODE",
               m_MenuSelection == 1);
    DrawButton(w / 2 - btnW / 2, startY + gap * 2, btnW, btnH, "OPTIONS",
               m_MenuSelection == 2);
    DrawButton(w / 2 - btnW / 2, startY + gap * 3, btnW, btnH, "QUIT",
               m_MenuSelection == 3);
  } else {
    bool isFullscreen =
        SDL_GetWindowFlags(m_Window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
    DrawButton(w / 2 - btnW / 2, startY, btnW, btnH, "FULLSCREEN",
               m_MenuSelection == 0);
    DrawButton(w / 2 - btnW / 2, startY + gap, btnW, btnH, "BACK",
               m_MenuSelection == 1);
  }
}

void DungeonsGame::RenderPauseMenu() {
  int w = m_Width;
  int h = m_Height;

  // Dim background
  m_GLRenderer.DrawRect2D(0, 0, w, h, {0, 0, 0, 150});

  m_TextRenderer->RenderTextCentered(&m_GLRenderer, "PAUSED", w / 2, 100,
                                     {255, 255, 255, 255});

  int btnW = 240;
  int btnH = 50;
  int startY = 200;
  int gap = 60;

  if (!m_InOptions) {
    DrawButton(w / 2 - btnW / 2, startY, btnW, btnH, "RESUME",
               m_MenuSelection == 0);
    DrawButton(w / 2 - btnW / 2, startY + gap, btnW, btnH, "SAVE DUNGEON",
               m_MenuSelection == 1);
    DrawButton(w / 2 - btnW / 2, startY + gap * 2, btnW, btnH, "LOAD DUNGEON",
               m_MenuSelection == 2);
    DrawButton(w / 2 - btnW / 2, startY + gap * 3, btnW, btnH, "OPTIONS",
               m_MenuSelection == 3);
    DrawButton(w / 2 - btnW / 2, startY + gap * 4, btnW, btnH, "MAIN MENU",
               m_MenuSelection == 4);
    DrawButton(w / 2 - btnW / 2, startY + gap * 5, btnW, btnH, "QUIT",
               m_MenuSelection == 5);
  } else {
    DrawButton(w / 2 - btnW / 2, startY, btnW, btnH, "FULLSCREEN",
               m_MenuSelection == 0);
    DrawButton(w / 2 - btnW / 2, startY + gap, btnW, btnH, "BACK",
               m_MenuSelection == 1);
  }
}

void DungeonsGame::RenderUI() {
  int w = m_Width;
  int h = m_Height;
  auto *t = m_Registry.GetComponent<Transform3DComponent>(m_PlayerEntity);
  if (t) {
    std::string tutorial = "";
    if (m_CurrentLevel == 1) {
      if (t->x < 8)
        tutorial =
            "SECTION 1: ARCHERY. Hold Left Click to draw, release to fire.";
      else if (t->x >= 8 && t->x < 20 && t->y < 12)
        tutorial =
            "SECTION 2: PARKOUR. Jump against the mossy wall to Wall Run!";
      else if (t->x > 12 && t->y >= 12)
        tutorial =
            "SECTION 3: GRAPPLE. Right Click (or E+Click) a pillar to Zip!";
    } else if (m_CurrentLevel == 2) {
      tutorial = "LEVEL 2: Wall Run over the Lava to find the hidden target!";
    } else if (m_CurrentLevel == 3) {
      tutorial =
          "LEVEL 3: GRAPPLE GAUNTLET. Zip between pillars to cross the pit!";
    }

    if (!tutorial.empty())

      m_TextRenderer->RenderTextWrappedCentered(&m_GLRenderer, tutorial, w / 2,
                                                50, 600,

                                                {255, 255, 255, 200});
  }

  // m_Crosshair->Render(w / 2 - 16, h / 2 - 16);

  m_GLRenderer.DrawRect2D(w / 2 - 1, h / 2 - 10, 2, 20, {255, 255, 255, 128});

  m_GLRenderer.DrawRect2D(w / 2 - 10, h / 2 - 1, 20, 2, {255, 255, 255, 128});

  auto *weapon = m_Registry.GetComponent<WeaponComponent>(m_PlayerEntity);

  auto *phys = m_Registry.GetComponent<PhysicsComponent>(m_PlayerEntity);

  if (weapon) {

    // Texture *bowTex = weapon->isDrawing ? m_BowDraw.get() : m_BowIdle.get();

    // ... Bow rendering would need GL texture ID ...

    // Draw Power Bar

    if (weapon->isDrawing) {

      float power = std::min(1.0f, weapon->drawTime);

      int barW = 200;

      int barH = 12;

      int barX = w / 2 - barW / 2;

      int barY = h / 2 + 40;

      // Background

      m_GLRenderer.DrawRect2D(barX, barY, barW, barH, {50, 50, 50, 180});

      // Fill

      m_GLRenderer.DrawRect2D(
          barX + 2, barY + 2, (int)((barW - 4) * power), barH - 4,

          {(Uint8)(150 + 105 * power), (Uint8)(255 * (1.0f - power)), 50, 255});
    }
  }

  if (m_HitmarkerTimer > 0) {

    m_GLRenderer.DrawRect2D(w / 2 - 15, h / 2 - 15, 5, 5, {255, 255, 255, 255});

    m_GLRenderer.DrawRect2D(w / 2 + 10, h / 2 - 15, 5, 5, {255, 255, 255, 255});

    m_GLRenderer.DrawRect2D(w / 2 - 15, h / 2 + 10, 5, 5, {255, 255, 255, 255});

    m_GLRenderer.DrawRect2D(w / 2 + 10, h / 2 + 10, 5, 5, {255, 255, 255, 255});
  }

  if (phys) {

    float speed = sqrt(phys->velX * phys->velX + phys->velY * phys->velY);

    std::string speedStr = "SPD: " + std::to_string((int)(speed * 10)) + " UPS";

    m_TextRenderer->RenderText(&m_GLRenderer, speedStr, 20, h - 40,
                               {255, 255, 255, 255});

    if (!phys->isGrounded) {

      m_GLRenderer.DrawRect2D(20, h - 60, 100, 10, {50, 50, 50, 200});

      m_GLRenderer.DrawRect2D(20, h - 60, (int)(phys->wallRunTimer * 20), 10,
                              {50, 200, 255, 255});
    }
  }

  char timerBuf[32];

  snprintf(timerBuf, 32, "TIME: %.2fs", m_RunTimer);

  m_TextRenderer->RenderText(&m_GLRenderer, timerBuf, w - 180, 20,
                             {255, 255, 255, 255});

  std::string targetStr = "TARGETS: " + std::to_string(m_TargetsDestroyed) +

                          "/" + std::to_string(m_TotalTargets);

  m_TextRenderer->RenderText(&m_GLRenderer, targetStr, w - 210, 50,
                             {255, 255, 255, 255});

  if (m_GameFinished) {

    m_GLRenderer.DrawRect2D(0, 0, w, h, {0, 0, 0, 180});

    m_TextRenderer->RenderTextCentered(&m_GLRenderer, "COURSE COMPLETE!", w / 2,
                                       h / 2 - 40,

                                       {255, 200, 50, 255});

    char finalTime[64];

    snprintf(finalTime, 64, "FINAL TIME: %.3fs", m_RunTimer);

    m_TextRenderer->RenderTextCentered(&m_GLRenderer, finalTime, w / 2,
                                       h / 2 + 10,

                                       {255, 255, 255, 255});

    m_TextRenderer->RenderTextCentered(&m_GLRenderer,
                                       "Press SPACE for Next Level", w / 2,

                                       h / 2 + 60, {200, 200, 200, 255});

    m_TextRenderer->RenderTextCentered(&m_GLRenderer, "Press R to Retry", w / 2,

                                       h / 2 + 90, {200, 200, 200, 255});
  }
}
