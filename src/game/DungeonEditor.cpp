#include "DungeonEditor.h"
#include "../engine/Components.h"
#include "../engine/Input.h"
#include "../engine/Math3D.h"
#include "../engine/TextRenderer.h"
#include "../engine/GLTFLoader.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace PixelsEngine {

DungeonEditor::DungeonEditor() {
  m_Hotbar.resize(9);
}

DungeonEditor::~DungeonEditor() {}

void DungeonEditor::Init(Registry *registry, GLRenderer *renderer) {
  m_Registry = registry;
  m_Renderer = renderer;
  ScanAssets();

  bool empty = true;
  for (const auto &item : m_Hotbar) { if (!item.name.empty()) { empty = false; break; } }

  if (empty) {
    auto findAndSet = [&](const std::string &name, int slot) {
      for (const auto &asset : m_Assets) { if (asset.name == name) { m_Hotbar[slot] = asset; return; } }
    };
    findAndSet("wall", 0);
    findAndSet("wall_corner", 1);
    findAndSet("floor_tile_large", 2);
    findAndSet("stairs", 3);
    findAndSet("barrel_small", 4);
    findAndSet("chest", 5);
    findAndSet("Knight", 6);
    findAndSet("Skeleton_Warrior", 7);
  }
}

void DungeonEditor::ScanAssets() {
  m_Assets.clear();
  // Basic Dungeon Assets
  std::string objPath = "assets/dungeons/Assets/obj/";
  if (fs::exists(objPath)) {
    for (const auto &entry : fs::directory_iterator(objPath)) {
      if (entry.path().extension() == ".obj") {
        std::string name = entry.path().stem().string();
        if (m_Renderer) m_Renderer->LoadMesh(name, entry.path().string());
        m_Assets.push_back({name, name, "dungeon"});
      }
    }
  }

  // Common Animations
  std::vector<SkeletalAnimation> advAnims = GLTFLoader::LoadAnimations("assets/adventurers/Animations/gltf/Rig_Medium/Rig_Medium_General.glb");
  std::vector<SkeletalAnimation> skelAnims = GLTFLoader::LoadAnimations("assets/skeletons/Animations/gltf/Rig_Medium/Rig_Medium_General.glb");

  // Add Adventurers (GLB)
  std::string advPath = "assets/adventurers/Characters/gltf/";
  if (fs::exists(advPath)) {
    for (const auto &entry : fs::directory_iterator(advPath)) {
      if (entry.path().extension() == ".glb") {
        std::string name = entry.path().stem().string();
        std::string texName = name + "_tex";
        std::string low = name; std::transform(low.begin(), low.end(), low.begin(), ::tolower);
        std::string texFile = "";
        if (low.find("barbarian") != std::string::npos) texFile = "barbarian_texture.png";
        else if (low.find("knight") != std::string::npos) texFile = "knight_texture.png";
        else if (low.find("mage") != std::string::npos) texFile = "mage_texture.png";
        else if (low.find("ranger") != std::string::npos) texFile = "ranger_texture.png";
        else if (low.find("rogue") != std::string::npos) texFile = "rogue_texture.png";

        if (!texFile.empty()) {
          if (m_Renderer) {
            m_Renderer->LoadTexture(texName, advPath + texFile);
            m_Renderer->LoadMesh(name, entry.path().string());
            RenderMesh* rm = m_Renderer->GetRenderMesh(name);
            if (rm) { rm->animations = advAnims; rm->isSkinned = true; }
          }
          m_Assets.push_back({name, name, texName});
        }
      }
    }
  }

  // Add Skeletons (GLB)
  std::string skelPath = "assets/skeletons/characters/gltf/";
  if (fs::exists(skelPath)) {
    for (const auto &entry : fs::directory_iterator(skelPath)) {
      if (entry.path().extension() == ".glb") {
        std::string name = entry.path().stem().string();
        std::string texName = "skeleton_tex";
        if (m_Renderer) {
          m_Renderer->LoadTexture(texName, skelPath + "skeleton_texture.png");
          m_Renderer->LoadMesh(name, entry.path().string());
          RenderMesh* rm = m_Renderer->GetRenderMesh(name);
          if (rm) { rm->animations = skelAnims; rm->isSkinned = true; }
        }
        m_Assets.push_back({name, name, texName});
      }
    }
  }

  std::sort(m_Assets.begin(), m_Assets.end(), [](const EditorAsset &a, const EditorAsset &b) { return a.name < b.name; });
  m_FilteredAssetIndices.clear();
  for (size_t i = 0; i < m_Assets.size(); ++i) m_FilteredAssetIndices.push_back(i);
}

