#include "BattleMode.h"
#include "../engine/Components.h"
#include "../engine/GLTFLoader.h"
#include "../engine/Input.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace PixelsEngine;

BattleMode::BattleMode(Registry* registry, GLRenderer* renderer)
    : m_Registry(registry), m_Renderer(renderer) {}

void BattleMode::Init(Camera* camera, Entity& playerEntity) {
    m_Camera = camera;
    // m_Registry is cleared by DungeonsGame before calling Init
    
    // 1. Load Assets (Ensure common assets are loaded)
    // Characters
    std::vector<std::string> adventurers = {"Knight", "Ranger", "Mage", "Rogue"};
    std::vector<std::string> skeletons = {"Skeleton_Warrior", "Skeleton_Mage", "Skeleton_Minion"};
    
    // Helper to load if missing
    auto LoadIfMissing = [&](const std::string& name, const std::string& path, const std::string& texName, const std::string& texPath) {
        if (!m_Renderer->GetRenderMesh(name)) {
            m_Renderer->LoadMesh(name, path);
            m_Renderer->LoadTexture(texName, texPath);
            // Animations
            std::string animPath = (name.find("Skeleton") != std::string::npos) ? 
                "assets/skeletons/Animations/gltf/Rig_Medium/Rig_Medium_General.glb" :
                "assets/adventurers/Animations/gltf/Rig_Medium/Rig_Medium_General.glb";
            
            auto anims = GLTFLoader::LoadAnimations(animPath);
            RenderMesh* rm = m_Renderer->GetRenderMesh(name);
            if (rm) { rm->animations = anims; rm->isSkinned = true; }
        }
    };

    LoadIfMissing("Knight", "assets/adventurers/Characters/gltf/Knight.glb", "knight_tex", "assets/adventurers/Textures/knight_texture.png");
    LoadIfMissing("Ranger", "assets/adventurers/Characters/gltf/Ranger.glb", "ranger_tex", "assets/adventurers/Textures/ranger_texture.png");
    LoadIfMissing("Mage", "assets/adventurers/Characters/gltf/Mage.glb", "mage_tex", "assets/adventurers/Textures/mage_texture.png");
    LoadIfMissing("Rogue", "assets/adventurers/Characters/gltf/Rogue.glb", "rogue_tex", "assets/adventurers/Textures/rogue_texture.png");
    
    LoadIfMissing("Skeleton_Warrior", "assets/skeletons/characters/gltf/Skeleton_Warrior.glb", "skeleton_tex", "assets/skeletons/texture/skeleton_texture.png");
    LoadIfMissing("Skeleton_Mage", "assets/skeletons/characters/gltf/Skeleton_Mage.glb", "skeleton_tex", "assets/skeletons/texture/skeleton_texture.png");
    LoadIfMissing("Skeleton_Minion", "assets/skeletons/characters/gltf/Skeleton_Minion.glb", "skeleton_tex", "assets/skeletons/texture/skeleton_texture.png");

    // Map Assets (Assumed loaded by ScanAssets or LoadMap will fail gracefully/fallback? LoadMap calls LoadMesh, so we need obj paths)
    // Since we don't have ScanAssets here, LoadMap might need to load mesh on fly. 
    // m_Renderer->LoadMesh handles loading if path is provided. But map file only has name.
    // I need to assume meshes are loaded or load them.
    // DungeonEditor::ScanAssets loaded them. 
    // I'll re-scan dungeon assets.
    // ... Simplified scan ...
    // For now, assume map meshes are standard and load them if needed.
    // Actually LoadMap in BattleMode needs to be robust.
    
    // 2. Load Map
    LoadMap("assets/dungeons/my_dungeon.map");
    
    // 3. Spawn Units
    SpawnCharacter("Knight", 0.0f, 0.0f, BattleUnitComponent::Player);
    SpawnCharacter("Ranger", 2.0f, -1.0f, BattleUnitComponent::Player);
    SpawnCharacter("Mage", -2.0f, -1.0f, BattleUnitComponent::Player);
    
    SpawnCharacter("Skeleton_Warrior", 0.0f, 8.0f, BattleUnitComponent::Enemy);
    SpawnCharacter("Skeleton_Mage", 3.0f, 10.0f, BattleUnitComponent::Enemy);
    SpawnCharacter("Skeleton_Minion", -3.0f, 10.0f, BattleUnitComponent::Enemy);
    
    // 4. Setup Camera
    if (m_Camera) {
        m_Camera->x = 0.0f; m_Camera->y = -5.0f; m_Camera->z = 12.0f;
        m_Camera->yaw = M_PI * 0.5f; 
        m_Camera->pitch = -1.0f; // Look down
    }
    
    StartBattle();
    
    // Assign a dummy player for compatibility
    if (!m_TurnOrder.empty()) playerEntity = m_TurnOrder[0];
}

