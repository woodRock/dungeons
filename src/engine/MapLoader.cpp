#include "MapLoader.h"
#include "ECS.h"
#include "Graphics.h"
#include "GLRenderer.h"
#include "Components.h"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace PixelsEngine;

// Helper function to determine hitbox properties based on mesh name
void SetHitboxFromMeshName(const std::string& meshName, HitboxComponent& hitbox) {
  // Actual doors (the moving part) - only for wall_doorway_door, not wall_doorway
  if (meshName == "wall_doorway_door") {
    hitbox.width = 1.0f;
    hitbox.depth = 0.2f;
    hitbox.height = 2.2f;
    hitbox.offsetZ = 1.1f;
    hitbox.isSolid = true;
    return;
  }
  
  // Door frames (static) - these are just visual, not solid
  if (meshName == "wall_doorway") {
    hitbox.width = 1.0f;
    hitbox.depth = 0.1f;
    hitbox.height = 2.2f;
    hitbox.offsetZ = 1.1f;
    hitbox.isSolid = false;  // Frame is not solid
    return;
  }
  
  // Walls
  if (meshName.find("wall") != std::string::npos || meshName.find("corner") != std::string::npos) {
    hitbox.width = 1.0f;
    hitbox.depth = 1.0f;
    hitbox.height = 2.5f;
    hitbox.offsetZ = 1.25f;
    hitbox.isSolid = true;
    hitbox.isClimbable = false;
    return;
  }
  
  // Climbable/Jumpable furniture
  if (meshName.find("table") != std::string::npos) {
    hitbox.width = 1.5f;
    hitbox.depth = 1.5f;
    hitbox.height = 1.0f;
    hitbox.offsetZ = 0.5f;
    hitbox.isSolid = true;
    hitbox.isClimbable = true;
    hitbox.climbHeight = 1.0f;
    return;
  }
  
  if (meshName.find("chest") != std::string::npos) {
    hitbox.width = 1.2f;
    hitbox.depth = 0.8f;
    hitbox.height = 0.8f;
    hitbox.offsetZ = 0.4f;
    hitbox.isSolid = true;
    hitbox.isClimbable = true;
    hitbox.climbHeight = 0.8f;
    return;
  }
  
  if (meshName.find("crates") != std::string::npos || meshName.find("box") != std::string::npos) {
    hitbox.width = 1.2f;
    hitbox.depth = 1.2f;
    hitbox.height = 1.2f;
    hitbox.offsetZ = 0.6f;
    hitbox.isSolid = true;
    hitbox.isClimbable = true;
    hitbox.climbHeight = 1.2f;
    return;
  }
  
  if (meshName.find("barrel") != std::string::npos) {
    hitbox.width = 0.8f;
    hitbox.depth = 0.8f;
    hitbox.height = 1.0f;
    hitbox.offsetZ = 0.5f;
    hitbox.isSolid = true;
    hitbox.isClimbable = true;
    hitbox.climbHeight = 1.0f;
    return;
  }
  
  if (meshName.find("bed") != std::string::npos) {
    hitbox.width = 2.0f;
    hitbox.depth = 1.0f;
    hitbox.height = 0.5f;
    hitbox.offsetZ = 0.25f;
    hitbox.isSolid = true;
    hitbox.isClimbable = true;
    hitbox.climbHeight = 0.5f;
    return;
  }
  
  // Default for unknown objects - small non-solid
  hitbox.width = 0.5f;
  hitbox.depth = 0.5f;
  hitbox.height = 0.5f;
  hitbox.offsetZ = 0.25f;
  hitbox.isSolid = false;
  hitbox.isClimbable = false;
}

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
    
    // Add hitbox based on mesh type
    HitboxComponent hitbox;
    SetHitboxFromMeshName(meshName, hitbox);
    registry->AddComponent<HitboxComponent>(e, hitbox);
    
    // Add door component ONLY to actual doors (wall_doorway_door), not door frames
    if (meshName == "wall_doorway_door") {
      DoorComponent door;
      door.originalX = x;
      door.originalY = y;
      door.originalZ = z;
      door.isOpen = false;
      door.currentOpenDistance = 0.0f;
      door.maxOpenDistance = 1.5f;
      
      // Door swings on left hinge - the rotation determines which axis
      // For most doors in dungeon, they swing perpendicular to their facing direction
      door.swingAxis = DoorComponent::X;  // Swing left/right
      door.hingeOffsetX = -0.5f;  // Hinge on left side of door
      registry->AddComponent<DoorComponent>(e, door);
    }

    entitiesCreated++;
  }

  in.close();
  return entitiesCreated;
}
