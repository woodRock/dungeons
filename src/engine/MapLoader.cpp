#include "MapLoader.h"
#include "ECS.h"
#include "Graphics.h"
#include "GLRenderer.h"
#include "Components.h"
#include <fstream>
#include <iostream>

using namespace PixelsEngine;

int MapLoader::LoadFromFile(const std::string& path, Registry* registry,
                             GLRenderer* renderer) {
  return LoadFromFileWithPath(path, registry, renderer,
                               "assets/dungeons/Assets/obj/");
}

int MapLoader::LoadFromFileWithPath(const std::string& path, Registry* registry,
                                     GLRenderer* renderer,
                                     const std::string& assetBasePath) {
  if (!registry || !renderer) {
    std::cerr << "MapLoader: Invalid registry or renderer" << std::endl;
    return 0;
  }

  std::ifstream in(path);
  if (!in.is_open()) {
    std::cerr << "MapLoader: Failed to open map file: " << path << std::endl;
    return 0;
  }

  int entitiesCreated = 0;
  std::string meshName, textureName;
  float x, y, z, rotation;

  while (in >> meshName >> textureName >> x >> y >> z >> rotation) {
    // Load mesh if not already loaded
    if (!renderer->GetRenderMesh(meshName)) {
      std::string fullPath = assetBasePath + meshName + ".obj";
      if (!renderer->LoadMesh(meshName, fullPath)) {
        std::cerr << "MapLoader: Failed to load mesh: " << meshName
                  << " from " << fullPath << std::endl;
        continue;
      }
    }

    // Create entity with transform and mesh components
    Entity e = registry->CreateEntity();
    registry->AddComponent<Transform3DComponent>(
        e, {x, y, z, rotation, 0});
    registry->AddComponent<MeshComponent>(e, {meshName, textureName, 1, 1, 1});

    entitiesCreated++;
  }

  in.close();
  return entitiesCreated;
}