void BattleMode::LoadMap(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) return;
    std::string m, t; float x, y, z, r;
    while (in >> m >> t >> x >> y >> z >> r) {
        // Ensure mesh is loaded
        if (!m_Renderer->GetRenderMesh(m)) {
            // Try to find it in default path
            m_Renderer->LoadMesh(m, "assets/dungeons/Assets/obj/" + m + ".obj");
        }
        
        auto e = m_Registry->CreateEntity();
        m_Registry->AddComponent<Transform3DComponent>(e, {x, y, z, r, 0});
        m_Registry->AddComponent<MeshComponent>(e, {m, t, 1, 1, 1});
    }
}

void BattleMode::SpawnCharacter(const std::string& mesh, float x, float y, BattleUnitComponent::Team team) {
    auto e = m_Registry->CreateEntity();
    std::string tex = "knight_tex";
    if (mesh == "Ranger") tex = "ranger_tex";
    if (mesh == "Mage") tex = "mage_tex";
    if (mesh == "Rogue") tex = "rogue_tex";
    if (mesh.find("Skeleton") != std::string::npos) tex = "skeleton_tex";
    
    m_Registry->AddComponent<Transform3DComponent>(e, {x, y, 0.0f, team == BattleUnitComponent::Player ? 0.0f : 3.14f, 0.0f});
    m_Registry->AddComponent<MeshComponent>(e, {mesh, tex, 1.0f, 1.0f, 1.0f});
    
    RenderMesh* rm = m_Renderer->GetRenderMesh(mesh);
    if (rm && rm->isSkinned) {
        int idleIdx = 0;
        for (size_t i = 0; i < rm->animations.size(); ++i) {
            if (rm->animations[i].name.find("Idle_A") != std::string::npos) { idleIdx = (int)i; break; }
        }
        m_Registry->AddComponent<SkeletalAnimationComponent>(e, {idleIdx, (float)(rand()%100)*0.01f, 1.0f});
    }
    
    BattleUnitComponent unit;
    unit.team = team;
    unit.hp = 20;
    unit.maxHp = 20;
    unit.movement = 6.0f;
    unit.currentMovement = 6.0f;
    unit.initiative = rand() % 20 + 1;
    m_Registry->AddComponent<BattleUnitComponent>(e, unit);
    m_Registry->AddComponent<ColliderComponent>(e, {0.5f, 2.0f, true});
}

void BattleMode::StartBattle() {
    m_TurnOrder.clear();
    auto view = m_Registry->View<BattleUnitComponent>();
    for (auto& pair : view) m_TurnOrder.push_back(pair.first);
    
    std::sort(m_TurnOrder.begin(), m_TurnOrder.end(), [&](Entity a, Entity b) {
        auto* ua = m_Registry->GetComponent<BattleUnitComponent>(a);
        auto* ub = m_Registry->GetComponent<BattleUnitComponent>(b);
        return ua->initiative > ub->initiative;
    });
    
    m_CurrentTurnIndex = -1;
    NextTurn();
}

void BattleMode::NextTurn() {
    m_CurrentTurnIndex = (m_CurrentTurnIndex + 1) % m_TurnOrder.size();
    Entity current = m_TurnOrder[m_CurrentTurnIndex];
    auto* unit = m_Registry->GetComponent<BattleUnitComponent>(current);
    
    if (!unit || unit->hp <= 0) { NextTurn(); return; } // Skip dead
    
    unit->currentMovement = unit->movement;
    unit->hasAction = true;
    
    if (unit->team == BattleUnitComponent::Player) {
        m_State = PlayerTurn;
        std::cout << "Turn: Player Unit " << current << std::endl;
    } else {
        m_State = EnemyTurn;
        std::cout << "Turn: Enemy Unit " << current << std::endl;
    }
    
    // Reset selection highlight
    m_Cursor.active = false;
}

