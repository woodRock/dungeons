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
  // Default hotbar slots (empty)
  m_Hotbar.resize(9);
}

DungeonEditor::~DungeonEditor() {}

void DungeonEditor::Init(Registry *registry, GLRenderer *renderer) {
  m_Registry = registry;
  m_Renderer = renderer;
  ScanAssets();

  // Only fill hotbar with defaults if it's empty (first init)
  bool empty = true;
  for (const auto &item : m_Hotbar) {
    if (!item.name.empty()) {
      empty = false;
      break;
    }
  }

  if (empty) {
    auto findAndSet = [&](const std::string &name, int slot) {
      for (const auto &asset : m_Assets) {
        if (asset.name == name) {
          m_Hotbar[slot] = asset;
          return;
        }
      }
    };
    findAndSet("wall", 0);
    findAndSet("wall_corner", 1);
    findAndSet("floor_tile_large", 2);
    findAndSet("stairs", 3);
    findAndSet("wall_doorway", 4);
    findAndSet("wall_doorway_door", 5);
    findAndSet("chest", 6);
    findAndSet("Knight", 7);
    findAndSet("Skeleton_Warrior", 8);
  }
}

void DungeonEditor::ScanAssets() {
  m_Assets.clear();

  // 1. Basic Dungeon Assets (OBJ)
  std::string objPath = "assets/dungeons/Assets/obj/";
  if (fs::exists(objPath)) {
    for (const auto &entry : fs::directory_iterator(objPath)) {
      if (entry.path().extension() == ".obj") {
        std::string meshName = entry.path().stem().string();
        if (m_Renderer) {
          m_Renderer->LoadMesh(meshName, entry.path().string());
        }
        m_Assets.push_back({meshName, meshName, "dungeon"});
      }
    }
  }

  // Common Animations for skinned meshes
  std::vector<SkeletalAnimation> advAnims = GLTFLoader::LoadAnimations("assets/adventurers/Animations/gltf/Rig_Medium/Rig_Medium_General.glb");
  std::vector<SkeletalAnimation> skelAnims = GLTFLoader::LoadAnimations("assets/skeletons/Animations/gltf/Rig_Medium/Rig_Medium_General.glb");

  // 2. Adventurers (GLB)
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
            m_Renderer->LoadTexture(texName, "assets/adventurers/Textures/" + texFile);
            m_Renderer->LoadMesh(name, entry.path().string());
            RenderMesh* rm = m_Renderer->GetRenderMesh(name);
            if (rm) { rm->animations = advAnims; rm->isSkinned = true; }
          }
          m_Assets.push_back({name, name, texName});
        }
      }
    }
  }

  // 3. Skeletons (GLB)
  std::string skelPath = "assets/skeletons/characters/gltf/";
  if (fs::exists(skelPath)) {
    for (const auto &entry : fs::directory_iterator(skelPath)) {
      if (entry.path().extension() == ".glb") {
        std::string name = entry.path().stem().string();
        std::string texName = "skeleton_tex";
        if (m_Renderer) {
          m_Renderer->LoadTexture(texName, "assets/skeletons/texture/skeleton_texture.png");
          m_Renderer->LoadMesh(name, entry.path().string());
          RenderMesh* rm = m_Renderer->GetRenderMesh(name);
          if (rm) { rm->animations = skelAnims; rm->isSkinned = true; }
        }
        m_Assets.push_back({name, name, texName});
      }
    }
  }

  // Sort alphabetically
  std::sort(m_Assets.begin(), m_Assets.end(),
            [](const EditorAsset &a, const EditorAsset &b) {
              return a.name < b.name;
            });

  m_FilteredAssetIndices.clear();
  for (size_t i = 0; i < m_Assets.size(); ++i)
    m_FilteredAssetIndices.push_back(i);
}

void DungeonEditor::ToggleEditorMode() {
  m_IsActive = !m_IsActive;
  if (m_IsActive) {
    SDL_SetRelativeMouseMode(SDL_TRUE);
  } else {
    SDL_SetRelativeMouseMode(SDL_FALSE);
  }
}

