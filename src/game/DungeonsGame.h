#pragma once
#include "../engine/Application.h"
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/Map.h"
#include "../engine/Raycaster.h"
#include "../engine/TextRenderer.h"
#include "BattleMode.h"
#include "CreativeMode.h"
#include "ExplorationMode.h"
#include "SiegeMode.h"
#include <memory>

enum class GameState { MainMenu, Playing, Creative, Siege, Battle, Paused };

class DungeonsGame : public PixelsEngine::Application {
public:
  DungeonsGame();
  ~DungeonsGame();

protected:
  void OnStart() override;
  void OnUpdate(float deltaTime) override;
  void OnRender() override;

private:
  // Modular systems
  void InitGame();
  void InitSiege();
  void InitBattle();
  void InitMainMenu();

  void UpdateMainMenu(float dt);
  void UpdateGameplay(float dt);
  void UpdateSiege(float dt);
  void UpdateBattle(float dt);

  void RenderMainMenu();
  void RenderGameplay();
  void RenderCreative();
  void RenderUI();
  void RenderPauseMenu();

  void HandleInputGameplay(float dt);
  void HandleInputCreative(float dt);
  void HandleInputMenu();
  void HandleInputPause();

  void UpdatePhysics(float dt);
  void UpdateDoors(float dt);
  void UpdateProjectiles(float dt);
  void UpdateAnimations(float dt);

  // UI Helpers
  void DrawButton(int x, int y, int w, int h, const std::string &text,
                  bool selected);

  PixelsEngine::GLRenderer m_GLRenderer;
  PixelsEngine::Raycaster m_Raycaster;
  PixelsEngine::Map m_Map;
  std::unique_ptr<PixelsEngine::TextRenderer> m_TextRenderer;

  PixelsEngine::CreativeMode m_CreativeMode;
  std::unique_ptr<SiegeMode> m_SiegeMode;
  std::unique_ptr<ExplorationMode> m_ExplorationMode;
  std::unique_ptr<BattleMode> m_BattleMode;

  std::shared_ptr<PixelsEngine::Texture> m_BowIdle;
  std::shared_ptr<PixelsEngine::Texture> m_BowDraw;
  std::shared_ptr<PixelsEngine::Texture> m_Crosshair;

  // Sounds
  Mix_Chunk *m_SfxShoot = nullptr;
  Mix_Chunk *m_SfxHit = nullptr;
  Mix_Chunk *m_SfxJump = nullptr;
  Mix_Chunk *m_SfxGrapple = nullptr;
  Mix_Chunk *m_SfxDoor = nullptr;
  Mix_Chunk *m_SfxChestOpen = nullptr;
  Mix_Music *m_Ambience = nullptr;

  PixelsEngine::Entity m_PlayerEntity;

  GameState m_State = GameState::MainMenu;
  GameState m_PreviousState = GameState::MainMenu; // Store state before pause
  int m_MenuSelection = 0; // 0: Play/Resume, 1: Options, 2: Quit/MainMenu
  bool m_InOptions = false;

  // Gameplay Stats & Juice
  float m_HitmarkerTimer = 0.0f;
  float m_CameraRoll = 0.0f; // Degrees
  float m_RunTimer = 0.0f;
  bool m_GameFinished = false;
  int m_TargetsDestroyed = 0;
  int m_TotalTargets = 0;
  int m_CurrentLevel = 1;

  // Main Menu Camera Props
  float m_MenuCamAngle = 0.0f;
  float m_TimeScale = 1.0f;
  float m_BobTimer = 0.0f;
  float m_SwayTimer = 0.0f;

  // Camera Tuner
  float m_TunerDist = 2.22f;
  float m_TunerShoulder = -0.662f;
  float m_TunerHeight = 1.724f;

  // Screenshake
  float m_ShakeIntensity = 0.0f;
  float m_ShakeTimer = 0.0f;

  bool m_IsGrappling = false;
  struct {
    float x, y, z;
  } m_GrapplePoint;

  // Spatial Audio Helper
  void PlaySpatialSfx(Mix_Chunk *chunk, float x, float y, float z);
};