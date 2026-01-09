#include "MapLoader.h"
#include "AssetManager.h"
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
    hitbox.width = 1.2f;  // Increased to prevent gap leaks
    hitbox.depth = 0.4f;  // Thicker to ensure raycast hits
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
    if (meshName.find("corner") != std::string::npos) {
        // Corners are usually connecting points
        hitbox.width = 1.5f;
        hitbox.depth = 1.5f;
    } else {
        // Straight walls are long
        // Assuming default alignment is along X axis, so width is length
        hitbox.width = 4.0f;
        hitbox.depth = 0.5f;
    }
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

  // Forest assets - trees, bushes, grass, rocks (mostly non-solid decoration)
  if (meshName.find("Tree") != std::string::npos) {
    hitbox.width = 0.8f;
    hitbox.depth = 0.8f;
    hitbox.height = 3.0f;
    hitbox.offsetZ = 1.5f;
    hitbox.isSolid = false;  // Decorative, not solid
    hitbox.isClimbable = false;
    return;
  }

  if (meshName.find("Bush") != std::string::npos) {
    hitbox.width = 0.6f;
    hitbox.depth = 0.6f;
    hitbox.height = 1.0f;
    hitbox.offsetZ = 0.5f;
    hitbox.isSolid = false;  // Decorative
    hitbox.isClimbable = false;
    return;
  }

  if (meshName.find("Grass") != std::string::npos) {
    hitbox.width = 0.5f;
    hitbox.depth = 0.5f;
    hitbox.height = 0.3f;
    hitbox.offsetZ = 0.15f;
    hitbox.isSolid = false;  // Ground decoration
    hitbox.isClimbable = false;
    return;
  }

  if (meshName.find("Rock") != std::string::npos) {
    hitbox.width = 0.7f;
    hitbox.depth = 0.7f;
    hitbox.height = 0.6f;
    hitbox.offsetZ = 0.3f;
    hitbox.isSolid = false;  // Decorative
    hitbox.isClimbable = false;
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
    std::string actualMeshToLoad = meshName;
    std::string actualTexToLoad = textureName;
    
    if (meshName == "player_spawn") {
        actualMeshToLoad = "Knight";
        actualTexToLoad = "Knight_tex";
    }

    // Load mesh if not already loaded
    if (!renderer->GetRenderMesh(actualMeshToLoad)) {
      if (actualMeshToLoad == "Knight" || actualMeshToLoad.find("Skeleton") != std::string::npos) {
          // Characters should be pre-loaded, but if not, try to load them
          // This is a safety fallback
          std::string meshPath = (actualMeshToLoad == "Knight") ? 
              "assets/adventurers/Characters/gltf/Knight.glb" :
              "assets/skeletons/characters/gltf/" + actualMeshToLoad + ".glb";
          std::string texPath = (actualMeshToLoad == "Knight") ?
              "assets/adventurers/Textures/knight_texture.png" :
              "assets/skeletons/texture/skeleton_texture.png";
          
          auto am = std::make_unique<AssetManager>(renderer);
          am->LoadCharacter(actualMeshToLoad, meshPath, texPath);
      } else if (actualMeshToLoad.find("Tree") != std::string::npos || 
                 actualMeshToLoad.find("Bush") != std::string::npos ||
                 actualMeshToLoad.find("Grass") != std::string::npos ||
                 actualMeshToLoad.find("Rock") != std::string::npos) {
          // Forest assets
          std::string fullPath = "assets/forests/Assets/gltf/" + actualMeshToLoad + ".gltf";
          if (!renderer->LoadMesh(actualMeshToLoad, fullPath)) {
            std::cerr << "MapLoader: Failed to load forest mesh: " << actualMeshToLoad
                      << " from " << fullPath << std::endl;
            continue;
          }
      } else {
          std::string fullPath = assetBasePath + actualMeshToLoad + ".obj";
          if (!renderer->LoadMesh(actualMeshToLoad, fullPath)) {
            std::cerr << "MapLoader: Failed to load mesh: " << actualMeshToLoad
                      << " from " << fullPath << std::endl;
            continue;
          }
      }
    }

    // Create entity with transform and mesh components
    Entity e = registry->CreateEntity();
    registry->AddComponent<Transform3DComponent>(
        e, {x, y, z, rotation, 0});
    registry->AddComponent<MeshComponent>(e, {meshName, textureName, 1, 1, 1});
    
    // Restore animations if skinned
    RenderMesh* rm = renderer->GetRenderMesh(actualMeshToLoad);
    if (rm && rm->isSkinned) {
        int idleIdx = 0;
        for (size_t i = 0; i < rm->animations.size(); ++i) {
            if (rm->animations[i].name.find("Idle") != std::string::npos) {
                idleIdx = (int)i;
                break;
            }
        }
        registry->AddComponent<SkeletalAnimationComponent>(e, {idleIdx, 0.0f, 1.0f});
    }
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