void DungeonEditor::ToggleEditorMode() {
  m_IsActive = !m_IsActive;
  SDL_SetRelativeMouseMode(m_IsActive ? SDL_TRUE : SDL_FALSE);
}

ActionResult DungeonEditor::Update(float dt, const Camera &cam) {
  if (!m_IsActive) return ActionResult::None;
  if (m_ShowInventory) {
    if (Input::IsKeyPressed(SDL_SCANCODE_BACKSPACE) && m_SearchQuery.length() > 0) { m_SearchQuery.pop_back(); m_ScrollOffset = 0; }
    std::string text = Input::GetTextInput();
    if (!text.empty()) { if (!(m_SearchQuery.empty() && (text == "e" || text == "E"))) m_SearchQuery += text; Input::ClearTextInput(); m_ScrollOffset = 0; }
    if (Input::IsKeyPressed(SDL_SCANCODE_UP)) m_ScrollOffset--;
    if (Input::IsKeyPressed(SDL_SCANCODE_DOWN)) m_ScrollOffset++;
    if (m_ScrollOffset < 0) m_ScrollOffset = 0;
    
    m_FilteredAssetIndices.clear();
    std::string query = m_SearchQuery; std::transform(query.begin(), query.end(), query.begin(), ::tolower);
    for (size_t i = 0; i < m_Assets.size(); ++i) {
      std::string name = m_Assets[i].name; std::transform(name.begin(), name.end(), name.begin(), ::tolower);
      if (name.find(query) != std::string::npos) m_FilteredAssetIndices.push_back(i);
    }
    if (Input::IsKeyPressed(SDL_SCANCODE_E)) m_ShowInventory = false;
    return ActionResult::None;
  }

  if (Input::IsKeyPressed(SDL_SCANCODE_E)) { m_ShowInventory = true; return ActionResult::None; }
  for (int i = 0; i < 9; ++i) if (Input::IsKeyPressed((SDL_Scancode)(SDL_SCANCODE_1 + i))) m_SelectedSlot = i;

  if (Input::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
    if (m_Highlight.active) {
      EditorAsset &asset = m_Hotbar[m_SelectedSlot];
      if (!asset.meshName.empty()) {
        auto e = m_Registry->CreateEntity();
        m_Registry->AddComponent<Transform3DComponent>(e, {m_Highlight.x, m_Highlight.y, m_Highlight.z, m_Highlight.rot, 0});
        m_Registry->AddComponent<MeshComponent>(e, {asset.meshName, asset.textureName, 1.0f, 1.0f, 1.0f});
        
        RenderMesh* rm = m_Renderer->GetRenderMesh(asset.meshName);
        if (rm && rm->isSkinned) {
            int idleIdx = 0;
            for (size_t i = 0; i < rm->animations.size(); ++i) {
                if (rm->animations[i].name.find("Idle_A") != std::string::npos) { idleIdx = (int)i; break; }
            }
            m_Registry->AddComponent<SkeletalAnimationComponent>(e, {idleIdx, (float)(rand()%100)*0.01f, 1.0f});
        }
        return ActionResult::Placed;
      }
    }
  } else if (Input::IsMouseButtonPressed(SDL_BUTTON_RIGHT)) {
    if (m_Highlight.hitEntity != -1) { m_Registry->DestroyEntity(m_Highlight.hitEntity); return ActionResult::Broken; }
  }
  return ActionResult::None;
}

void DungeonEditor::Render3D(GLRenderer *ren) {
  if (!m_IsActive || m_ShowInventory) return;
  if (m_Highlight.active) {
    EditorAsset &asset = m_Hotbar[m_SelectedSlot];
    if (!asset.meshName.empty()) ren->RenderMeshPreview(asset.meshName, asset.textureName, m_Highlight.x, m_Highlight.y, m_Highlight.z, m_Highlight.rot, 0.5f);
  }
}

