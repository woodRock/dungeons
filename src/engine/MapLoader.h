#pragma once

#include <string>
#include <vector>
#include "ECS.h"

namespace PixelsEngine {
class GLRenderer;
} // namespace PixelsEngine

using PixelsEngine::Registry;
using PixelsEngine::Entity;
using PixelsEngine::GLRenderer;

/**
 * MapLoader utility for loading map files.
 * Map files contain lines with format: meshName textureName x y z rotation
 */
class MapLoader {
public:
  /**
   * Load a map from a file and create entities in the registry
   * @param path File path to the .map file
   * @param registry ECS registry to add entities to
   * @param renderer Renderer for loading mesh assets
   * @return Number of entities created
   */
  static int LoadFromFile(const std::string& path, Registry* registry,
                           GLRenderer* renderer);

  /**
   * Load a map from a file with custom asset path
   * @param path File path to the .map file
   * @param registry ECS registry to add entities to
   * @param renderer Renderer for loading mesh assets
   * @param assetBasePath Base path for mesh assets (e.g.,
   * "assets/dungeons/Assets/obj/")
   * @return Number of entities created
   */
  static int LoadFromFileWithPath(const std::string& path, Registry* registry,
                                   GLRenderer* renderer,
                                   const std::string& assetBasePath);
};
