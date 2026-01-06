#include "SiegeMode.h"
#include "../engine/Components.h"
#include "../engine/GLTFLoader.h"
#include <cmath>
#include <iostream>
#include <algorithm> // for min/max

using namespace PixelsEngine;

SiegeMode::SiegeMode(Registry* registry, GLRenderer* renderer)
    : m_Registry(registry), m_Renderer(renderer) {}

SiegeMode::~SiegeMode() {}

void SiegeMode::Init(Camera* camera, Entity& playerEntity) {
    // 1. Load Skeleton Assets
    if (!m_Renderer->GetRenderMesh("skel_minion")) {
        if (!m_Renderer->LoadTexture("skeleton_tex", "assets/skeletons/texture/skeleton_texture.png")) {
             std::cerr << "Failed to load skeleton texture" << std::endl;
        }
        
        m_Renderer->LoadMesh("skel_minion", "assets/skeletons/characters/gltf/Skeleton_Minion.glb");
        m_Renderer->LoadMesh("skel_warrior", "assets/skeletons/characters/gltf/Skeleton_Warrior.glb");
        m_Renderer->LoadMesh("skel_mage", "assets/skeletons/characters/gltf/Skeleton_Mage.glb");
        
        // 2. Load and Assign Animations
        std::vector<SkeletalAnimation> anims = GLTFLoader::LoadAnimations("assets/skeletons/Animations/gltf/Rig_Medium/Rig_Medium_General.glb");
        
        auto SetupMesh = [&](const std::string& name) {
            RenderMesh* rm = m_Renderer->GetRenderMesh(name);
            if (rm) {
                rm->animations = anims;
                rm->isSkinned = true;
            }
        };
        SetupMesh("skel_minion");
        SetupMesh("skel_warrior");
        SetupMesh("skel_mage");
    }

    // 3. Spawn Floor
    Entity floor = m_Registry->CreateEntity();
    m_Registry->AddComponent<Transform3DComponent>(floor, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
    m_Registry->AddComponent<MeshComponent>(floor, {"floor", "dungeon", 1.0f, 1.0f, 1.0f});
    
    // 4. Spawn Player
    playerEntity = m_Registry->CreateEntity();
    m_Registry->AddComponent<Transform3DComponent>(playerEntity, {0.0f, 0.0f, 2.0f, 0.0f, 0.0f});
    m_Registry->AddComponent<PlayerControlComponent>(playerEntity, {10.0f, 0.003f, 6.0f});
    m_Registry->AddComponent<PhysicsComponent>(playerEntity, {0, 0, 0, 15.0f, true, false, 0.0f, 1.0f}); 
    m_Registry->AddComponent<WeaponComponent>(playerEntity, {0.0f, 0.0f, false});
    
    if (camera) {
        camera->x = 0.0f; camera->y = 0.0f; camera->z = 2.0f;
        camera->yaw = 0; camera->pitch = 0;
    }

    // 5. Spawn Skeletons
    auto SpawnSkeleton = [&](const std::string& mesh, float x, float y, CharacterComponent::Type type) {
        Entity e = m_Registry->CreateEntity();
        float rot = atan2(-y, -x); // Face center
        m_Registry->AddComponent<Transform3DComponent>(e, {x, y, 0.0f, rot, 0.0f});
        m_Registry->AddComponent<MeshComponent>(e, {mesh, "skeleton_tex", 1.0f, 1.0f, 1.0f});
        
        // Find Idle animation
        int idleIdx = 0;
        RenderMesh* rm = m_Renderer->GetRenderMesh(mesh);
        if (rm && !rm->animations.empty()) {
             for(size_t i=0; i<rm->animations.size(); i++) {
                 if(rm->animations[i].name.find("Idle") != std::string::npos) { idleIdx=(int)i; break; }
             }
        }
        m_Registry->AddComponent<SkeletalAnimationComponent>(e, {idleIdx, (float)(rand()%100)*0.01f, 1.0f});
        m_Registry->AddComponent<EnemyComponent>(e, {100.0f, 1.5f + (float)(rand()%10)*0.1f, -1});
        m_Registry->AddComponent<CharacterComponent>(e, {type});
        m_Registry->AddComponent<ColliderComponent>(e, {0.5f, 1.8f, true});
    };
    
    // Circle of minions
    for(int i=0; i<8; i++) {
        float angle = i * (2*M_PI/8);
        SpawnSkeleton("skel_minion", cos(angle)*8.0f, sin(angle)*8.0f, CharacterComponent::SkeletonMinion);
    }
    // Some specialists
    SpawnSkeleton("skel_warrior", 12.0f, 0.0f, CharacterComponent::SkeletonWarrior);
    SpawnSkeleton("skel_mage", -12.0f, 0.0f, CharacterComponent::SkeletonMage);
}

void SiegeMode::Update(float dt, Entity playerEntity) {
  auto &enemies = m_Registry->View<EnemyComponent>();
  auto *playerTrans = m_Registry->GetComponent<Transform3DComponent>(playerEntity);
  if (!playerTrans) return;

  for (auto &pair : enemies) {
    auto &enemy = pair.second;
    auto *t = m_Registry->GetComponent<Transform3DComponent>(pair.first);
    if (t) {
      float dx = playerTrans->x - t->x;
      float dy = playerTrans->y - t->y;
      float dist = sqrt(dx * dx + dy * dy);
      if (dist > 1.5f) {
        t->rot = atan2(dy, dx);
        t->x += cos(t->rot) * enemy.speed * dt;
        t->y += sin(t->rot) * enemy.speed * dt;
      } 
      // Attack logic could go here
    }
  }
}
