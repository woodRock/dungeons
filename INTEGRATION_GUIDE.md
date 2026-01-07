# Quick Integration Guide

## How to Use the New Refactoring Modules

### 1. AudioManager Integration Example

**Before:**
```cpp
// In SiegeMode::Init()
m_SfxShoot = Mix_LoadWAV("assets/bow_shoot.wav");
m_SfxHit = Mix_LoadWAV("assets/bow_hit.wav");
```

**After:**
```cpp
// In SiegeMode header
std::unique_ptr<AudioManager> m_AudioManager;

// In SiegeMode::Init()
m_AudioManager = std::make_unique<AudioManager>();
m_SfxShoot = m_AudioManager->LoadWAV("assets/bow_shoot.wav");
m_SfxHit = m_AudioManager->LoadWAV("assets/bow_hit.wav");

// Playing sounds:
m_AudioManager->PlaySfx(m_SfxShoot);
m_AudioManager->PlaySfxSpatial(m_SfxHit, x, y, z, 
                                m_Camera->x, m_Camera->y, m_Camera->yaw);
```

---

### 2. AssetManager Integration Example

**Before:**
```cpp
// In BattleMode::Init()
auto LoadIfMissing = [this](const std::string& name, ...) { /* 50+ lines */ };
LoadIfMissing("Knight", "assets/adventurers/Characters/gltf/Knight.glb", ...);
LoadIfMissing("Skeleton_Warrior", "assets/skeletons/characters/gltf/Skeleton_Warrior.glb", ...);
```

**After:**
```cpp
// In BattleMode header
std::unique_ptr<AssetManager> m_AssetManager;

// In BattleMode::Init()
m_AssetManager = std::make_unique<AssetManager>(m_Renderer);
m_AssetManager->LoadCharacter("Knight", 
                               "assets/adventurers/Characters/gltf/Knight.glb",
                               "assets/adventurers/Textures/knight_texture.png");
m_AssetManager->LoadCharacter("Skeleton_Warrior",
                               "assets/skeletons/characters/gltf/Skeleton_Warrior.glb",
                               "assets/skeletons/texture/skeleton_texture.png");
```

---

### 3. MapLoader Integration Example

**Before:**
```cpp
// In both BattleMode::LoadMap() and SiegeMode::LoadMap()
void LoadMap(const std::string &path) {
  std::ifstream in(path);
  std::string m, t;
  float x, y, z, r;
  while (in >> m >> t >> x >> y >> z >> r) {
    if (!m_Renderer->GetRenderMesh(m)) {
      m_Renderer->LoadMesh(m, "assets/dungeons/Assets/obj/" + m + ".obj");
    }
    auto e = m_Registry->CreateEntity();
    m_Registry->AddComponent<Transform3DComponent>(e, {x, y, z, r, 0});
    m_Registry->AddComponent<MeshComponent>(e, {m, t, 1, 1, 1});
  }
}
```

**After:**
```cpp
// Replace entire LoadMap() implementation with single line:
MapLoader::LoadFromFile("assets/dungeons/my_dungeon.map", m_Registry, m_Renderer);

// Or with custom asset path:
MapLoader::LoadFromFileWithPath(path, m_Registry, m_Renderer, 
                                "assets/dungeons/Assets/obj/");
```

---

### 4. CharacterFactory Integration Example

**Before:**
```cpp
// In SiegeMode::SpawnPlayer()
playerEntity = m_Registry->CreateEntity();
m_Registry->AddComponent<Transform3DComponent>(playerEntity, {12.0f, 0.0f, 0.0f, 0.0f, 0.0f});
m_Registry->AddComponent<MeshComponent>(playerEntity, {"Knight", "knight_tex", 1, 1, 1});
m_Registry->AddComponent<PlayerControlComponent>(playerEntity, {5.0f, 0.003f, 6.0f});
m_Registry->AddComponent<PhysicsComponent>(playerEntity, {0, 0, 0, 15.0f, true, false, 0.0f, 1.0f});
m_Registry->AddComponent<WeaponComponent>(playerEntity, {0.0f, 0.0f, false});
m_Registry->AddComponent<BattleUnitComponent>(playerEntity, {BattleUnitComponent::Player, 100, 100, 15, 0});
// ... find Idle animation, set up attachment, etc.
```

