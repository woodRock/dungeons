# Refactoring Analysis - Game Mode Code Reuse

## Identified Patterns for Engine Abstraction

### 1. **Audio Management** (HIGH PRIORITY)
**Current Pattern:** Each game mode loads and manages audio separately
- SiegeMode: ~40 lines of Mix_LoadWAV and error checking
- BattleMode: ~60 lines of Mix_LoadWAV, Mix_LoadMUS, Mix_PlayMusic
- DungeonsGame: PlaySpatialSfx implementation for 3D audio

**Proposed Solution:** `AudioManager` class in engine
```cpp
class AudioManager {
  Mix_Chunk* LoadWAV(const std::string& path);
  Mix_Music* LoadMusic(const std::string& path);
  void PlaySfx(Mix_Chunk* chunk, int loops = 0);
  void PlaySfxSpatial(Mix_Chunk* chunk, float x, float y, float z);
  void PlayMusic(Mix_Music* music, int loops = -1);
  void FreeChunk(Mix_Chunk*& chunk);
  void FreeMusic(Mix_Music*& music);
};
```

**Benefit:** Eliminates duplicated audio loading, error checking, and spatial audio code

---

### 2. **Asset Loading** (HIGH PRIORITY)
**Current Pattern:** Each game mode has similar LoadIfMissing pattern
- BattleMode: Complex LoadIfMissing lambda with animation loading
- SiegeMode: Mesh/texture loading with animation setup
- CreativeMode: Asset scanning and caching

**Proposed Solution:** `AssetManager` in engine
```cpp
class AssetManager {
  bool LoadMesh(const std::string& name, const std::string& path);
  bool LoadTexture(const std::string& name, const std::string& path);
  void LoadCharacter(const std::string& name, const std::string& meshPath,
                     const std::string& texPath, bool withAnimations = true);
  RenderMesh* GetMesh(const std::string& name);
  
private:
  GLRenderer* m_Renderer;
  void LoadSkeletalAnimations(RenderMesh* mesh, const std::string& character);
};
```

**Benefit:** Centralizes asset caching, animation loading, and prevents duplicate loads

---

### 3. **Map Loading** (MEDIUM PRIORITY)
**Current Pattern:** Identical map loading code in BattleMode and SiegeMode
```cpp
void LoadMap(const std::string &path) {
  std::ifstream in(path);
  while (in >> m << t >> x >> y >> z >> r) {
    if (!m_Renderer->GetRenderMesh(m)) {
      m_Renderer->LoadMesh(m, "assets/dungeons/Assets/obj/" + m + ".obj");
    }
    auto e = m_Registry->CreateEntity();
    m_Registry->AddComponent<Transform3DComponent>(e, {x, y, z, r, 0});
    m_Registry->AddComponent<MeshComponent>(e, {m, t, 1, 1, 1});
  }
}
```

**Proposed Solution:** `MapLoader` utility function
```cpp
class MapLoader {
  static std::vector<Entity> LoadFromFile(const std::string& path, 
                                          Registry* registry, 
                                          GLRenderer* renderer);
};
```

**Benefit:** Single implementation, used by all game modes

---

### 4. **Character Spawning** (MEDIUM PRIORITY)
**Current Pattern:** SpawnPlayer, SpawnCharacter, SpawnEnemy methods
- Nearly identical spawning logic across modes
- Add components in same sequence: Transform, Mesh, Animation, Physics, Battle/Control

**Proposed Solution:** `CharacterFactory` in engine
```cpp
class CharacterFactory {
  static Entity CreateKnight(Registry* registry, float x, float y, float z);
  static Entity CreateSkeleton(Registry* registry, float x, float y, float z,
                               SkeletonType type);
  static Entity CreateCharacter(Registry* registry, 
                                const CharacterDefinition& def,
                                float x, float y, float z);
};
```

**Benefit:** Ensures consistency in entity setup, reduces duplication

---

### 5. **Cursor/Raycasting** (MEDIUM PRIORITY)
**Current Pattern:** Manual cursor raycasting in SiegeMode
- Raycaster already exists in engine but not fully utilized
- Each mode implements similar cursor position tracking

**Proposed Solution:** Enhance `Raycaster` for unified cursor handling
```cpp
class CursorTracker {
  void Update(Registry* registry, Camera* camera, GLRenderer* renderer);
  bool IsActive() const;
  float GetX(), GetY(), GetZ() const;
  Entity GetHoveredEntity() const;
};
```

**Benefit:** Consistent cursor behavior across modes

---

### 6. **UI Rendering Helpers** (MEDIUM PRIORITY)
**Current Pattern:** RenderUI in each mode has similar patterns
- HUD elements, text rendering, health bars
- Reusable patterns for crosshairs, UI panels, health displays

**Proposed Solution:** `UIHelper` utility class
```cpp
class UIHelper {
  static void DrawCrosshair(GLRenderer* renderer, int centerX, int centerY);
  static void DrawHealthBar(GLRenderer* renderer, int x, int y, 
                           int hp, int maxHp);
  static void DrawText(TextRenderer* tr, const std::string& text,
                       int x, int y, SDL_Color color);
};
```

**Benefit:** Consistent UI appearance, less code duplication

---

### 7. **Animation State Management** (LOW PRIORITY)
**Current Pattern:** Finding animations by name string pattern
- BattleMode and SiegeMode both do similar animation searching
- `std::string::find()` patterns repeated multiple times

**Proposed Solution:** `AnimationManager` helper
```cpp
class AnimationManager {
  static int FindAnimationIndex(RenderMesh* mesh, const std::string& pattern);
  static bool SetAnimation(SkeletalAnimationComponent* anim, 
                          RenderMesh* mesh, const std::string& pattern);
};
```

**Benefit:** Cleaner animation handling, single search implementation

---

## Implementation Priority

1. **Phase 1 (High Impact):**
   - AudioManager - eliminates ~100+ lines of duplicated code
   - AssetManager - eliminates ~200+ lines, improves asset caching
   - MapLoader - eliminates ~30 lines per game mode

2. **Phase 2 (Medium Impact):**
   - CharacterFactory - standardizes entity creation
   - Enhanced Raycaster with CursorTracker
   - UIHelper utilities

3. **Phase 3 (Polish):**
   - AnimationManager helpers
   - Additional UI components as needed

## Estimated Code Reduction
- **Before Refactoring:** ~1500+ lines of game mode code
- **After Refactoring:** ~800 lines (estimated 47% reduction)
- **Engine Additions:** ~400 new reusable lines
- **Net Savings:** ~300 lines + improved maintainability
