#pragma once

#include "../engine/ECS.h"
#include "../engine/Components.h"
#include <vector>
#include <string>
#include <functional>

namespace PixelsEngine {

class Simulation {
public:
  Simulation(Registry* registry);
  ~Simulation();

  // Input Simulation
  void SimulateKeyDown(SDL_Scancode key);
  void SimulateKeyUp(SDL_Scancode key);
  void SimulateKeyPress(SDL_Scancode key, float duration);
  
  // State Recording
  void RecordFrame();
  struct GameFrame {
    float playerX, playerY, playerZ;
    float playerVelX, playerVelY, playerVelZ;
    float playerRot, playerPitch;
    std::string description;
  };
  
  // Test Cases
  void RunTestWASDMovement();
  void RunTestJump();
  void RunTestRotation();
  
  // Debug Output
  void PrintPlayerState() const;
  void PrintComponentStatus() const;
  
  // Utilities
  bool IsPlayerMoving() const;
  bool DidPlayerMove(float minDistance = 0.1f) const;
  void ResetPlayer();
  
  // Access
  const std::vector<GameFrame>& GetFrameHistory() const { return m_FrameHistory; }
  
private:
  Registry* m_Registry;
  PixelsEngine::Entity m_PlayerEntity;
  std::vector<GameFrame> m_FrameHistory;
  GameFrame m_LastFrame;
  
  GameFrame CaptureFrame() const;
  bool ValidatePlayerComponents() const;
};

} // namespace PixelsEngine