**After:**
```cpp
// Single line replaces ~40 lines of boilerplate:
playerEntity = CharacterFactory::CreatePlayer(m_Registry, m_Renderer, 12.0f, 0.0f, 0.0f);

// For enemies:
Entity skeleton = CharacterFactory::CreateSkeleton(m_Registry, m_Renderer, "skel_minion", 12.0f, 10.0f, 0.0f);

// For custom characters:
Entity custom = CharacterFactory::CreateCharacter(m_Registry, m_Renderer,
                                                   "Ranger", "ranger_tex",
                                                   10, 5, 0,  // position
                                                   120, 120, 18,  // hp, maxHp, attack
                                                   true);  // isPlayer
```

---

### 5. UIHelper Integration Example

**Before:**
```cpp
// In SiegeMode::RenderUI()
ren->DrawRect2D(w / 2 - 1, h / 2 - 10, 2, 20, {255, 255, 255, 150});
ren->DrawRect2D(w / 2 - 10, h / 2 - 1, 20, 2, {255, 255, 255, 150});

ren->DrawRect2D(20, h - 40, 200, 20, {50, 50, 50, 200});
ren->DrawRect2D(22, h - 38, (int)(196 * ((float)hp / maxHp)), 16, {255, 50, 50, 255});

if (m_HitmarkerTimer > 0) {
  ren->DrawRect2D(w / 2 - 15, h / 2 - 15, 5, 5, {255, 0, 0, 255});
  // ... 8 more lines
}
```

**After:**
```cpp
// In SiegeMode::RenderUI()
UIHelper::DrawCrosshair(ren, w / 2, h / 2);

UIHelper::DrawHealthBar(ren, 20, h - 40, 200, 20, hp, maxHp);

if (m_HitmarkerTimer > 0) {
  UIHelper::DrawHitmarker(ren, w / 2, h / 2);
}

UIHelper::DrawText(tr, ren, "SIEGE MODE: 1 - SWORD, 2 - CROSSBOW", 20, 20, {255, 255, 255, 255});
```

---

## Integration Checklist

- [ ] Create AudioManager instance in BattleMode and SiegeMode
- [ ] Replace all Mix_LoadWAV calls with m_AudioManager->LoadWAV()
- [ ] Replace all Mix_PlayChannel calls with m_AudioManager->PlaySfx()
- [ ] Replace PlaySpatialSfx calls with m_AudioManager->PlaySfxSpatial()
- [ ] Clean up Mix_FreeChunk calls (AudioManager handles cleanup)

- [ ] Create AssetManager instance in BattleMode and SiegeMode
- [ ] Replace LoadIfMissing lambdas with m_AssetManager->LoadCharacter()
- [ ] Remove LoadIfMissing method implementations

- [ ] Replace LoadMap() implementations with MapLoader::LoadFromFile()
- [ ] Remove duplicate LoadMap() code from both game modes

- [ ] Replace SpawnPlayer() with CharacterFactory::CreatePlayer()
- [ ] Replace SpawnEnemy() with CharacterFactory::CreateSkeleton() or CreateCharacter()
- [ ] Update SpawnCharacter() calls to use factory

- [ ] Replace DrawRect2D calls in RenderUI() with UIHelper equivalents
- [ ] Ensure UI appearance is consistent across all modes

---

## Quick Reference: Headers to Include

```cpp
#include "../engine/AudioManager.h"
#include "../engine/AssetManager.h"
#include "../engine/MapLoader.h"
#include "../engine/CharacterFactory.h"
#include "../engine/UIHelper.h"
```

## Estimated Impact
- **Lines of code eliminated**: 430+
- **Code reduction**: 25-30% from game mode implementations
- **Maintenance burden**: Significantly reduced
- **Consistency**: Greatly improved across game modes
- **Bug risk**: Minimized through centralized implementations
