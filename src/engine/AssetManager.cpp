#include "AssetManager.h"
#include "GLRenderer.h"
#include "GLTFLoader.h"
#include "Graphics.h"
#include <iostream>
#include <filesystem>

AssetManager::AssetManager(GLRenderer* renderer) : m_Renderer(renderer) {}

bool AssetManager::LoadMesh(const std::string& name, const std::string& path) {
  if (!m_Renderer) {
    return false;
  }

  // Check if already loaded
  if (IsMeshLoaded(name)) {
    return true;
  }

  if (!m_Renderer->LoadMesh(name, path)) {
    std::cerr << "Failed to load mesh: " << name << " from " << path
              << std::endl;
    return false;
  }

  m_LoadedMeshes[name] = true;
  return true;
}

bool AssetManager::LoadTexture(const std::string& name,
                                const std::string& path) {
  if (!m_Renderer) {
    return false;
  }

  // Check if already loaded
  if (IsTextureLoaded(name)) {
    return true;
  }

  if (!m_Renderer->LoadTexture(name, path)) {
    std::cerr << "Failed to load texture: " << name << " from " << path
              << std::endl;
    return false;
  }

  m_LoadedTextures[name] = true;
  return true;
}

bool AssetManager::LoadCharacter(const std::string& name,
                                  const std::string& meshPath,
                                  const std::string& texturePath,
                                  bool withAnimations) {
  // Load texture first
  std::string textureName = name + "_tex";
  if (!LoadTexture(textureName, texturePath)) {
    return false;
  }

  // Load mesh
  if (!LoadMesh(name, meshPath)) {
    return false;
  }

  // Load animations if requested
  if (withAnimations) {
    RenderMesh* mesh = GetMesh(name);
    if (mesh) {
      LoadSkeletalAnimations(mesh, name);
    }
  }

  return true;
}

RenderMesh* AssetManager::GetMesh(const std::string& name) {
  if (!m_Renderer) {
    return nullptr;
  }
  return m_Renderer->GetRenderMesh(name);
}

bool AssetManager::IsMeshLoaded(const std::string& name) const {
  return m_LoadedMeshes.find(name) != m_LoadedMeshes.end();
}

bool AssetManager::IsTextureLoaded(const std::string& name) const {
  return m_LoadedTextures.find(name) != m_LoadedTextures.end();
}

void AssetManager::ClearCache() {
  m_LoadedMeshes.clear();
  m_LoadedTextures.clear();
}

void AssetManager::LoadSkeletalAnimations(RenderMesh* mesh,
                                          const std::string& character) {
  if (!mesh) {
    return;
  }

  std::vector<PixelsEngine::SkeletalAnimation> allAnims;

  // Standard animation files to load
  std::vector<std::string> animFiles = {
      "Rig_Medium_General.glb",
      "Rig_Medium_CombatMelee.glb",
      "Rig_Medium_CombatRanged.glb",
      "Rig_Medium_MovementBasic.glb"};

  // Determine base animation path based on character type
  std::string basePath;
  if (character.find("skel_") == 0 || character.find("Skeleton") != std::string::npos) {
    // Skeleton characters
    basePath = "assets/skeletons/Animations/gltf/Rig_Medium/";
  } else {
    // Adventurer characters
    basePath = "assets/adventurers/Animations/gltf/Rig_Medium/";
  }

  namespace fs = std::filesystem;

  // Load standard animation files
  for (const auto& animFile : animFiles) {
    std::string fullPath = basePath + animFile;
    
    // If specific path doesn't exist, try generic animations folder
    if (!fs::exists(fullPath)) {
      fullPath = "assets/animations/Animations/gltf/Rig_Medium/" + animFile;
    }

    try {
      if (fs::exists(fullPath)) {
        auto anims = PixelsEngine::GLTFLoader::LoadAnimations(fullPath);
        allAnims.insert(allAnims.end(), anims.begin(), anims.end());
      }
    } catch (const std::exception& e) {
      std::cerr << "Error loading animation file " << fullPath << ": "
                << e.what() << std::endl;
      continue;
    }
  }

  // Apply animations to mesh
  if (!allAnims.empty()) {
    mesh->animations = allAnims;
    mesh->isSkinned = true;
  } else {
    std::cerr << "Warning: No animations loaded for " << character << std::endl;
  }
}