void BattleMode::Update(float dt, Entity playerEntity) {
    // Camera Panning
    if (m_Camera) {
        float speed = 10.0f * dt;
        if (Input::IsKeyDown(SDL_SCANCODE_W)) m_Camera->y += speed;
        if (Input::IsKeyDown(SDL_SCANCODE_S)) m_Camera->y -= speed;
        if (Input::IsKeyDown(SDL_SCANCODE_A)) m_Camera->x -= speed;
        if (Input::IsKeyDown(SDL_SCANCODE_D)) m_Camera->x += speed;
    }

    // Animation Update (Movement)
    if (m_State == Moving) {
        auto* t = m_Registry->GetComponent<Transform3DComponent>(m_AnimState.actor);
        if (t) {
            m_AnimState.timer += dt;
            float p = std::min(1.0f, m_AnimState.timer / m_AnimState.duration);
            t->x = m_AnimState.startX + (m_AnimState.targetX - m_AnimState.startX) * p;
            t->y = m_AnimState.startY + (m_AnimState.targetY - m_AnimState.startY) * p;
            
            if (p >= 1.0f) {
                // Done moving
                auto* unit = m_Registry->GetComponent<BattleUnitComponent>(m_AnimState.actor);
                if (unit && unit->team == BattleUnitComponent::Player) m_State = PlayerTurn;
                else m_State = EnemyTurn; // Or End Turn?
            }
        }
    } else if (m_State == PlayerTurn) {
        HandlePlayerInput();
    } else if (m_State == EnemyTurn) {
        UpdateAI(dt);
    }
    
    // Update Animations
    auto &skeletalView = m_Registry->View<SkeletalAnimationComponent>();
    for (auto &pair : skeletalView) {
        Entity e = pair.first;
        auto &anim = pair.second;
        auto *mesh = m_Registry->GetComponent<MeshComponent>(e);
        if (mesh) {
            RenderMesh* rm = m_Renderer->GetRenderMesh(mesh->meshName);
            if (rm && rm->isSkinned) {
                anim.currentTime += dt * anim.speed;
                m_Renderer->UpdateSkinnedMesh(*rm, anim.animationIndex, anim.currentTime);
            }
        }
    }
    
    // Draw Cursor (Debug box)
    if (m_State == PlayerTurn && m_Cursor.active) {
        if (m_Cursor.hoveredEntity != -1) {
            // Hover Unit - Red Box
            auto* t = m_Registry->GetComponent<Transform3DComponent>(m_Cursor.hoveredEntity);
            if (t) m_Renderer->DrawWireCube(t->x, t->y, 1.0f, 1.5f, {255, 0, 0, 255});
        } else {
            // Hover Ground - Green Box
            m_Renderer->DrawWireCube(m_Cursor.x, m_Cursor.y, 0.1f, 0.5f, {0, 255, 0, 255});
        }
    }
}

void BattleMode::HandlePlayerInput() {
    RaycastCursor();
    
    if (Input::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
        if (m_Cursor.hoveredEntity != -1) {
            // Select or Attack
            auto* target = m_Registry->GetComponent<BattleUnitComponent>(m_Cursor.hoveredEntity);
            Entity current = m_TurnOrder[m_CurrentTurnIndex];
            auto* actor = m_Registry->GetComponent<BattleUnitComponent>(current);
            
            if (target && actor && target->team == BattleUnitComponent::Enemy && actor->hasAction) {
                // Attack!
                // Check range (melee = 1.5m)
                auto* t1 = m_Registry->GetComponent<Transform3DComponent>(current);
                auto* t2 = m_Registry->GetComponent<Transform3DComponent>(m_Cursor.hoveredEntity);
                float dist = sqrt(pow(t1->x - t2->x, 2) + pow(t1->y - t2->y, 2));
                
                if (dist < 2.0f) {
                    std::cout << "Attacking!" << std::endl;
                    // Roll To Hit
                    int roll = rand() % 20 + 1;
                    int hitMod = 5; // Str mod
                    if (roll + hitMod >= target->ac) {
                        int dmg = rand() % 8 + 3; // 1d8 + 3
                        target->hp -= dmg;
                        std::cout << "Hit! Damage: " << dmg << " HP: " << target->hp << std::endl;
                        if (target->hp <= 0) {
                            std::cout << "Enemy Defeated!" << std::endl;
                            m_Registry->DestroyEntity(m_Cursor.hoveredEntity);
                            // Cleanup m_TurnOrder lazily in NextTurn
                        }
                    } else {
                        std::cout << "Miss! Roll: " << roll << std::endl;
                    }
                    actor->hasAction = false;
                } else {
                    std::cout << "Out of range!" << std::endl;
                }
            }
        } else {
            // Move
            Entity current = m_TurnOrder[m_CurrentTurnIndex];
            auto* unit = m_Registry->GetComponent<BattleUnitComponent>(current);
            auto* t = m_Registry->GetComponent<Transform3DComponent>(current);
            
            if (unit && unit->currentMovement > 0) {
                float dist = sqrt(pow(m_Cursor.x - t->x, 2) + pow(m_Cursor.y - t->y, 2));
                if (dist <= unit->currentMovement) {
                    unit->currentMovement -= dist;
                    // Start Animation
                    m_AnimState.actor = current;
                    m_AnimState.startX = t->x; m_AnimState.startY = t->y;
                    m_AnimState.targetX = m_Cursor.x; m_AnimState.targetY = m_Cursor.y;
                    m_AnimState.duration = dist / 5.0f; // Speed 5 m/s
                    m_AnimState.timer = 0.0f;
                    m_State = Moving;
                    
                    // Face direction
                    t->rot = atan2(m_Cursor.y - t->y, m_Cursor.x - t->x);
                }
            }
        }
    }
    
    if (Input::IsKeyPressed(SDL_SCANCODE_SPACE)) {
        NextTurn();
    }
}

