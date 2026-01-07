#pragma once

#include "ECS.h"
#include <string>

namespace PixelsEngine {
class GLRenderer;
struct RenderMesh;
} // namespace PixelsEngine

using PixelsEngine::Entity;
using PixelsEngine::Registry;
using PixelsEngine::GLRenderer;
using PixelsEngine::RenderMesh;

/**
 * CharacterFactory provides standardized entity creation for game characters.
 * Ensures consistency in component setup across different character types.
 */
class CharacterFactory {
public:
  /**
   * Create a player character (Knight)
   * @param registry ECS registry
   * @param renderer Renderer for animation lookup
   * @param x Starting X position
   * @param y Starting Y position
   * @param z Starting Z position
   * @return Created entity
   */
  static Entity CreatePlayer(Registry* registry, GLRenderer* renderer,
                              float x, float y, float z);

  /**
   * Create an enemy skeleton character
   * @param registry ECS registry
   * @param renderer Renderer for animation lookup
   * @param meshName Name of the skeleton mesh (e.g., "skel_minion",
   * "skel_warrior")
   * @param x Starting X position
   * @param y Starting Y position
   * @param z Starting Z position
   * @return Created entity
   */
  static Entity CreateSkeleton(Registry* registry, GLRenderer* renderer,
                                const std::string& meshName, float x, float y,
                                float z);

  /**
   * Create a generic skeletal character with specified stats
   * @param registry ECS registry
   * @param renderer Renderer for animation lookup
   * @param meshName Name of the mesh
   * @param textureName Name of the texture
   * @param x Starting X position
   * @param y Starting Y position
   * @param z Starting Z position
   * @param hp Health points
   * @param maxHp Maximum health
   * @param attack Attack damage
   * @param isPlayer Whether this is a player-controlled entity
   * @return Created entity
   */
  static Entity CreateCharacter(Registry* registry, GLRenderer* renderer,
                                 const std::string& meshName,
                                 const std::string& textureName, float x,
                                 float y, float z, int hp, int maxHp,
                                 int attack, bool isPlayer = false);

private:
  /**
   * Helper to find animation index by name pattern
   * @param rm Render mesh containing animations
   * @param pattern Pattern to search for in animation names
   * @return Animation index, or 0 if not found
   */
  static int FindAnimationIndex(class RenderMesh* rm,
                                 const std::string& pattern);
};
