#include "Simulation.h"
#include "../engine/Input.h"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace PixelsEngine {

Simulation::Simulation(Registry* registry) 
    : m_Registry(registry), m_PlayerEntity(-1) {
  // Find player entity
  auto& transforms = m_Registry->View<Transform3DComponent>();
  auto& players = m_Registry->View<PlayerControlComponent>();
  
  for (auto& pair : players) {
    m_PlayerEntity = pair.first;
    break;
  }
  
  if (m_PlayerEntity == -1) {
    std::cerr << "[Simulation] ERROR: Player entity not found!" << std::endl;
  }
}

Simulation::~Simulation() {}

void Simulation::SimulateKeyDown(SDL_Scancode key) {
  SDL_Event event = {};
  event.type = SDL_KEYDOWN;
  event.key.keysym.scancode = key;
  Input::ProcessEvent(event);
  std::cout << "[Simulation] Key DOWN: " << SDL_GetScancodeName(key) << std::endl;
}

void Simulation::SimulateKeyUp(SDL_Scancode key) {
  SDL_Event event = {};
  event.type = SDL_KEYUP;
  event.key.keysym.scancode = key;
  Input::ProcessEvent(event);
  std::cout << "[Simulation] Key UP: " << SDL_GetScancodeName(key) << std::endl;
}

void Simulation::SimulateKeyPress(SDL_Scancode key, float duration) {
  SimulateKeyDown(key);
  // In a real scenario, you'd wait for 'duration' frames and then release
  SimulateKeyUp(key);
}

Simulation::GameFrame Simulation::CaptureFrame() const {
  GameFrame frame = {};
  frame.description = "";
  
  if (m_PlayerEntity == -1) {
    return frame;
  }
  
  auto* t = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
  auto* p = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
  
  if (t) {
    frame.playerX = t->x;
    frame.playerY = t->y;
    frame.playerZ = t->z;
    frame.playerRot = t->rot;
    frame.playerPitch = t->pitch;
  }
  
  if (p) {
    frame.playerVelX = p->velX;
    frame.playerVelY = p->velY;
    frame.playerVelZ = p->velZ;
  }
  
  return frame;
}

void Simulation::RecordFrame() {
  GameFrame frame = CaptureFrame();
  m_FrameHistory.push_back(frame);
  m_LastFrame = frame;
}

void Simulation::PrintPlayerState() const {
  if (m_PlayerEntity == -1) {
    std::cout << "[Simulation] Player entity not found!" << std::endl;
    return;
  }
  
  auto* t = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
  auto* p = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
  auto* pc = m_Registry->GetComponent<PlayerControlComponent>(m_PlayerEntity);
  auto* w = m_Registry->GetComponent<WeaponComponent>(m_PlayerEntity);
  
  std::cout << "\n[Simulation] === Player State ===" << std::endl;
  
  if (t) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Position: (" << t->x << ", " << t->y << ", " << t->z << ")" << std::endl;
    std::cout << "  Rotation: " << t->rot << " rad, Pitch: " << t->pitch << " rad" << std::endl;
  } else {
    std::cout << "  Transform: MISSING!" << std::endl;
  }
  
  if (p) {
    std::cout << "  Velocity: (" << p->velX << ", " << p->velY << ", " << p->velZ << ")" << std::endl;
    std::cout << "  Grounded: " << (p->isGrounded ? "YES" : "NO") << std::endl;
    std::cout << "  Flying: " << (p->isFlying ? "YES" : "NO") << std::endl;
    std::cout << "  Sliding: " << (p->isSliding ? "YES" : "NO") << std::endl;
  } else {
    std::cout << "  Physics: MISSING!" << std::endl;
  }
  
  if (pc) {
    std::cout << "  Speed: " << pc->speed << ", Jump Force: " << pc->jumpForce << std::endl;
  } else {
    std::cout << "  PlayerControl: MISSING!" << std::endl;
  }
  
  if (w) {
    std::cout << "  Weapon Cooldown: " << w->cooldown << std::endl;
  } else {
    std::cout << "  Weapon: MISSING!" << std::endl;
  }
  
  std::cout << std::endl;
}

void Simulation::PrintComponentStatus() const {
  std::cout << "\n[Simulation] === Component Status ===" << std::endl;
  
  auto& transforms = m_Registry->View<Transform3DComponent>();
  auto& physics = m_Registry->View<PhysicsComponent>();
  auto& players = m_Registry->View<PlayerControlComponent>();
  auto& weapons = m_Registry->View<WeaponComponent>();
  
  std::cout << "  Transforms: " << transforms.size() << std::endl;
  std::cout << "  Physics: " << physics.size() << std::endl;
  std::cout << "  PlayerControl: " << players.size() << std::endl;
  std::cout << "  Weapons: " << weapons.size() << std::endl;
  
  std::cout << std::endl;
}

