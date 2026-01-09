#pragma once
#include "../engine/Camera.h"
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/InputController.h"
#include "../engine/Input.h"
#include "../engine/Console.h"
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>

namespace PixelsEngine {

class TextRenderer; // Forward decl

struct EditorAsset {
  std::string name;
  std::string meshName;
  std::string textureName;
};

enum class ActionResult {
  None,
  Placed,
  Broken,
  InteractedDoor,
  InteractedChest
};

class CreativeMode {
public:
  CreativeMode();
  ~CreativeMode();

  void Init(Registry *registry, GLRenderer *renderer, Camera *camera);
  ActionResult Update(float dt, Entity playerEntity);
  void RenderUI(GLRenderer *renderer, TextRenderer *textRenderer, int width,
                int height);
  void RenderWorld(GLRenderer *renderer);

  void ToggleEditorMode();
  void ToggleSaveMenu() { 
    m_ShowSaveMenu = !m_ShowSaveMenu; 
    if(m_ShowSaveMenu) { 
      ScanSavedMaps(); 
      ScanDungeons(); 
      m_MenuState = EditorMenuState::Dungeons; 
      m_MapInputBuffer = (m_CurrentMapName == "untitled") ? "" : m_CurrentMapName;
      m_DungeonInputBuffer = "";
      m_FocusedInput = ActiveInput::MapName; // Default focus to map name
      Input::StartTextInput();
    } else {
      Input::StopTextInput();
    }
  }
  bool IsActive() { return m_IsActive; }
  bool IsInventoryOpen() { return m_ShowInventory || m_ShowSaveMenu || m_ShowMapSelection; }

  // Exit request
  bool RequestedExit() const { return m_RequestedExit; }
  void ClearExitRequest() { m_RequestedExit = false; }

  void SaveDungeon(const std::string &filename);
  void LoadDungeon(const std::string &filename);
  void CreateInitialFloorTile();
  void ShowMapSelectionMenu();
  void HideMapSelectionMenu() { m_ShowMapSelection = false; Input::StopTextInput(); }
  bool IsMapSelectionMenuActive() { return m_ShowMapSelection; }

private:
  enum class EditorMenuState { Dungeons, Levels, MapSelection };
  enum class ActiveInput { None, MapName, DungeonName };
  EditorMenuState m_MenuState = EditorMenuState::Dungeons;
  ActiveInput m_FocusedInput = ActiveInput::None;
  
  void ScanAssets();
  void ScanSavedMaps();
  void ScanDungeons();
  void LoadDungeonLevels(const std::string& dungeonName);
  void SaveDungeonLevels(const std::string& dungeonName);
  
  ActionResult HandleInput();
  void UpdateCursor();
  ActionResult ExecuteAction(bool place);
  void DrawHotbar(GLRenderer *renderer, TextRenderer *textRenderer, int w,
                  int h);
  void DrawInventory(GLRenderer *renderer, TextRenderer *textRenderer, int w,
                     int h);
  void DrawSaveMenu(GLRenderer *renderer, TextRenderer *textRenderer, int w,
                    int h);
  void DrawMapSelectionMenu(GLRenderer *renderer, TextRenderer *textRenderer, int w,
                            int h);

  bool m_IsActive = false;
  bool m_ShowInventory = false;
  bool m_ShowSaveMenu = false;
  bool m_ShowMapSelection = false;
  bool m_RequestedExit = false;

  Registry *m_Registry = nullptr;
  GLRenderer *m_Renderer = nullptr;
  Camera *m_Camera = nullptr;
  std::unique_ptr<FirstPersonMovementController> m_MovementController;
  std::unique_ptr<Console> m_Console;

  std::vector<EditorAsset> m_Assets;
  std::vector<EditorAsset> m_Hotbar;
  int m_SelectedSlot = 0;

  // Inventory & Save State
  std::string m_SearchQuery;
  std::string m_MapInputBuffer;
  std::string m_DungeonInputBuffer;
  std::string m_CurrentMapName = "untitled";
  std::string m_SelectedDungeon = "";
  std::string m_NewMapName = "";
  int m_MapSelectionIdx = 0;
  std::vector<int> m_FilteredAssetIndices;
  std::vector<std::string> m_SavedMaps;
  std::vector<std::string> m_SavedDungeons;
  std::vector<std::string> m_CurrentDungeonLevels;
  int m_ScrollOffset = 0;
  int m_SaveScrollOffset = 0;
  int m_SelectedDungeonIdx = -1;

  // Grid
  float m_GridSize = 4.0f;

  // Breaking state
  float m_BreakProgress = 0.0f;
  Entity m_BreakingEntity = -1;

  // Selection Highlight
  struct {
    float x, y, z;
    float rot;
    Entity hitEntity;
    bool active;

    // Targeted Entity (different from placement pos)
    Entity targetEntity;
    float targetX, targetY, targetZ;
  } m_Highlight;
};

} // namespace PixelsEngine
