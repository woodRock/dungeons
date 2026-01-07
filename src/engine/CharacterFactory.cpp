#include "CharacterFactory.h"
#include "ECS.h"
#include "Graphics.h"
#include "GLRenderer.h"
#include "Components.h"
#include <cstdlib>

using namespace PixelsEngine;

int CharacterFactory::FindAnimationIndex(RenderMesh* rm,
                                          const std::string& pattern) {
  if (!rm || rm->animations.empty()) {
    return 0;
  }

  for (size_t i = 0; i < rm->animations.size(); i++) {
    if (rm->animations[i].name.find(pattern) != std::string::npos) {
      return (int)i;
    }
  }

  return 0;
}

Entity CharacterFactory::CreatePlayer(Registry* registry,
                                       GLRenderer* renderer, float x, float y,
                                       float z) {
  return CreateCharacter(registry, renderer, "Knight", "Knight_tex", x, y, z,
                          100, 100, 15, true);
}

Entity CharacterFactory::CreateSkeleton(Registry* registry,
                                         GLRenderer* renderer,
                                         const std::string& meshName, float x,
                                         float y, float z) {
  // Use the mesh name with _tex suffix (matching AssetManager pattern)
  std::string texName = meshName + "_tex";
  return CreateCharacter(registry, renderer, meshName, texName, x, y, z,
                          50, 50, 10, false);
}

Entity CharacterFactory::CreateCharacter(Registry* registry,
                                          GLRenderer* renderer,
                                          const std::string& meshName,
                                          const std::string& textureName,
                                          float x, float y, float z, int hp,
                                          int maxHp, int attack,
                                          bool isPlayer) {
  if (!registry || !renderer) {
    return -1; // Invalid entity
  }

  Entity e = registry->CreateEntity();

  // Transform component
  registry->AddComponent<Transform3DComponent>(e,
                                                {x, y, z, 0.0f, 0.0f});

  // Mesh component
  registry->AddComponent<MeshComponent>(e, {meshName, textureName, 1, 1, 1});

  // Physics component
  registry->AddComponent<PhysicsComponent>(e,
                                            {0, 0, 0, 15.0f, true, false,
                                             0.0f, 1.0f});

  // Battle unit component
  BattleUnitComponent::Team team =
      isPlayer ? BattleUnitComponent::Player : BattleUnitComponent::Enemy;
  registry->AddComponent<BattleUnitComponent>(e,
                                               {team, hp, maxHp, attack, 0});

  // Animation component with Idle animation
  RenderMesh* rm = renderer->GetRenderMesh(meshName);
  int idleIdx = FindAnimationIndex(rm, "Idle");
  float startTime =
      isPlayer ? 0.0f : (float)(rand() % 100) * 0.01f; // Random start for
                                                         // variety
  registry->AddComponent<SkeletalAnimationComponent>(e,
                                                      {idleIdx, startTime, 1.0f});

  // Specific components based on role
  if (isPlayer) {
    // Player components
    registry->AddComponent<PlayerControlComponent>(e,
                                                    {5.0f, 0.003f, 6.0f});
    registry->AddComponent<WeaponComponent>(e, {0.0f, 0.0f, false});

    // Default sword attachment
    AttachmentComponent attach;
    attach.meshName = "Sword";
    attach.textureName = textureName;
    attach.boneName = "hand.r";
    attach.rotX = -7.63f;
    attach.rotY = 18.148f;
    attach.rotZ = 1.702f;
    registry->AddComponent<AttachmentComponent>(e, attach);
  } else {
    // Enemy components
    registry->AddComponent<EnemyComponent>(e, {50.0f, 2.0f, -1});
    registry->AddComponent<ColliderComponent>(e, {0.5f, 1.8f, true});
  }

  return e;
}
