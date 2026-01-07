#include "InputController.h"
#include "Input.h"
#include <cmath>

namespace PixelsEngine {

// ============================================================================
// FirstPersonMovementController Implementation
// ============================================================================

FirstPersonMovementController::FirstPersonMovementController(Camera* camera, PhysicsComponent* physics)
    : m_Camera(camera), m_Physics(physics) {}

void FirstPersonMovementController::HandleInput(float dt) {
  if (!m_Camera || !m_Physics) return;

  float moveSpeed = m_MoveSpeed * dt;
  
  // Get camera forward direction
  float dirX = cos(m_Camera->yaw) * cos(m_Camera->pitch);
  float dirY = sin(m_Camera->yaw) * cos(m_Camera->pitch);
  float dirZ = sin(m_Camera->pitch);
  
  // Get camera right direction
  float rightX = cos(m_Camera->yaw - M_PI / 2.0f);
  float rightY = sin(m_Camera->yaw - M_PI / 2.0f);

  if (Input::IsKeyDown(m_KeyForward)) {
    m_Camera->x += dirX * moveSpeed;
    m_Camera->y += dirY * moveSpeed;
    m_Camera->z += dirZ * moveSpeed;
  }
  if (Input::IsKeyDown(m_KeyBack)) {
    m_Camera->x -= dirX * moveSpeed;
    m_Camera->y -= dirY * moveSpeed;
    m_Camera->z -= dirZ * moveSpeed;
  }
  if (Input::IsKeyDown(m_KeyLeft)) {
    m_Camera->x -= rightX * moveSpeed;
    m_Camera->y -= rightY * moveSpeed;
  }
  if (Input::IsKeyDown(m_KeyRight)) {
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

// ============================================================================
// ThirdPersonMovementController Implementation
// ============================================================================

ThirdPersonMovementController::ThirdPersonMovementController(Registry* registry, Entity playerEntity)
    : m_Registry(registry), m_PlayerEntity(playerEntity) {}

void ThirdPersonMovementController::HandleInput(float dt) {
  if (!m_Registry) return;

  auto *t = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
  auto *phys = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
  
  if (!t || !phys) return;

  // Apply friction/damping for smoother movement
  phys->velX *= m_Friction;
  phys->velY *= m_Friction;

  // Calculate direction towards cursor (player facing direction)
  float playerFacingX = -sin(t->rot);  // Negated
  float playerFacingY = -cos(t->rot);  // Negated
  
  // Calculate perpendicular direction (strafe)
  float perpX = -cos(t->rot);  // Negated
  float perpY = sin(t->rot);   // Negated

  float accel = m_Acceleration * dt;
  
  if (Input::IsKeyDown(m_KeyForward)) {
    phys->velX += playerFacingX * accel;
    phys->velY += playerFacingY * accel;
  }
  if (Input::IsKeyDown(m_KeyBack)) {
    phys->velX -= playerFacingX * accel;
    phys->velY -= playerFacingY * accel;
  }
  if (Input::IsKeyDown(m_KeyLeft)) {
    phys->velX -= perpX * accel;
    phys->velY -= perpY * accel;
  }
  if (Input::IsKeyDown(m_KeyRight)) {
    phys->velX += perpX * accel;
    phys->velY += perpY * accel;
  }

  // Clamp velocity to prevent excessive speed
  float speed = sqrt(phys->velX * phys->velX + phys->velY * phys->velY);
  if (speed > m_MaxSpeed) {
    float scale = m_MaxSpeed / speed;
    phys->velX *= scale;
    phys->velY *= scale;
  }

  if (Input::IsKeyPressed(m_KeyJump) && phys->isGrounded) {
    phys->velZ = m_JumpForce;
    phys->isGrounded = false;
  }
}

} // namespace PixelsEngine
