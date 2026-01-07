#pragma once
#include "../engine/Camera.h"
#include "../engine/ECS.h"
#include "../engine/GLRenderer.h"
#include "../engine/InputController.h"
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
  bool IsActive() { return m_IsActive; }
  bool IsInventoryOpen() { return m_ShowInventory; }

  void SaveDungeon(const std::string &filename);
  void LoadDungeon(const std::string &filename);

private:
  void ScanAssets();
  ActionResult HandleInput();
  void UpdateCursor();
  ActionResult ExecuteAction(bool place);
  void DrawHotbar(GLRenderer *renderer, TextRenderer *textRenderer, int w,
                  int h);
  void DrawInventory(GLRenderer *renderer, TextRenderer *textRenderer, int w,
                     int h);

  bool m_IsActive = false;
  bool m_ShowInventory = false;

  Registry *m_Registry = nullptr;
  GLRenderer *m_Renderer = nullptr;
  Camera *m_Camera = nullptr;
  std::unique_ptr<FirstPersonMovementController> m_MovementController;

  std::vector<EditorAsset> m_Assets;
  std::vector<EditorAsset> m_Hotbar;
  int m_SelectedSlot = 0;

  // Inventory State
  std::string m_SearchQuery;
  std::vector<int> m_FilteredAssetIndices;
  int m_ScrollOffset = 0;

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