void DungeonEditor::RenderUI(GLRenderer *ren, TextRenderer *tr, int w, int h) {
  if (!m_IsActive) return;
  if (m_ShowInventory) DrawInventory(ren, tr, w, h);
  DrawHotbar(ren, tr, w, h);
}

void DungeonEditor::DrawHotbar(GLRenderer *ren, TextRenderer *tr, int w, int h) {
  int slots = 9, slotSize = 50, gap = 5;
  int totalW = slots * slotSize + (slots - 1) * gap;
  int startX = (w - totalW) / 2, startY = h - slotSize - 20;
  for (int i = 0; i < slots; ++i) {
    int x = startX + i * (slotSize + gap), y = startY;
    if (i == m_SelectedSlot) ren->DrawRect2D(x - 2, y - 2, slotSize + 4, slotSize + 4, {255, 255, 255, 255});
    ren->DrawRect2D(x, y, slotSize, slotSize, {100, 100, 100, 200});
    if (!m_Hotbar[i].name.empty()) {
      ren->RenderThumbnail(m_Hotbar[i].meshName, m_Hotbar[i].textureName, x + 5, y + 5, slotSize - 10);
      std::string sn = m_Hotbar[i].name; if (sn.length() > 5) sn = sn.substr(0, 4) + ".";
      tr->RenderText(ren, sn, x + 2, y + slotSize - 15, {255, 255, 255, 200});
    }
  }
}

void DungeonEditor::DrawInventory(GLRenderer *ren, TextRenderer *tr, int w, int h) {
  ren->DrawRect2D(0, 0, w, h, {0, 0, 0, 200});
  ren->DrawRect2D(w / 2 - 200, 50, 400, 40, {50, 50, 50, 255});
  tr->RenderText(ren, "Search: " + m_SearchQuery + "_", w / 2 - 190, 60, {255, 255, 255, 255});
  int cols = 4, size = 100, gap = 10;
  int startX = w / 2 - (cols * size + (cols - 1) * gap) / 2, startY = 120;
  for (size_t i = 0; i < m_FilteredAssetIndices.size(); ++i) {
    int idx = m_FilteredAssetIndices[i];
    int r = (int)i / cols, c = (int)i % cols;
    int x = startX + c * (size + gap), y = startY + r * (size + gap);
    ren->DrawRect2D(x, y, size, size, {100, 100, 100, 200});
    ren->RenderThumbnail(m_Assets[idx].meshName, m_Assets[idx].textureName, x + 5, y + 5, size - 10);
  }
}

void DungeonEditor::SaveDungeon(const std::string &f) {
  std::ofstream out(f);
  auto view = m_Registry->View<MeshComponent>();
  for (auto &pair : view) {
    auto *t = m_Registry->GetComponent<Transform3DComponent>(pair.first);
    if (t) out << pair.second.meshName << " " << pair.second.textureName << " " << t->x << " " << t->y << " " << t->z << " " << t->rot << "\n";
  }
}

void DungeonEditor::LoadDungeon(const std::string &f) {
  std::ifstream in(f); if (!in) return;
  std::string m, t; float x, y, z, r;
  while (in >> m >> t >> x >> y >> z >> r) {
    auto e = m_Registry->CreateEntity();
    m_Registry->AddComponent<Transform3DComponent>(e, {x, y, z, r, 0});
    m_Registry->AddComponent<MeshComponent>(e, {m, t, 1, 1, 1});
    RenderMesh* rm = m_Renderer->GetRenderMesh(m);
    if (rm && rm->isSkinned) {
        int idleIdx = 0;
        for (size_t i = 0; i < rm->animations.size(); ++i) { if (rm->animations[i].name.find("Idle_A") != std::string::npos) { idleIdx = (int)i; break; } }
        m_Registry->AddComponent<SkeletalAnimationComponent>(e, {idleIdx, 0, 1});
    }
  }
}

} // namespace PixelsEngine