void BattleMode::UpdateAI(float dt) {
    static float aiTimer = 0.0f;
    aiTimer += dt;
    if (aiTimer < 1.0f) return; // Delay
    aiTimer = 0.0f;
    
    // Simple AI: Move to nearest player and attack
    Entity current = m_TurnOrder[m_CurrentTurnIndex];
    auto* actor = m_Registry->GetComponent<BattleUnitComponent>(current);
    if (!actor) { NextTurn(); return; }
    
    // Find target
    Entity target = -1;
    float minDist = 999.0f;
    auto* t1 = m_Registry->GetComponent<Transform3DComponent>(current);
    
    auto view = m_Registry->View<BattleUnitComponent>();
    for (auto& pair : view) {
        if (pair.second.team == BattleUnitComponent::Player) {
            auto* t2 = m_Registry->GetComponent<Transform3DComponent>(pair.first);
            float d = sqrt(pow(t1->x - t2->x, 2) + pow(t1->y - t2->y, 2));
            if (d < minDist) { minDist = d; target = pair.first; }
        }
    }
    
    if (target != -1 && actor->hasAction) {
        if (minDist < 2.0f) {
            // Attack
            auto* tgt = m_Registry->GetComponent<BattleUnitComponent>(target);
            int roll = rand() % 20 + 1;
            if (roll + 4 >= tgt->ac) {
                tgt->hp -= rand() % 6 + 2;
                std::cout << "Enemy Hits Player! HP: " << tgt->hp << std::endl;
                if (tgt->hp <= 0) m_Registry->DestroyEntity(target);
            } else {
                std::cout << "Enemy Misses!" << std::endl;
            }
            actor->hasAction = false;
        } else {
            // Move
            float moveDist = std::min(actor->currentMovement, minDist - 1.5f);
            if (moveDist > 0) {
                // Vector towards target
                auto* t2 = m_Registry->GetComponent<Transform3DComponent>(target);
                float angle = atan2(t2->y - t1->y, t2->x - t1->x);
                
                m_AnimState.actor = current;
                m_AnimState.startX = t1->x; m_AnimState.startY = t1->y;
                m_AnimState.targetX = t1->x + cos(angle) * moveDist;
                m_AnimState.targetY = t1->y + sin(angle) * moveDist;
                m_AnimState.duration = moveDist / 5.0f;
                m_AnimState.timer = 0.0f;
                m_State = Moving;
                t1->rot = angle;
                actor->currentMovement -= moveDist;
                return; // Return to let Update handle movement animation
            }
        }
    }
    
    NextTurn();
}

