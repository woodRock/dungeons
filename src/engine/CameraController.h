#pragma once
#include "Camera.h"
#include "../engine/ECS.h"

namespace PixelsEngine {

class CameraController {
public:
  virtual ~CameraController() = default;
  
  // Update camera position/rotation based on internal state
  virtual void Update(float dt) = 0;
  
  // Handle input (mouse, keyboard) each frame
  virtual void HandleInput(float dt) = 0;
  
  // Get camera yaw (return 0.0f by default for cameras that don't track it)
  virtual float GetYaw() const { return 0.0f; }
};

// First-person camera with WASD movement in camera direction
class FirstPersonCamera : public CameraController {
public:
  FirstPersonCamera(Camera* camera);
  ~FirstPersonCamera() override = default;
  
  void Update(float dt) override;
  void HandleInput(float dt) override;
  
  void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }
  void SetMouseSensitivity(float sens) { m_MouseSensitivity = sens; }
  void SetInventoryOpen(bool open) { m_InventoryOpen = open; }

private:
  Camera* m_Camera;
  float m_MoveSpeed = 10.0f;
  float m_MouseSensitivity = 0.003f;
  bool m_InventoryOpen = false;
};

// Third-person camera orbiting a target entity
class ThirdPersonCamera : public CameraController {
public:
  ThirdPersonCamera(Camera* camera, Registry* registry, Entity targetEntity);
  ~ThirdPersonCamera() override = default;
  
  void Update(float dt) override;
  void HandleInput(float dt) override;
  
  void SetDistance(float dist) { m_Distance = dist; }
  void SetShoulder(float shoulder) { m_Shoulder = shoulder; }
  void SetHeight(float height) { m_Height = height; }
  void SetMouseSensitivity(float sens) { m_MouseSensitivity = sens; }
  
  float GetYaw() const override { return m_CameraYaw; }
  float GetCameraYaw() const { return m_CameraYaw; }
  float GetCameraPitch() const { return m_CameraPitch; }

private:
  Camera* m_Camera;
  Registry* m_Registry;
  Entity m_TargetEntity;
  
  float m_Distance = 2.22f;
  float m_Shoulder = -0.662f;
  float m_Height = 1.724f;
  float m_MouseSensitivity = 0.003f;
  
  float m_CameraYaw = 0.0f;
  float m_CameraPitch = 0.0f;
};

// Top-down isometric-style camera
class TopDownCamera : public CameraController {
public:
  TopDownCamera(Camera* camera);
  ~TopDownCamera() override = default;
  
  void Update(float dt) override;
  void HandleInput(float dt) override;
  
  void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }

private:
  Camera* m_Camera;
  float m_MoveSpeed = 10.0f;
};

} // namespace PixelsEngine
