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
                                     50, {255, 255, 255, 255});

  int btnW = 240;
  int btnH = 50;
  int startY = 150;
  int gap = 60;

  if (!m_InOptions) {
    DrawButton(w / 2 - btnW / 2, startY, btnW, btnH, "CREATIVE MODE",
               m_MenuSelection == 0);
    DrawButton(w / 2 - btnW / 2, startY + gap, btnW, btnH, "DUNGEON MODE",
               m_MenuSelection == 1);
    DrawButton(w / 2 - btnW / 2, startY + gap * 2, btnW, btnH, "SIDESCROLLER",
               m_MenuSelection == 2);
    DrawButton(w / 2 - btnW / 2, startY + gap * 3, btnW, btnH, "STEALTH MODE",
               m_MenuSelection == 3);
    DrawButton(w / 2 - btnW / 2, startY + gap * 4, btnW, btnH, "SKELETON SIEGE",
               m_MenuSelection == 4);
    DrawButton(w / 2 - btnW / 2, startY + gap * 5, btnW, btnH, "BATTLE MODE",
               m_MenuSelection == 5);
    DrawButton(w / 2 - btnW / 2, startY + gap * 6, btnW, btnH, "OPTIONS",
               m_MenuSelection == 6);
    DrawButton(w / 2 - btnW / 2, startY + gap * 7, btnW, btnH, "QUIT",
               m_MenuSelection == 7);
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

  if (m_State == GameState::Battle && m_BattleMode) {
    m_BattleMode->RenderUI(&m_GLRenderer, m_TextRenderer.get(), w, h);
    if (m_Minimap) {
      m_Minimap->Render(&m_GLRenderer, &m_Registry, m_PlayerEntity, m_Camera.get(), w, h);
    }
    return;
  }

  if (m_State == GameState::Siege && m_SiegeMode) {
    m_SiegeMode->RenderUI(&m_GLRenderer, m_TextRenderer.get(), w, h, m_PlayerEntity);
    // Render minimap for Siege mode
    if (m_Minimap) {
      m_Minimap->Render(&m_GLRenderer, &m_Registry, m_PlayerEntity, m_Camera.get(), w, h);
    }
    return;
  }

  // Only show tutorials and crosshair in Playing state
  if (m_State != GameState::Playing && m_State != GameState::Creative) {
    return;
  }

  auto *t = m_Registry.GetComponent<Transform3DComponent>(m_PlayerEntity);
  if (t && m_State == GameState::Playing) {
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
            "SECTION 3: PARKOUR MASTERY. Show us what you've got!";
    } else if (m_CurrentLevel == 2) {
      tutorial = "LEVEL 2: Wall Run over the Lava to find the hidden target!";
    } else if (m_CurrentLevel == 3) {
      tutorial =
          "LEVEL 3: THE PIT. Don't look down!";
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
  
  // Render minimap (only in Playing state, not in Creative or Paused)
  if (m_State == GameState::Playing && m_Minimap) {
    m_Minimap->Render(&m_GLRenderer, &m_Registry, m_PlayerEntity, m_Camera.get(), w, h);
  }
}
void DungeonsGame::RenderSettings() {
  int w = m_Width;
  int h = m_Height;

  // Background
  m_GLRenderer.DrawRect2D(0, 0, w, h, {50, 50, 50, 255});

  m_TextRenderer->RenderTextCentered(&m_GLRenderer, "SETTINGS", w / 2, 50,
                                     {255, 255, 255, 255});

  int startX = w / 2 - 200;
  int startY = 150;
  int gap = 60;
  GameSettings& settings = GameSettings::Instance();

  // Music Volume
  m_TextRenderer->RenderText(&m_GLRenderer, "Music Volume",
                             startX, startY, {200, 200, 200, 255});
  std::string musicVol = std::to_string((int)(settings.GetMusicVolume() * 100)) + "%";
  m_TextRenderer->RenderText(&m_GLRenderer, musicVol,
                             startX + 250, startY, 
                             m_SettingsSelection == 0 ? SDL_Color{255, 100, 100, 255} : SDL_Color{200, 200, 200, 255});

  // SFX Volume
  m_TextRenderer->RenderText(&m_GLRenderer, "SFX Volume",
                             startX, startY + gap, {200, 200, 200, 255});
  std::string sfxVol = std::to_string((int)(settings.GetSFXVolume() * 100)) + "%";
  m_TextRenderer->RenderText(&m_GLRenderer, sfxVol,
                             startX + 250, startY + gap,
                             m_SettingsSelection == 1 ? SDL_Color{255, 100, 100, 255} : SDL_Color{200, 200, 200, 255});

  // Mouse Sensitivity
  m_TextRenderer->RenderText(&m_GLRenderer, "Mouse Sensitivity",
                             startX, startY + gap * 2, {200, 200, 200, 255});
  std::string mouseSens = std::to_string((int)(settings.GetMouseSensitivity() * 100)) + "%";
  m_TextRenderer->RenderText(&m_GLRenderer, mouseSens,
                             startX + 250, startY + gap * 2,
                             m_SettingsSelection == 2 ? SDL_Color{255, 100, 100, 255} : SDL_Color{200, 200, 200, 255});

  // Difficulty
  m_TextRenderer->RenderText(&m_GLRenderer, "Difficulty",
                             startX, startY + gap * 3, {200, 200, 200, 255});
  m_TextRenderer->RenderText(&m_GLRenderer, settings.GetDifficultyName(),
                             startX + 250, startY + gap * 3,
                             m_SettingsSelection == 3 ? SDL_Color{255, 100, 100, 255} : SDL_Color{200, 200, 200, 255});

  // Back button
  DrawButton(w / 2 - 100, h - 120, 200, 50, "BACK",
             m_SettingsSelection == 4);
}

void DungeonsGame::RenderGameOver() {
  int w = m_Width;
  int h = m_Height;

  // Background
  m_GLRenderer.DrawRect2D(0, 0, w, h, {30, 30, 50, 255});

  // Title
  m_TextRenderer->RenderTextCentered(&m_GLRenderer, "GAME OVER", w / 2, 60,
                                     {255, 100, 100, 255});

  // Stats
  int startY = 180;
  int gap = 50;
  
  // Cap display at total levels
  int displayLevel = std::min(m_LastDungeonStats.currentLevel, m_LastDungeonStats.totalLevels);
  std::string statsText = "Level: " + std::to_string(displayLevel) + 
                          "/" + std::to_string(m_LastDungeonStats.totalLevels);
  m_TextRenderer->RenderTextCentered(&m_GLRenderer, statsText, w / 2, startY,
                                     {200, 200, 200, 255});

  statsText = "Enemies Defeated: " + std::to_string(m_LastDungeonStats.enemiesKilled);
  m_TextRenderer->RenderTextCentered(&m_GLRenderer, statsText, w / 2, startY + gap,
                                     {200, 200, 200, 255});

  statsText = "Time Survived: " + std::to_string((int)m_LastDungeonStats.timeElapsed) + "s";
  m_TextRenderer->RenderTextCentered(&m_GLRenderer, statsText, w / 2, startY + gap * 2,
                                     {200, 200, 200, 255});

  // Buttons
  int btnW = 200;
  int btnH = 50;
  int btnGap = 80;
  int btnStartX = w / 2 - btnW / 2;
  int btnStartY = startY + gap * 4;

  DrawButton(btnStartX - btnW - 20, btnStartY, btnW, btnH, "RETRY",
             m_MenuSelection == 0);
  DrawButton(btnStartX + btnW + 20, btnStartY, btnW, btnH, "MAIN MENU",
             m_MenuSelection == 1);
}

void DungeonsGame::RenderCharacterSelect() {
  int w = m_Width;
  int h = m_Height;

  // Background
  m_GLRenderer.DrawRect2D(0, 0, w, h, {40, 40, 60, 255});

  m_TextRenderer->RenderTextCentered(&m_GLRenderer, "SELECT CHARACTER", w / 2, 60,
                                     {255, 200, 100, 255});

  int btnW = 200;
  int btnH = 60;
  int gap = 100;
  int startX = (w - (btnW * 2 + gap)) / 2;
  int startY = 200;

  // Knight
  DrawButton(startX, startY, btnW, btnH, "KNIGHT",
             m_CharacterSelection == 0 || (m_SelectedCharacter == "Knight" && m_CharacterSelection == 4));

  // Ranger
  DrawButton(startX + btnW + gap, startY, btnW, btnH, "RANGER",
             m_CharacterSelection == 1 || (m_SelectedCharacter == "Ranger" && m_CharacterSelection == 4));

  // Mage
  DrawButton(startX, startY + btnH + 50, btnW, btnH, "MAGE",
             m_CharacterSelection == 2 || (m_SelectedCharacter == "Mage" && m_CharacterSelection == 4));

  // Rogue
  DrawButton(startX + btnW + gap, startY + btnH + 50, btnW, btnH, "ROGUE",
             m_CharacterSelection == 3 || (m_SelectedCharacter == "Rogue" && m_CharacterSelection == 4));

  // Confirm button
  DrawButton(w / 2 - 100, h - 120, 200, 50, "CONFIRM",
             m_CharacterSelection == 4);
  
  // Character preview text on the right
  m_TextRenderer->RenderTextCentered(&m_GLRenderer, m_SelectedCharacter, w - 150, 300,
                                     {200, 200, 255, 255});
  m_TextRenderer->RenderTextCentered(&m_GLRenderer, "SELECTED", w - 150, 350,
                                     {200, 200, 255, 255});
}