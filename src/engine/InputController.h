#pragma once
#include "Components.h"
#include "Camera.h"
#include "ECS.h"
#include <SDL2/SDL.h>

namespace PixelsEngine {

// Base input controller for character movement
class InputController {
public:
  virtual ~InputController() = default;
  
  virtual void HandleInput(float dt) = 0;
  
  // Keybind setters
  virtual void SetMoveForwardKey(SDL_Scancode key) { m_KeyForward = key; }
  virtual void SetMoveBackKey(SDL_Scancode key) { m_KeyBack = key; }
  virtual void SetMoveLeftKey(SDL_Scancode key) { m_KeyLeft = key; }
  virtual void SetMoveRightKey(SDL_Scancode key) { m_KeyRight = key; }
  virtual void SetJumpKey(SDL_Scancode key) { m_KeyJump = key; }

protected:
  SDL_Scancode m_KeyForward = SDL_SCANCODE_W;
  SDL_Scancode m_KeyBack = SDL_SCANCODE_S;
  SDL_Scancode m_KeyLeft = SDL_SCANCODE_A;
  SDL_Scancode m_KeyRight = SDL_SCANCODE_D;
  SDL_Scancode m_KeyJump = SDL_SCANCODE_SPACE;
};

// First-person movement controller (moves in camera direction)
class FirstPersonMovementController : public InputController {
public:
  FirstPersonMovementController(Camera* camera, PhysicsComponent* physics);
  ~FirstPersonMovementController() override = default;
  
  void HandleInput(float dt) override;
  
  void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }
  void SetAcceleration(float accel) { m_Acceleration = accel; }
  void SetFriction(float friction) { m_Friction = friction; }
  void SetMaxSpeed(float maxSpeed) { m_MaxSpeed = maxSpeed; }
  void SetJumpForce(float force) { m_JumpForce = force; }

private:
  Camera* m_Camera;
  PhysicsComponent* m_Physics;
  
  float m_MoveSpeed = 10.0f;
  float m_Acceleration = 0.0f;  // Not used in simple mode, but kept for compatibility
  float m_Friction = 1.0f;      // 1.0 = no friction
  float m_MaxSpeed = 20.0f;
  float m_JumpForce = 6.0f;
};

// Third-person movement controller (moves towards cursor/facing direction)
class ThirdPersonMovementController : public InputController {
public:
  ThirdPersonMovementController(Registry* registry, Entity playerEntity);
  ~ThirdPersonMovementController() override = default;
  
  void HandleInput(float dt) override;
  
  void SetAcceleration(float accel) { m_Acceleration = accel; }
  void SetFriction(float friction) { m_Friction = friction; }
  void SetMaxSpeed(float maxSpeed) { m_MaxSpeed = maxSpeed; }
  void SetJumpForce(float force) { m_JumpForce = force; }

private:
  Registry* m_Registry;
  Entity m_PlayerEntity;
  
  float m_Acceleration = 35.0f;
  float m_Friction = 0.92f;
  float m_MaxSpeed = 15.0f;
  float m_JumpForce = 6.0f;
};

} // namespace PixelsEngine