void BattleMode::RaycastCursor() {
    int mx, my;
    Input::GetMousePosition(mx, my);
    if (!m_Camera) return;
    
    // Unproject
    // Need viewport size. Assuming 1280x720 for logical scale
    // m_Camera has width/height?
    // Camera doesn't store viewport.
    // Assume window size from Input?
    // Using crude raycast for top-down:
    // Perspective projection.
    
    // Simplification: Calculate world pos on Z=0 plane directly if camera is simple
    // Ray origin = camera pos
    // Ray dir: from screen point.
    
    // Let's use camera vectors.
    // Normalized Device Coordinates:
    // x: -1 to 1, y: 1 to -1 (SDL y is down)
    // We need Aspect Ratio.
    float aspect = 1280.0f / 720.0f; // TODO: Get from Camera or Renderer
    
    float ndcX = (2.0f * mx) / 1280.0f - 1.0f;
    float ndcY = 1.0f - (2.0f * my) / 720.0f;
    
    // FOV 60 deg (1.04 rad)
    float tanFov = tan(1.04f / 2.0f);
    
    // View Space Ray
    float vx = ndcX * aspect * tanFov;
    float vy = ndcY * tanFov;
    float vz = -1.0f;
    
    // World Space Ray: Inverse View Matrix * View Ray
    // View Matrix: LookAt(eye, center, up)
    // Inverse View is roughly: Right * vx + Up * vy + Forward * vz + Eye
    
    Vec3 f = {cos(m_Camera->yaw)*cos(m_Camera->pitch), sin(m_Camera->yaw)*cos(m_Camera->pitch), sin(m_Camera->pitch)};
    Vec3 r = {sin(m_Camera->yaw), -cos(m_Camera->yaw), 0}; // Cross(f, up) - approximate for simple camera
    // Wait, let's use Math3D.h if available or recompute
    // Math3D LookAt implementation:
    // f = normalize(center - eye)
    // s = normalize(cross(f, up))
    // u = cross(s, f)
    // R = [s, u, -f]
    
    // My camera uses yaw/pitch.
    // Forward
    float cy = cos(m_Camera->yaw);
    float sy = sin(m_Camera->yaw);
    float cp = cos(m_Camera->pitch);
    float sp = sin(m_Camera->pitch);
    
    Vec3 F = {cy*cp, sy*cp, sp};
    Vec3 R = {sy, -cy, 0}; // Cross F, Z-up? No, standard is Y-up in my engine?
    // In Init: m_Camera->z is height. So Z is up.
    // Renderer LookAt:
    // Vec3 glEye = {cam.x, cam.z, -cam.y};
    // This implies a coordinate swap in Renderer.
    // Renderer converts game coords (X,Y,Z) to GL coords (X, Z, -Y)?
    // Let's look at GLRenderer::Render again.
    // Vec3 glEye = {cam.x, cam.z, -cam.y};
    // Mat4 view = Mat4::LookAt(glEye, {glEye.x+dx, glEye.y+dz, glEye.z-dy}, {0,1,0});
    // Game X -> GL X
    // Game Y -> GL -Z
    // Game Z -> GL Y
    
    // So if I want to raycast in GAME space (X, Y, Z-up).
    // I should do it in GL space and convert back.
    
    // GL Ray:
    // View Ray in GL space
    // Eye = (cam.x, cam.z, -cam.y)
    // Basis vectors in GL:
    // F_gl = (dx, dz, -dy)
    // R_gl = Cross(F_gl, (0,1,0))
    // U_gl = Cross(R_gl, F_gl)
    
    float dx = cy*cp; 
    float dy = sy*cp; 
    float dz = sp;
    
    // GL Forward vector
    Vec3 F_gl = {dx, dz, -dy};
    Vec3 R_gl = Vec3::Normalize(Vec3::Cross(F_gl, {0,1,0}));
    Vec3 U_gl = Vec3::Normalize(Vec3::Cross(R_gl, F_gl));
    
    Vec3 RayDir_gl = Vec3::Normalize(R_gl * vx + U_gl * vy + F_gl * vz);
    Vec3 RayOrigin_gl = {m_Camera->x, m_Camera->z, -m_Camera->y};
    
    // Intersect with Plane Y=0 (GL Y is Game Z)
    // Ray: O + D * t
    // O.y + D.y * t = 0  => t = -O.y / D.y
    
    if (abs(RayDir_gl.y) > 0.001f) {
        float t = -RayOrigin_gl.y / RayDir_gl.y;
        if (t > 0) {
            Vec3 Hit_gl = RayOrigin_gl + RayDir_gl * t;
            // Convert back to Game Space
            // Game X = GL X
            // Game Z = GL Y (should be 0)
            // Game Y = -GL Z
            m_Cursor.x = Hit_gl.x;
            m_Cursor.y = -Hit_gl.z;
            m_Cursor.z = 0;
            m_Cursor.active = true;
            
            // Check entities
            m_Cursor.hoveredEntity = -1;
            auto view = m_Registry->View<ColliderComponent>();
            for (auto& pair : view) {
                auto* trans = m_Registry->GetComponent<Transform3DComponent>(pair.first);
                if (trans) {
                    float dist = sqrt(pow(m_Cursor.x - trans->x, 2) + pow(m_Cursor.y - trans->y, 2));
                    if (dist < 1.0f) { // Radius
                        m_Cursor.hoveredEntity = pair.first;
                        break;
                    }
                }
            }
        }
    }
}
