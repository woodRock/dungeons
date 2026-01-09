#pragma once
#include "Texture.h"
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>
#include <utility>

namespace PixelsEngine {

struct Transform3DComponent {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;   // 0 is floor
  float rot = 0.0f; // Yaw
  float pitch = 0.0f;
};

struct PatrolComponent {
    std::vector<std::pair<float, float>> waypoints;
    int currentWaypointIndex = 0;
    float waitTimer = 0.0f;
    float waitDuration = 5.0f; // Time to wait at each waypoint
    bool movingForward = true; // For ping-pong patrol (optional)
};

struct ObjectiveComponent {
    enum Type { Exit, Collectible };
    Type type = Exit;
    bool isCompleted = false;
    float radius = 1.0f;
    std::string description = "Reach the exit";
};

struct BillboardComponent {
  std::shared_ptr<Texture> texture;
  float scale = 1.0f;
  float width = 0.5f; // World units
  float height = 0.5f;
  bool alwaysFaceCamera = true;
};

struct PhysicsComponent {
  float velX = 0.0f;
  float velY = 0.0f;
  float velZ = 0.0f;
  float gravity = 15.0f;
  bool isGrounded = false;
  bool isWallRunning = false;
  float wallRunTimer = 0.0f;
  float friction = 5.0f;

  // New Movement Tech
  bool isSliding = false;
  float slideTimer = 0.0f;
  int doubleJumpCount = 0;
  int maxDoubleJumps = 1;

  bool isFlying = false;
  uint32_t lastSpaceTime = 0;
};

struct PlayerControlComponent {
  float speed = 5.0f;
  float mouseSensitivity = 0.003f;
  float jumpForce = 6.0f;

  // Checkpoint
  float spawnX = 2.0f;
  float spawnY = 2.0f;
  float spawnZ = 0.5f;
};

struct ColliderComponent {
  float radius = 0.3f;
  float height = 1.0f;
  bool isSolid = true;
};

// AABB (Axis-Aligned Bounding Box) for object collisions
struct HitboxComponent {
  // Dimensions
  float width = 1.0f;   // X axis
  float depth = 1.0f;   // Y axis
  float height = 1.0f;  // Z axis
  
  // Offset from transform position
  float offsetX = 0.0f;
  float offsetY = 0.0f;
  float offsetZ = 0.0f;
  
  // Physics
  bool isSolid = true;      // Blocks movement
  bool isClimbable = false; // Can jump on it
  float climbHeight = 0.5f; // Maximum step height
  
  // Get actual bounds (min and max)
  void GetBounds(float posX, float posY, float posZ, float rotation,
                 float &minX, float &maxX,
                 float &minY, float &maxY,
                 float &minZ, float &maxZ) const {
    
    // Determine effective dimensions based on rotation
    // We assume 90 degree increments for dungeon walls
    float effectiveWidth = width;
    float effectiveDepth = depth;
    
    // Check if rotated approx 90 or 270 degrees (PI/2 or 3*PI/2)
    // PI/2 = 1.57, 3*PI/2 = 4.71
    float absRot = std::abs(rotation);
    // Normalize to 0-PI
    while (absRot > 3.14159f) absRot -= 3.14159f;
    
    if (absRot > 0.78f && absRot < 2.35f) { // roughly 45 to 135 degrees
        effectiveWidth = depth;
        effectiveDepth = width;
    }
    
    minX = posX + offsetX - effectiveWidth / 2.0f;
    maxX = posX + offsetX + effectiveWidth / 2.0f;
    minY = posY + offsetY - effectiveDepth / 2.0f;
    maxY = posY + offsetY + effectiveDepth / 2.0f;
    minZ = posZ + offsetZ;
    maxZ = posZ + offsetZ + height;
  }
};

struct DoorComponent {
  bool isOpen = false;
  float openSpeed = 2.0f;  // Units per second
  float maxOpenDistance = 1.5f;  // How far door opens
  float currentOpenDistance = 0.0f;
  
  // Door swings on this axis
  enum Axis { X, Y, Z };
  Axis swingAxis = Y;
  
  // Original position before opening
  float originalX = 0.0f;
  float originalY = 0.0f;
  float originalZ = 0.0f;
  
  // Hinge offset from door center (negative = left hinge, positive = right hinge)
  float hingeOffsetX = 0.0f;
  float hingeOffsetY = 0.0f;
};

struct ProjectileComponent {
      enum Type { Arrow };  Type type = Arrow;
  float damage = 10.0f;
  bool active = true;
  float lifeTime = 5.0f;
};

struct TargetComponent {
  bool isDestroyed = false;
  int points = 10;
};

struct ParticleComponent {
  float vx, vy, vz;
  float life;
  float maxLife;
  SDL_Color color;
  float size;
};

struct WeaponComponent {
  float cooldown = 0.0f;
  float drawTime = 0.0f; // How long held
  bool isDrawing = false;
};

struct MeshComponent {

  std::string meshName;

  std::string textureName;

  float scaleX = 1.0f;

  float scaleY = 1.0f;

  float scaleZ = 1.0f;

  float offsetX = 0.0f;

  float offsetY = 0.0f;

  float offsetZ = 0.0f;
};

struct InteractableComponent {

  enum Type { Door, Chest };

  Type type;

  bool isOpen = false;
};

struct CharacterComponent {

  enum Type {
    Knight,
    Ranger,
    Mage,
    Barbarian,
    SkeletonMinion,
    SkeletonWarrior,
    SkeletonMage
  };

  Type type;
};

struct ProceduralAnimationComponent {
  float timeOffset = 0.0f;
  float bobAmount = 0.05f;
  float bobSpeed = 2.0f;
  float swayAmount = 0.03f;
  float swaySpeed = 1.5f;
  float baseZ = 0.0f;
};

struct SkeletalAnimationComponent {
  int animationIndex = 0;
  float currentTime = 0.0f;
  float speed = 1.0f;
  bool loop = true; // Whether animation should loop
};

struct EnemyComponent {
  float health = 100.0f;
  float speed = 2.0f;
  int target =
      -1; // Use int to avoid circular or early parse dependency on Entity
};

struct BattleUnitComponent {
  enum Team { Player, Enemy };
  Team team;
  int maxHp = 20;
  int hp = 20;
  int ac = 12; // Armor Class
  int initiative = 0;

  // Actions
  float movement = 6.0f; // Meters
  float currentMovement = 6.0f;
  bool hasAction = true;
  bool hasBonusAction = true;
  bool isSneaking = false;

  // UI State
  bool isSelected = false;
  bool isTurn = false;
  float flashAmount = 0.0f;
  int preferredAction = 2; // Default to Melee (index from ActionType)
};

struct AttachmentComponent {
  std::string meshName;
  std::string textureName;
  std::string boneName = "hand.R";
  float scale = 1.0f;
  float rotX = 0, rotY = 0, rotZ = 0;
  float offX = 0, offY = 0, offZ = 0;
};

struct SkyboxComponent {
  float colorR = 0.53f;
  float colorG = 0.81f;
  float colorB = 0.92f;
  float colorA = 1.0f;
};

} // namespace PixelsEngine
