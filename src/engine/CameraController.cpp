#include "CameraController.h"
#include "Input.h"
#include "Components.h"
#include <cmath>
#include <iostream>

namespace PixelsEngine {

// ============================================================================
// FirstPersonCamera Implementation
// ============================================================================

FirstPersonCamera::FirstPersonCamera(Camera* camera)
    : m_Camera(camera) {}

void FirstPersonCamera::HandleInput(float dt) {
  if (!m_Camera || m_InventoryOpen) return;

  // Mouse Look
  int mx, my;
  Input::GetMouseDelta(mx, my);
  
  m_Camera->yaw -= mx * m_MouseSensitivity;
  m_Camera->pitch -= my * m_MouseSensitivity;
  
  if (m_Camera->pitch > 1.5f) m_Camera->pitch = 1.5f;
  if (m_Camera->pitch < -1.5f) m_Camera->pitch = -1.5f;

  // WASD Movement
  float dirX = cos(m_Camera->yaw) * cos(m_Camera->pitch);
  float dirY = sin(m_Camera->yaw) * cos(m_Camera->pitch);
  float dirZ = sin(m_Camera->pitch);
  
  float rightX = cos(m_Camera->yaw + M_PI / 2.0f);
  float rightY = sin(m_Camera->yaw + M_PI / 2.0f);
  
  float moveSpeed = m_MoveSpeed * dt;

  if (Input::IsKeyDown(SDL_SCANCODE_W)) {
    m_Camera->x += dirX * moveSpeed;
    m_Camera->y += dirY * moveSpeed;
    m_Camera->z += dirZ * moveSpeed;
  }
  if (Input::IsKeyDown(SDL_SCANCODE_S)) {
    m_Camera->x -= dirX * moveSpeed;
    m_Camera->y -= dirY * moveSpeed;
    m_Camera->z -= dirZ * moveSpeed;
  }
  if (Input::IsKeyDown(SDL_SCANCODE_A)) {
    m_Camera->x -= rightX * moveSpeed;
    m_Camera->y -= rightY * moveSpeed;
  }
  if (Input::IsKeyDown(SDL_SCANCODE_D)) {
    m_Camera->x += rightX * moveSpeed;
    m_Camera->y += rightY * moveSpeed;
  }
  if (Input::IsKeyDown(SDL_SCANCODE_SPACE)) {
    m_Camera->z += moveSpeed;
  }
  if (Input::IsKeyDown(SDL_SCANCODE_LSHIFT)) {
    m_Camera->z -= moveSpeed;
  }
}

void FirstPersonCamera::Update(float dt) {
  // First person camera doesn't need update; it only responds to input
}

// ============================================================================
// ThirdPersonCamera Implementation
// ============================================================================

ThirdPersonCamera::ThirdPersonCamera(Camera* camera, Registry* registry, Entity targetEntity)
    : m_Camera(camera), m_Registry(registry), m_TargetEntity(targetEntity) {}

void ThirdPersonCamera::HandleInput(float dt) {
  if (!m_Camera || !m_Registry) return;

  // Mouse Look (rotates camera around player)
  int mx, my;
  Input::GetMouseDelta(mx, my);
  
  m_CameraYaw -= mx * m_MouseSensitivity;
  m_CameraPitch -= my * m_MouseSensitivity;
  
  if (m_CameraPitch > 1.5f) m_CameraPitch = 1.5f;
  if (m_CameraPitch < -1.5f) m_CameraPitch = -1.5f;
}

void ThirdPersonCamera::Update(float dt) {
  if (!m_Camera || !m_Registry) return;

  auto *t = m_Registry->GetComponent<Transform3DComponent>(m_TargetEntity);
  if (!t) return;

  // Calculate camera direction
  float dx = cos(m_CameraYaw) * cos(m_CameraPitch);
  float dy = sin(m_CameraYaw) * cos(m_CameraPitch);
  float dz = sin(m_CameraPitch);

  // Target camera position relative to player
  float targetX = t->x - dx * m_Distance + cos(m_CameraYaw + 1.57f) * m_Shoulder;
  float targetY = t->y - dy * m_Distance + sin(m_CameraYaw + 1.57f) * m_Shoulder;
  float targetZ = t->z + m_Height - dz * m_Distance;

  // Smooth camera movement with lerp
  float smoothSpeed = 8.0f * dt;
  m_Camera->x += (targetX - m_Camera->x) * smoothSpeed;
  m_Camera->y += (targetY - m_Camera->y) * smoothSpeed;
  m_Camera->z += (targetZ - m_Camera->z) * smoothSpeed;

  m_Camera->yaw = m_CameraYaw;
  m_Camera->pitch = m_CameraPitch;
}

// ============================================================================
// TopDownCamera Implementation
// ============================================================================

TopDownCamera::TopDownCamera(Camera* camera, Registry* registry, Entity targetEntity)
    : m_Camera(camera), m_Registry(registry), m_TargetEntity(targetEntity) {}

void TopDownCamera::HandleInput(float dt) {
  // Top-down follow camera doesn't need manual movement input
}

void TopDownCamera::Update(float dt) {
  if (!m_Camera || !m_Registry) return;

  auto *t = m_Registry->GetComponent<Transform3DComponent>(m_TargetEntity);
  if (!t) return;

  // Fixed top-down position relative to player
  // Offset "North" (-Y) to look "South" (+Y)
  float targetX = t->x;
  float targetY = t->y - m_Distance; 
  float targetZ = t->z + m_Height;

  // Smooth camera movement
  float smoothSpeed = 5.0f * dt;
  m_Camera->x += (targetX - m_Camera->x) * smoothSpeed;
  m_Camera->y += (targetY - m_Camera->y) * smoothSpeed;
  m_Camera->z += (targetZ - m_Camera->z) * smoothSpeed;

  // Point down and South (+Y)
  m_Camera->yaw = M_PI / 2.0f; 
  m_Camera->pitch = -0.98f; // Match height/distance ratio (atan(12/8))
}

} // namespace PixelsEngine
