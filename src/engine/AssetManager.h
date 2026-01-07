#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace PixelsEngine {
class GLRenderer;
struct RenderMesh;
} // namespace PixelsEngine

using PixelsEngine::GLRenderer;
using PixelsEngine::RenderMesh;

/**
 * AssetManager centralizes asset loading and caching.
 * Prevents duplicate loads and provides consistent asset initialization.
 */
class AssetManager {
public:
  AssetManager(GLRenderer* renderer);
  ~AssetManager() = default;

  /**
   * Load a mesh and cache it
   * @param name Unique identifier for the mesh
   * @param path File path to the mesh file
   * @return true if load successful, false otherwise
   */
  bool LoadMesh(const std::string& name, const std::string& path);

  /**
   * Load a texture and cache it
   * @param name Unique identifier for the texture
   * @param path File path to the texture file
   * @return true if load successful, false otherwise
   */
  bool LoadTexture(const std::string& name, const std::string& path);

  /**
   * Load a complete character with mesh, texture, and skeletal animations
   * @param name Unique identifier for the character
   * @param meshPath Path to the mesh file (.glb, .obj, etc.)
   * @param texturePath Path to the texture file
   * @param withAnimations Whether to load skeletal animations
   * @return true if load successful, false otherwise
   */
  bool LoadCharacter(const std::string& name, const std::string& meshPath,
                      const std::string& texturePath,
                      bool withAnimations = true);

  /**
   * Get a previously loaded mesh
   * @param name Unique identifier
   * @return Pointer to RenderMesh, or nullptr if not found
   */
  RenderMesh* GetMesh(const std::string& name);

  /**
   * Check if a mesh is already loaded
   * @param name Unique identifier
   * @return true if loaded, false otherwise
   */
  bool IsMeshLoaded(const std::string& name) const;

  /**
   * Check if a texture is already loaded
   * @param name Unique identifier
   * @return true if loaded, false otherwise
   */
  bool IsTextureLoaded(const std::string& name) const;

  /**
   * Clear all cached asset names (doesn't free from renderer)
   */
  void ClearCache();

private:
  GLRenderer* m_Renderer;
  std::unordered_map<std::string, bool> m_LoadedMeshes;
  std::unordered_map<std::string, bool> m_LoadedTextures;

  /**
   * Load skeletal animations for a character from standard animation paths
   * @param character Character name (e.g., "Knight", "Skeleton_Warrior")
   */
  void LoadSkeletalAnimations(RenderMesh* mesh, const std::string& character);
};