ActionResult DungeonEditor::Update(float dt, const Camera &cam) {
  if (!m_IsActive)
    return ActionResult::None;

  if (m_ShowInventory) {
    // ... search logic ...
    if (Input::IsKeyPressed(SDL_SCANCODE_BACKSPACE) &&
        m_SearchQuery.length() > 0) {
      m_SearchQuery.pop_back();
      m_ScrollOffset = 0;
    }
    std::string text = Input::GetTextInput();
    if (!text.empty()) {
      if (m_SearchQuery.empty() && (text == "e" || text == "E")) {
      } else {
        m_SearchQuery += text;
      }
      Input::ClearTextInput();
      m_ScrollOffset = 0;
    }

    if (Input::IsKeyPressed(SDL_SCANCODE_UP))
      m_ScrollOffset--;
    if (Input::IsKeyPressed(SDL_SCANCODE_DOWN))
      m_ScrollOffset++;

    if (m_ScrollOffset < 0)
      m_ScrollOffset = 0;
    int cCount = 4;
    int totalRows = (int)(m_FilteredAssetIndices.size() + cCount - 1) / cCount;
    if (m_ScrollOffset > totalRows - 1)
      m_ScrollOffset = totalRows - 1;
    if (m_ScrollOffset < 0)
      m_ScrollOffset = 0;

    m_FilteredAssetIndices.clear();
    std::string query = m_SearchQuery;
    std::transform(query.begin(), query.end(), query.begin(), ::tolower);
    for (size_t i = 0; i < m_Assets.size(); ++i) {
      std::string name = m_Assets[i].name;
      std::transform(name.begin(), name.end(), name.begin(), ::tolower);
      if (name.find(query) != std::string::npos) {
        m_FilteredAssetIndices.push_back(i);
      }
    }

    if (Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
      m_ShowInventory = false;
      Input::StopTextInput();
      SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    return ActionResult::None;
  } else {
    UpdateCursor(cam);
    
    // Hold to Break logic
    bool breaking = Input::IsMouseButtonDown(SDL_BUTTON_LEFT) && 
                   !Input::IsKeyDown(SDL_SCANCODE_LCTRL) && 
                   !Input::IsKeyDown(SDL_SCANCODE_LGUI);
    
    if (breaking && m_Highlight.targetEntity != -1) {
        if (m_BreakingEntity != m_Highlight.targetEntity) {
            m_BreakingEntity = m_Highlight.targetEntity;
            m_BreakProgress = 0.0f;
        }
        m_BreakProgress += dt * 2.0f; // 0.5s to break
    } else {
        m_BreakingEntity = -1;
        m_BreakProgress = 0.0f;
    }

    ActionResult res = HandleInput(cam);

    if (Input::IsKeyPressed(SDL_SCANCODE_E)) {
      m_ShowInventory = true;
      Input::StartTextInput();
      Input::ClearKeys();
      SDL_SetRelativeMouseMode(SDL_FALSE);
    }

    if (Input::IsKeyPressed(SDL_SCANCODE_1))
      m_SelectedSlot = 0;
    if (Input::IsKeyPressed(SDL_SCANCODE_2))
      m_SelectedSlot = 1;
    if (Input::IsKeyPressed(SDL_SCANCODE_3))
      m_SelectedSlot = 2;
    if (Input::IsKeyPressed(SDL_SCANCODE_4))
      m_SelectedSlot = 3;
    if (Input::IsKeyPressed(SDL_SCANCODE_5))
      m_SelectedSlot = 4;
    if (Input::IsKeyPressed(SDL_SCANCODE_6))
      m_SelectedSlot = 5;
    if (Input::IsKeyPressed(SDL_SCANCODE_7))
      m_SelectedSlot = 6;
    if (Input::IsKeyPressed(SDL_SCANCODE_8))
      m_SelectedSlot = 7;
    if (Input::IsKeyPressed(SDL_SCANCODE_9))
      m_SelectedSlot = 8;
    
    return res;
  }
}

ActionResult DungeonEditor::HandleInput(const Camera &cam) {
  bool ctrlPressed = Input::IsKeyDown(SDL_SCANCODE_LCTRL) ||
                     Input::IsKeyDown(SDL_SCANCODE_RCTRL) ||
                     Input::IsKeyDown(SDL_SCANCODE_LGUI) ||
                     Input::IsKeyDown(SDL_SCANCODE_RGUI);

  if (m_BreakProgress >= 1.0f) {
      m_BreakProgress = 0.0f;
      return ExecuteAction(false); // Break
  }

  if (Input::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
    if (ctrlPressed) {
      return ExecuteAction(true); // Place
    }
  }

  if (Input::IsMouseButtonPressed(SDL_BUTTON_RIGHT)) {
    return ExecuteAction(true); // Place
  }

  // Pick Block (Q)
  if (Input::IsKeyPressed(SDL_SCANCODE_Q)) {
    if (m_Highlight.active) {
      if (m_Highlight.hitEntity != -1) {
        auto *m = 
            m_Registry->GetComponent<MeshComponent>(m_Highlight.hitEntity);
        if (m) {
          for (const auto &asset : m_Assets) {
            if (asset.meshName == m->meshName) {
              m_Hotbar[m_SelectedSlot] = asset;
              goto q_done;
            }
          }
        }
      }
      if (m_Highlight.z <= 0.5f) {
        for (const auto &asset : m_Assets) {
          if (asset.meshName == "floor_tile_large") {
            m_Hotbar[m_SelectedSlot] = asset;
            break;
          }
        }
      }
    }
  }

q_done:
  return ActionResult::None;
}

void DungeonEditor::UpdateCursor(const Camera &cam) {
  float dirX = cos(cam.yaw) * cos(cam.pitch);
  float dirY = sin(cam.yaw) * cos(cam.pitch);
  float dirZ = sin(cam.pitch);

  float x = cam.x;
  float y = cam.y;
  float z = cam.z;

  float step = 0.1f; // Smaller step for precision
  float dist = 0.0f;
  float maxDist = 20.0f;

  bool hit = false;
  Entity hitEntity = -1;
  float hitX, hitY, hitZ;

  while (dist < maxDist) {
    x += dirX * step;
    y += dirY * step;
    z += dirZ * step;
    dist += step;

    // 1. Check existing meshes (First priority)
    auto &view = m_Registry->View<MeshComponent>();
    bool meshHit = false;
    for (auto &pair : view) {
      auto *t = m_Registry->GetComponent<Transform3DComponent>(pair.first);
      if (t) {
        float dx = t->x - x;
        float dy = t->y - y;
        float dz = t->z - z;
        // Accurate targeting: use a larger radius to cover the 4x4 block
        if (sqrt(dx * dx + dy * dy + dz * dz) < 2.2f) {
          hit = true;
          hitEntity = pair.first;
          meshHit = true;
          hitX = t->x; hitY = t->y; hitZ = t->z;
          break;
        }
      }
    }
    if (meshHit)
      break;

    // 2. Check ground plane (Z=0)
    if (z <= 0.1f && z >= -0.1f) {
      hit = true;
      hitX = x; hitY = y; hitZ = z;
      break;
    }
  }

  if (hit) {
    // Found placement position (small step back from hit point)
    float prevX = x - dirX * 0.05f;
    float prevY = y - dirY * 0.05f;
    float prevZ = z - dirZ * 0.05f;

    float snapSize = m_GridSize;
    bool isDoor = false;
    bool isScaffoldDoor = false;

    if (m_SelectedSlot >= 0 && m_SelectedSlot < m_Hotbar.size()) {
        std::string name = m_Hotbar[m_SelectedSlot].meshName;
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        isDoor = (name == "wall_doorway_door");
        isScaffoldDoor = (name == "wall_doorway_scaffold_door");
        if (name.find("small") != std::string::npos || name.find("corner") != std::string::npos) {
            snapSize = m_GridSize / 4.0f;
        }
    }

    m_Highlight.x = round(prevX / snapSize) * snapSize;
    m_Highlight.y = round(prevY / snapSize) * snapSize;
    
    // Fix sinking: Ensure Z is snapped to grid but at least 0
    m_Highlight.z = round(prevZ / snapSize) * snapSize;
    if (m_Highlight.z < 0) m_Highlight.z = 0;

    // Calculate Rotation: Map to 4 cardinal directions (Face Player)
    float yaw = cam.yaw;
    while (yaw < 0) yaw += M_PI * 2.0f;
    while (yaw >= M_PI * 2.0f) yaw -= M_PI * 2.0f;

    if (yaw >= M_PI * 0.25f && yaw < M_PI * 0.75f) m_Highlight.rot = 0.0f; // Looking North
    else if (yaw >= M_PI * 0.75f && yaw < M_PI * 1.25f) m_Highlight.rot = M_PI * 1.5f; // Looking West
    else if (yaw >= M_PI * 1.25f && yaw < M_PI * 1.75f) m_Highlight.rot = M_PI; // Looking South
    else m_Highlight.rot = M_PI * 0.5f; // Looking East

    m_Highlight.hitEntity = hitEntity; 
    m_Highlight.active = true;
    
    // Targeted Entity state
    m_Highlight.targetEntity = hitEntity;
    m_Highlight.targetX = hitX;
    m_Highlight.targetY = hitY;
    m_Highlight.targetZ = hitZ;

    // Snapping Logic for Doors
    if (isDoor || isScaffoldDoor) {
        bool snapped = false;
        if (hitEntity != -1) {
            auto* hitMesh = m_Registry->GetComponent<MeshComponent>(hitEntity);
            if (hitMesh) {
                std::string hitName = hitMesh->meshName;
                std::transform(hitName.begin(), hitName.end(), hitName.begin(), ::tolower);

                bool match = (isDoor && hitName == "wall_doorway") || 
                             (isScaffoldDoor && hitName == "wall_doorway_scaffold");
                
                if (match) {
                    auto* t = m_Registry->GetComponent<Transform3DComponent>(hitEntity);
                    if (t) {
                        m_Highlight.x = t->x;
                        m_Highlight.y = t->y;
                        m_Highlight.z = t->z;
                        
                        // Normalize rotation logic to match my_dungeon.map style
                        // (Door always 0 for X-aligned walls, presumably 90 for Y-aligned)
                        float rot = t->rot;
                        while(rot < 0) rot += M_PI * 2.0f;
                        while(rot >= M_PI * 2.0f) rot -= M_PI * 2.0f;

                        // Check alignment (allow some epsilon)
                        bool isXAligned = (rot < 0.1f || rot > M_PI * 2.0f - 0.1f || (rot > M_PI - 0.1f && rot < M_PI + 0.1f));

                        if (isXAligned) {
                            m_Highlight.rot = 0.0f;
                        } else {
                            m_Highlight.rot = rot;
                            float yOffset = m_GridSize / 6.0f;
                            float xOffset = 0.7f;

                            if (rot < M_PI) {
                                m_Highlight.y += yOffset;
                                m_Highlight.x += xOffset;
                            } else {
                                m_Highlight.y -= yOffset;
                                m_Highlight.x += xOffset;
                            }
                        }

                        snapped = true;
                    }
                }
            }
        }
    }

  } else {
    m_Highlight.active = false;
    m_Highlight.targetEntity = -1;
  }
}

ActionResult DungeonEditor::ExecuteAction(bool place) {
  if (!m_Highlight.active)
    return ActionResult::None;

  if (place) {
    if (m_Highlight.hitEntity != -1) {
        if (m_Registry->HasComponent<InteractableComponent>(m_Highlight.hitEntity)) {
            auto* inter = m_Registry->GetComponent<InteractableComponent>(m_Highlight.hitEntity);
            auto* trans = m_Registry->GetComponent<Transform3DComponent>(m_Highlight.hitEntity);
            
            inter->isOpen = !inter->isOpen;
            if (inter->isOpen) {
                trans->rot += M_PI * 0.5f;
            } else {
                trans->rot -= M_PI * 0.5f;
            }
            
            if (inter->type == InteractableComponent::Door) return ActionResult::InteractedDoor;
            return ActionResult::InteractedChest;
        } else {
            // Search nearby for an interactable entity (e.g. door inside a frame)
            auto* t = m_Registry->GetComponent<Transform3DComponent>(m_Highlight.hitEntity);
            if (t) {
                auto& view = m_Registry->View<InteractableComponent>();
                for (auto& pair : view) {
                    auto* doorTrans = m_Registry->GetComponent<Transform3DComponent>(pair.first);
                    if (doorTrans) {
                        float dx = doorTrans->x - t->x;
                        float dy = doorTrans->y - t->y;
                        float dz = doorTrans->z - t->z;
                        if (sqrt(dx*dx + dy*dy + dz*dz) < 2.5f) {
                            auto* inter = &pair.second;
                            inter->isOpen = !inter->isOpen;
                            if (inter->isOpen) {
                                doorTrans->rot += M_PI * 0.5f;
                            } else {
                                doorTrans->rot -= M_PI * 0.5f;
                            }
                            if (inter->type == InteractableComponent::Door) return ActionResult::InteractedDoor;
                            return ActionResult::InteractedChest;
                        }
                    }
                }
            }
        }
    }

    EditorAsset &asset = m_Hotbar[m_SelectedSlot];

    if (!asset.meshName.empty()) {
      auto e = m_Registry->CreateEntity();

      m_Registry->AddComponent<Transform3DComponent>(
          e, {m_Highlight.x, m_Highlight.y, m_Highlight.z, m_Highlight.rot, 0});

      m_Registry->AddComponent<MeshComponent>(
          e, {asset.meshName, asset.textureName, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f});
      
      std::string lowName = asset.meshName;
      std::transform(lowName.begin(), lowName.end(), lowName.begin(), ::tolower);
      bool isActualDoor = (lowName == "wall_doorway_door" || lowName == "wall_doorway_scaffold_door");

      if (isActualDoor) {
          m_Registry->AddComponent<InteractableComponent>(e, {InteractableComponent::Door, false});
          auto* mc = m_Registry->GetComponent<MeshComponent>(e);
          mc->offsetX = -(m_GridSize / 6.0f);
      } else if (lowName.find("chest") != std::string::npos) {
          m_Registry->AddComponent<InteractableComponent>(e, {InteractableComponent::Chest, false});
      }

      // Add Animation for skinned meshes (Characters)
      RenderMesh* rm = m_Renderer->GetRenderMesh(asset.meshName);
      if (rm && rm->isSkinned && !rm->animations.empty()) {
          int idleIdx = 0;
          for (size_t i = 0; i < rm->animations.size(); ++i) {
              if (rm->animations[i].name.find("Idle") != std::string::npos) {
                  idleIdx = (int)i;
                  break;
              }
          }
          m_Registry->AddComponent<SkeletalAnimationComponent>(e, {idleIdx, (float)(rand()%100)*0.01f, 1.0f});
      }

      return ActionResult::Placed;
    }
  } else {
    if (m_Highlight.hitEntity != -1) {
      m_Registry->DestroyEntity(m_Highlight.hitEntity);
      m_Highlight.hitEntity = -1;
      return ActionResult::Broken;
    }
  }
  return ActionResult::None;
}

void DungeonEditor::Render3D(GLRenderer *ren) {
  if (!m_IsActive || m_ShowInventory)
    return;

  // 1. Draw Selection Border around targeted entity
  if (m_Highlight.targetEntity != -1) {
      auto* t = m_Registry->GetComponent<Transform3DComponent>(m_Highlight.targetEntity);
      if (t) {
          // Draw a white border
          ren->DrawWireCube(t->x, t->y, t->z, 4.0f, {255, 255, 255, 255});
          
          // 2. Breaking visual feedback (Transparency)
          if (m_BreakingEntity == m_Highlight.targetEntity) {
              auto* m = m_Registry->GetComponent<MeshComponent>(m_Highlight.targetEntity);
              if (m) {
                  // Render a transparent version over it or just rely on the fact that we can't easily hide the original
                  // For now, render a "cracking" ghost
                  ren->RenderMeshPreview(m->meshName, m->textureName, t->x, t->y, t->z, t->rot, 1.0f - m_BreakProgress);
              }
          }
      }
  }

  // 3. Placement Preview
  if (m_Highlight.active) {
    EditorAsset &asset = m_Hotbar[m_SelectedSlot];
    if (!asset.meshName.empty()) {
      float ox = 0;
      std::string ln = asset.meshName;
      std::transform(ln.begin(), ln.end(), ln.begin(), ::tolower);
      if (ln == "wall_doorway_door" || ln == "wall_doorway_scaffold_door") ox = -(m_GridSize / 6.0f);

      ren->RenderMeshPreview(asset.meshName, asset.textureName, m_Highlight.x,
                             m_Highlight.y, m_Highlight.z, m_Highlight.rot,
                             0.5f, ox, 0, 0);
    }
  }
}

void DungeonEditor::RenderUI(GLRenderer *ren, TextRenderer *tr, int w, int h) {
  if (!m_IsActive)
    return;

  if (m_ShowInventory) {
    DrawInventory(ren, tr, w, h);
  }

  DrawHotbar(ren, tr, w, h);

  if (!m_ShowInventory) {
    // Instruction
    tr->RenderText(ren, "E: Inventory | 1-9: Select Slot | Click: Place/Break",
                   20, 20, {255, 255, 255, 255});

    // Crosshair
    ren->DrawRect2D(w / 2 - 10, h / 2, 20, 1, {255, 255, 255, 128});
    ren->DrawRect2D(w / 2, h / 2 - 10, 1, 20, {255, 255, 255, 128});
  }
}

void DungeonEditor::DrawHotbar(GLRenderer *ren, TextRenderer *tr, int w,
                               int h) {
  int slots = 9;
  int slotSize = 50;
  int gap = 5;
  int totalW = slots * slotSize + (slots - 1) * gap;
  int startX = (w - totalW) / 2;
  int startY = h - slotSize - 20;

  for (int i = 0; i < slots; ++i) {
    int x = startX + i * (slotSize + gap);
    int y = startY;

    if (i == m_SelectedSlot) {
      ren->DrawRect2D(x - 2, y - 2, slotSize + 4, slotSize + 4,
                      {255, 255, 255, 255});
    }

    ren->DrawRect2D(x, y, slotSize, slotSize, {100, 100, 100, 200});

    if (!m_Hotbar[i].name.empty()) {
      ren->RenderThumbnail(m_Hotbar[i].meshName, m_Hotbar[i].textureName, x + 5,
                           y + 5, slotSize - 10);

      std::string shortName = m_Hotbar[i].name;
      if (shortName.length() > 5)
        shortName = shortName.substr(0, 4) + ".";
      tr->RenderText(ren, shortName, x + 2, y + slotSize - 15,
                     {255, 255, 255, 200});
    }
  }
}

void DungeonEditor::DrawInventory(GLRenderer *ren, TextRenderer *tr, int w,
                                  int h) {
  // Dim background
  ren->DrawRect2D(0, 0, w, h, {0, 0, 0, 200});

  // Search Box
  ren->DrawRect2D(w / 2 - 200, 50, 400, 40, {50, 50, 50, 255});
  tr->RenderText(ren, "Search: " + m_SearchQuery + "_", w / 2 - 190, 60,
                 {255, 255, 255, 255});

  // Grid of assets
  int cols = 4;
  int startX = w / 2 - 300;
  int startY = 120;
  int btnW = 140;
  int btnH = 40;
  int pad = 10;

  int mx, my;
  Input::GetMousePosition(mx, my);
  bool clicked = Input::IsMouseButtonPressed(SDL_BUTTON_LEFT);

  int maxRows = 10; // Number of rows to show
  int startIndex = m_ScrollOffset * cols;

  std::string tooltip = "";

  for (size_t i = startIndex;
       i < m_FilteredAssetIndices.size() && (i - startIndex) < (cols * maxRows);
       ++i) {
    int assetIdx = m_FilteredAssetIndices[i];
    int displayIdx = (int)(i - startIndex);
    int row = displayIdx / cols;
    int col = displayIdx % cols;

    int x = startX + col * (btnW + pad);
    int y = startY + row * (btnH + pad);

    bool hover = (mx >= x && mx <= x + btnW && my >= y && my <= y + btnH);

    SDL_Color c = {100, 100, 100, 255};
    if (hover) {
      c = {150, 150, 150, 255};
      tooltip = m_Assets[assetIdx].name;

      if (Input::IsKeyPressed(SDL_SCANCODE_1)) m_Hotbar[0] = m_Assets[assetIdx];
      if (Input::IsKeyPressed(SDL_SCANCODE_2)) m_Hotbar[1] = m_Assets[assetIdx];
      if (Input::IsKeyPressed(SDL_SCANCODE_3)) m_Hotbar[2] = m_Assets[assetIdx];
      if (Input::IsKeyPressed(SDL_SCANCODE_4)) m_Hotbar[3] = m_Assets[assetIdx];
      if (Input::IsKeyPressed(SDL_SCANCODE_5)) m_Hotbar[4] = m_Assets[assetIdx];
      if (Input::IsKeyPressed(SDL_SCANCODE_6)) m_Hotbar[5] = m_Assets[assetIdx];
      if (Input::IsKeyPressed(SDL_SCANCODE_7)) m_Hotbar[6] = m_Assets[assetIdx];
      if (Input::IsKeyPressed(SDL_SCANCODE_8)) m_Hotbar[7] = m_Assets[assetIdx];
      if (Input::IsKeyPressed(SDL_SCANCODE_9)) m_Hotbar[8] = m_Assets[assetIdx];
    }

    ren->DrawRect2D(x, y, btnW, btnH, c);

    std::string displayName = m_Assets[assetIdx].name;
    if (displayName.length() > 15)
      displayName = displayName.substr(0, 12) + "...";

    tr->RenderText(ren, displayName, x + 5, y + 10, {255, 255, 255, 255});

    if (clicked && hover) {
      m_Hotbar[m_SelectedSlot] = m_Assets[assetIdx];
      // m_ShowInventory = false; // Don't close immediately so user can fill
      // multiple slots Input::StopTextInput();
      // SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    ren->RenderThumbnail(m_Assets[assetIdx].meshName,
                         m_Assets[assetIdx].textureName, x + btnW - 35, y + 5,
                         30);
  }

  if (!tooltip.empty()) {
    ren->DrawRect2D(mx + 15, my + 15, (int)tooltip.length() * 12, 30,
                    {20, 20, 20, 230});
    tr->RenderText(ren, tooltip, mx + 20, my + 20, {255, 255, 100, 255});
  }
}

void DungeonEditor::SaveDungeon(const std::string &filename) {
  std::ofstream out(filename);
  if (!out.is_open())
    return;

  auto &view = m_Registry->View<MeshComponent>();
  for (auto &pair : view) {
    if (!m_Registry->HasComponent<Transform3DComponent>(pair.first))
      continue;
    auto *t = m_Registry->GetComponent<Transform3DComponent>(pair.first);
    auto *m = &pair.second;

    // Format: MeshName TextureName X Y Z Rot
    out << m->meshName << " " << m->textureName << " " << t->x << " " << t->y
        << " " << t->z << " " << t->rot << "\n";
  }
  out.close();
  std::cout << "Saved dungeon to " << filename << std::endl;
}

void DungeonEditor::LoadDungeon(const std::string &filename) {
  std::ifstream in(filename);
  if (!in.is_open())
    return;

  // Clear existing mesh entities
  std::vector<Entity> toKill;
  auto &view = m_Registry->View<MeshComponent>();
  for (auto &pair : view)
    toKill.push_back(pair.first);
  for (auto e : toKill)
    m_Registry->DestroyEntity(e);

  std::string meshName, texName;
  float x, y, z, rot;

  while (in >> meshName >> texName >> x >> y >> z >> rot) {
    auto e = m_Registry->CreateEntity();
    m_Registry->AddComponent<Transform3DComponent>(e, {x, y, z, rot, 0.0f});
    m_Registry->AddComponent<
        MeshComponent>(
        e,
        {meshName, texName, 1.0f, 1.0f, 1.0f});

    std::string lowName = meshName;
    std::transform(lowName.begin(), lowName.end(), lowName.begin(), ::tolower);
    bool isActualDoor = (lowName == "wall_doorway_door" || lowName == "wall_doorway_scaffold_door");

    if (isActualDoor) {
        m_Registry->AddComponent<InteractableComponent>(e, {InteractableComponent::Door, false});
        auto* mc = m_Registry->GetComponent<MeshComponent>(e);
        mc->offsetX = -(m_GridSize / 6.0f);
    } else if (lowName.find("chest") != std::string::npos) {
        m_Registry->AddComponent<InteractableComponent>(e, {InteractableComponent::Chest, false});
    }

    // Restore animations for skinned meshes
    RenderMesh* rm = m_Renderer->GetRenderMesh(meshName);
    if (rm && rm->isSkinned && !rm->animations.empty()) {
        int idleIdx = 0;
        for (size_t i = 0; i < rm->animations.size(); ++i) {
            if (rm->animations[i].name.find("Idle") != std::string::npos) {
                idleIdx = (int)i;
                break;
            }
        }
        m_Registry->AddComponent<SkeletalAnimationComponent>(e, {idleIdx, (float)(rand()%100)*0.01f, 1.0f});
    }
  }
  std::cout << "Loaded dungeon from " << filename << std::endl;
}

} // namespace PixelsEngine
