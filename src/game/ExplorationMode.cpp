#include "ExplorationMode.h"
#include "../engine/Components.h"
#include <cmath>

using namespace PixelsEngine;

ExplorationMode::ExplorationMode(Registry *registry, GLRenderer *renderer,
                                 CreativeMode *editor)
    : m_Registry(registry), m_Renderer(renderer), m_CreativeMode(editor) {}

void ExplorationMode::Init(Camera *camera, Entity &playerEntity) {
    m_CreativeMode->Init(m_Registry, m_Renderer, camera);

  // Spawn Floor
  Entity floor = m_Registry->CreateEntity();
  m_Registry->AddComponent<Transform3DComponent>(
      floor, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
  m_Registry->AddComponent<MeshComponent>(
      floor, {"floor", "dungeon", 1.0f, 1.0f, 1.0f});

  // Spawn Player
  playerEntity = m_Registry->CreateEntity();
  m_Registry->AddComponent<Transform3DComponent>(
      playerEntity, {0.0f, -5.0f, 2.0f, M_PI * 0.5f, -0.2f});
  m_Registry->AddComponent<PlayerControlComponent>(playerEntity,
                                                   {10.0f, 0.003f, 6.0f});
  m_Registry->AddComponent<PhysicsComponent>(
      playerEntity, {0, 0, 0, 15.0f, true, true, 0.0f, 5.0f});
  m_Registry->AddComponent<WeaponComponent>(playerEntity, {0.0f, 0.0f, false});

  if (camera) {
    camera->x = 0.0f;
    camera->y = -5.0f;
    camera->z = 2.0f;
    camera->yaw = M_PI * 0.5f;
    camera->pitch = -0.2f;
  }
}