bool Simulation::ValidatePlayerComponents() const {
  if (m_PlayerEntity == -1) {
    std::cerr << "[Simulation] ERROR: Player entity not found!" << std::endl;
    return false;
  }
  
  auto* t = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
  auto* p = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
  auto* pc = m_Registry->GetComponent<PlayerControlComponent>(m_PlayerEntity);
  auto* w = m_Registry->GetComponent<WeaponComponent>(m_PlayerEntity);
  
  if (!t || !p || !pc || !w) {
    std::cerr << "[Simulation] ERROR: Missing player components!" << std::endl;
    std::cerr << "  Transform: " << (t ? "OK" : "MISSING") << std::endl;
    std::cerr << "  Physics: " << (p ? "OK" : "MISSING") << std::endl;
    std::cerr << "  PlayerControl: " << (pc ? "OK" : "MISSING") << std::endl;
    std::cerr << "  Weapon: " << (w ? "OK" : "MISSING") << std::endl;
    return false;
  }
  
  return true;
}

void Simulation::RunTestWASDMovement() {
  std::cout << "\n[Simulation] ========== TEST: WASD Movement ==========" << std::endl;
  
  if (!ValidatePlayerComponents()) {
    return;
  }
  
  PrintPlayerState();
  
  std::cout << "[Simulation] Testing W (forward)..." << std::endl;
  GameFrame beforeW = CaptureFrame();
  SimulateKeyDown(SDL_SCANCODE_W);
  RecordFrame();
  GameFrame afterW = CaptureFrame();
  
  float distW = std::sqrt(
    (afterW.playerX - beforeW.playerX) * (afterW.playerX - beforeW.playerX) +
    (afterW.playerY - beforeW.playerY) * (afterW.playerY - beforeW.playerY)
  );
  
  std::cout << "  Distance moved: " << distW << std::endl;
  std::cout << "  Velocity: (" << afterW.playerVelX << ", " << afterW.playerVelY << ")" << std::endl;
  
  SimulateKeyUp(SDL_SCANCODE_W);
  RecordFrame();
  
  PrintPlayerState();
  
  std::cout << "[Simulation] ========== TEST COMPLETE ==========" << std::endl;
}

void Simulation::RunTestJump() {
  std::cout << "\n[Simulation] ========== TEST: Jump ==========" << std::endl;
  
  if (!ValidatePlayerComponents()) {
    return;
  }
  
  PrintPlayerState();
  
  auto* p = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
  if (!p || !p->isGrounded) {
    std::cout << "[Simulation] Player must be grounded to jump. Current state:" << std::endl;
    PrintPlayerState();
    return;
  }
  
  std::cout << "[Simulation] Simulating Space key press..." << std::endl;
  SimulateKeyPress(SDL_SCANCODE_SPACE, 1.0f);
  RecordFrame();
  
  PrintPlayerState();
  
  std::cout << "[Simulation] ========== TEST COMPLETE ==========" << std::endl;
}

void Simulation::RunTestRotation() {
  std::cout << "\n[Simulation] ========== TEST: Rotation ==========" << std::endl;
  
  if (!ValidatePlayerComponents()) {
    return;
  }
  
  PrintPlayerState();
  
  std::cout << "[Simulation] Simulating mouse movement..." << std::endl;
  
  auto* t = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
  if (t) {
    float rotBefore = t->rot;
    
    // Simulate mouse delta (this would normally come from SDL_MOUSEMOTION)
    SDL_Event event = {};
    event.type = SDL_MOUSEMOTION;
    event.motion.xrel = 100;  // Mouse moved 100 pixels right
    event.motion.yrel = 50;   // Mouse moved 50 pixels down
    Input::ProcessEvent(event);
    
    RecordFrame();
    
    float rotAfter = t->rot;
    std::cout << "  Rotation before: " << rotBefore << std::endl;
    std::cout << "  Rotation after: " << rotAfter << std::endl;
    std::cout << "  Delta: " << (rotAfter - rotBefore) << std::endl;
  }
  
  PrintPlayerState();
  
  std::cout << "[Simulation] ========== TEST COMPLETE ==========" << std::endl;
}

bool Simulation::IsPlayerMoving() const {
  auto* p = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
  if (!p) return false;
  
  float speed = std::sqrt(p->velX * p->velX + p->velY * p->velY);
  return speed > 0.1f;
}

bool Simulation::DidPlayerMove(float minDistance) const {
  if (m_FrameHistory.size() < 2) {
    return false;
  }
  
  const GameFrame& first = m_FrameHistory.front();
  const GameFrame& last = m_FrameHistory.back();
  
  float distance = std::sqrt(
    (last.playerX - first.playerX) * (last.playerX - first.playerX) +
    (last.playerY - first.playerY) * (last.playerY - first.playerY)
  );
  
  return distance >= minDistance;
}

void Simulation::ResetPlayer() {
  if (m_PlayerEntity == -1) return;
  
  auto* t = m_Registry->GetComponent<Transform3DComponent>(m_PlayerEntity);
  auto* p = m_Registry->GetComponent<PhysicsComponent>(m_PlayerEntity);
  
  if (t) {
    t->x = 0.0f;
    t->y = 0.0f;
    t->z = 2.0f;
    t->rot = 0.0f;
    t->pitch = 0.0f;
  }
  
  if (p) {
    p->velX = 0.0f;
    p->velY = 0.0f;
    p->velZ = 0.0f;
  }
  
  m_FrameHistory.clear();
  std::cout << "[Simulation] Player reset to origin." << std::endl;
}

} // namespace PixelsEngine
