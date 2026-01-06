#include "BattleMode.h"
#include "../engine/Components.h"
#include "../engine/GLTFLoader.h"
#include "../engine/Input.h"
#include "../engine/TextRenderer.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace PixelsEngine;

BattleMode::BattleMode(Registry* registry, GLRenderer* renderer)
    : m_Registry(registry), m_Renderer(renderer) {}

void BattleMode::Init(Camera* camera, Entity& playerEntity) {
    m_Camera = camera;
    
    // 1. Load Assets (Ensure common assets are loaded)
    // Characters
    std::vector<std::string> adventurers = {"Knight", "Ranger", "Mage", "Rogue"};
    std::vector<std::string> skeletons = {"Skeleton_Warrior", "Skeleton_Mage", "Skeleton_Minion"};
    
    // Helper to load if missing
    auto LoadIfMissing = [&](const std::string& name, const std::string& path, const std::string& texName, const std::string& texPath) {
        if (!m_Renderer->GetRenderMesh(name)) {
            m_Renderer->LoadMesh(name, path);
            m_Renderer->LoadTexture(texName, texPath);
            
            // Load Animations (General + Combat + Movement)
            std::string rigPath = (name.find("Skeleton") != std::string::npos) ? 
                "assets/skeletons/Animations/gltf/Rig_Medium/" :
                "assets/adventurers/Animations/gltf/Rig_Medium/"; 
            
            std::vector<std::string> animFiles = {
                "Rig_Medium_General.glb",
                "Rig_Medium_CombatMelee.glb",
                "Rig_Medium_CombatRanged.glb",
                "Rig_Medium_MovementBasic.glb"
            };
            
            std::vector<SkeletalAnimation> allAnims;
            for (const auto& file : animFiles) {
                std::string fullPath = rigPath + file;
                // If specific doesn't exist, try adventurers (shared)
                if (file != "Rig_Medium_General.glb") fullPath = "assets/adventurers/Animations/gltf/Rig_Medium/" + file;
                
                auto anims = GLTFLoader::LoadAnimations(fullPath);
                allAnims.insert(allAnims.end(), anims.begin(), anims.end());
            }

            RenderMesh* rm = m_Renderer->GetRenderMesh(name);
            if (rm) { rm->animations = allAnims; rm->isSkinned = true; }
        }
    };

    LoadIfMissing("Knight", "assets/adventurers/Characters/gltf/Knight.glb", "knight_tex", "assets/adventurers/Textures/knight_texture.png");
    LoadIfMissing("Ranger", "assets/adventurers/Characters/gltf/Ranger.glb", "ranger_tex", "assets/adventurers/Textures/ranger_texture.png");
    LoadIfMissing("Mage", "assets/adventurers/Characters/gltf/Mage.glb", "mage_tex", "assets/adventurers/Textures/mage_texture.png");
    LoadIfMissing("Rogue", "assets/adventurers/Characters/gltf/Rogue.glb", "rogue_tex", "assets/adventurers/Textures/rogue_texture.png");
    
    LoadIfMissing("Skeleton_Warrior", "assets/skeletons/characters/gltf/Skeleton_Warrior.glb", "skeleton_tex", "assets/skeletons/texture/skeleton_texture.png");
    LoadIfMissing("Skeleton_Mage", "assets/skeletons/characters/gltf/Skeleton_Mage.glb", "skeleton_tex", "assets/skeletons/texture/skeleton_texture.png");
    LoadIfMissing("Skeleton_Minion", "assets/skeletons/characters/gltf/Skeleton_Minion.glb", "skeleton_tex", "assets/skeletons/texture/skeleton_texture.png");

    // 2. Load Map
    LoadMap("assets/dungeons/my_dungeon.map");
    
    // 3. Spawn Units
    SpawnCharacter("Knight", 12.0f, -4.0f, BattleUnitComponent::Player);
    SpawnCharacter("Ranger", 14.0f, -5.0f, BattleUnitComponent::Player);
    SpawnCharacter("Mage", 10.0f, -5.0f, BattleUnitComponent::Player);
    
    SpawnCharacter("Skeleton_Warrior", 12.0f, 4.0f, BattleUnitComponent::Enemy);
    SpawnCharacter("Skeleton_Mage", 15.0f, 6.0f, BattleUnitComponent::Enemy);
    SpawnCharacter("Skeleton_Minion", 9.0f, 6.0f, BattleUnitComponent::Enemy);
    
    // 4. Setup Camera
    if (m_Camera) {
        m_Camera->x = 12.0f; m_Camera->y = 0.0f; m_Camera->z = 12.0f;
        m_Camera->yaw = M_PI * 0.5f; 
        m_Camera->pitch = -1.0f; // Look down
    }
    
    StartBattle();
    
    if (!m_TurnOrder.empty()) playerEntity = m_TurnOrder[0];
}

void BattleMode::LoadMap(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) return;
    std::string m, t; float x, y, z, r;
    while (in >> m >> t >> x >> y >> z >> r) {
        if (!m_Renderer->GetRenderMesh(m)) {
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
    unit->hasBonusAction = true;
    unit->isTurn = true;
    
    // Deselect others turn flag
    for (auto e : m_TurnOrder) {
        if (e != current) {
            auto* u = m_Registry->GetComponent<BattleUnitComponent>(e);
            if (u) u->isTurn = false;
        }
    }
    
    if (unit->team == BattleUnitComponent::Player) {
        m_State = PlayerTurn;
        std::cout << "Turn: Player Unit " << current << std::endl;
    } else {
        m_State = EnemyTurn;
        std::cout << "Turn: Enemy Unit " << current << std::endl;
    }
    m_Cursor.active = false;
    m_SelectedAction = None;
}

void BattleMode::Update(float dt, Entity playerEntity) {
    if (m_Camera) {
        float speed = 10.0f * dt;
        if (Input::IsKeyDown(SDL_SCANCODE_W)) m_Camera->y += speed;
        if (Input::IsKeyDown(SDL_SCANCODE_S)) m_Camera->y -= speed;
        if (Input::IsKeyDown(SDL_SCANCODE_A)) m_Camera->x -= speed;
        if (Input::IsKeyDown(SDL_SCANCODE_D)) m_Camera->x += speed;
    }

    if (m_State == Moving) {
        auto* t = m_Registry->GetComponent<Transform3DComponent>(m_AnimState.actor);
        if (t) {
            m_AnimState.timer += dt;
            float p = std::min(1.0f, m_AnimState.timer / m_AnimState.duration);
            t->x = m_AnimState.startX + (m_AnimState.targetX - m_AnimState.startX) * p;
            t->y = m_AnimState.startY + (m_AnimState.targetY - m_AnimState.startY) * p;
            
            if (m_SelectedAction == Jump) {
                float h = sin(p * M_PI) * 2.0f; 
                t->z = h;
            }
            
            if (p >= 1.0f) {
                t->z = 0.0f;
                // Revert to Idle animation
                auto* animComp = m_Registry->GetComponent<SkeletalAnimationComponent>(m_AnimState.actor);
                auto* mesh = m_Registry->GetComponent<MeshComponent>(m_AnimState.actor);
                if (animComp && mesh) {
                    RenderMesh* rm = m_Renderer->GetRenderMesh(mesh->meshName);
                    if (rm) {
                        for(size_t i=0; i<rm->animations.size(); i++) {
                            if (rm->animations[i].name.find("Idle") != std::string::npos) {
                                animComp->animationIndex = (int)i;
                                animComp->currentTime = 0.0f;
                                break;
                            }
                        }
                    }
                }

                auto* unit = m_Registry->GetComponent<BattleUnitComponent>(m_AnimState.actor);
                if (unit && unit->team == BattleUnitComponent::Player) m_State = PlayerTurn;
                else m_State = EnemyTurn; 
                m_SelectedAction = None;
            }
        }
    } else if (m_State == PlayerTurn) {
        HandlePlayerInput();
    } else if (m_State == EnemyTurn) {
        UpdateAI(dt);
    }
    
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
    
    if (m_State == PlayerTurn && m_Cursor.active) {
        if (m_Cursor.hoveredEntity != -1) {
            auto* t = m_Registry->GetComponent<Transform3DComponent>(m_Cursor.hoveredEntity);
            auto* targetUnit = m_Registry->GetComponent<BattleUnitComponent>(m_Cursor.hoveredEntity);
            
            if (t) {
                // Targeting Circle for enemies if attack selected
                if (targetUnit && targetUnit->team == BattleUnitComponent::Enemy && 
                   (m_SelectedAction == Melee || m_SelectedAction == Ranged || m_SelectedAction == Shove)) {
                    m_Renderer->DrawWireCircle(t->x, t->y, 0.1f, 1.0f, {255, 50, 50, 255});
                } else {
                    m_Renderer->DrawWireCube(t->x, t->y, 1.0f, 1.5f, {255, 255, 255, 255});
                }
            }
        } else {
            // Movement marker
            if (m_SelectedAction == Move || m_SelectedAction == None || m_SelectedAction == Jump) {
                 m_Renderer->DrawWireCircle(m_Cursor.x, m_Cursor.y, 0.1f, 0.5f, {50, 255, 50, 255});
            } else {
                 m_Renderer->DrawWireCube(m_Cursor.x, m_Cursor.y, 0.1f, 0.5f, {200, 200, 200, 255});
            }
        }
    }
}

void BattleMode::HandlePlayerInput() {
    // std::cout << "HandlePlayerInput" << std::endl;
    RaycastCursor();
    
    int mx, my; Input::GetMousePosition(mx, my);
    // if (my > 600) return; // Disable UI block for debugging

    if (Input::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
        std::cout << "Click! Action: " << m_SelectedAction << " Hovered: " << m_Cursor.hoveredEntity << std::endl;
        Entity current = m_TurnOrder[m_CurrentTurnIndex];
        auto* unit = m_Registry->GetComponent<BattleUnitComponent>(current);
        auto* t = m_Registry->GetComponent<Transform3DComponent>(current);
        if (!unit) { std::cout << "No unit component!" << std::endl; return; }

        if (m_SelectedAction == Move || m_SelectedAction == None || m_SelectedAction == Jump) {
            std::cout << "Move Action. Cursor: " << m_Cursor.x << "," << m_Cursor.y << " Active: " << m_Cursor.active << std::endl;
            if (m_Cursor.hoveredEntity == -1) {
                // ...
                float dist = sqrt(pow(m_Cursor.x - t->x, 2) + pow(m_Cursor.y - t->y, 2));
                if (dist <= unit->currentMovement) {
                    unit->currentMovement -= dist;
                    m_AnimState.actor = current;
                    m_AnimState.startX = t->x; m_AnimState.startY = t->y;
                    m_AnimState.targetX = m_Cursor.x; m_AnimState.targetY = m_Cursor.y;
                    m_AnimState.duration = dist / 5.0f;
                    m_AnimState.timer = 0.0f;
                    m_State = Moving;
                    t->rot = atan2(m_Cursor.y - t->y, m_Cursor.x - t->x);
                    
                    // Set Run Animation
                    auto* animComp = m_Registry->GetComponent<SkeletalAnimationComponent>(current);
                    auto* mesh = m_Registry->GetComponent<MeshComponent>(current);
                    if (animComp && mesh) {
                        RenderMesh* rm = m_Renderer->GetRenderMesh(mesh->meshName);
                        if (rm) {
                            for(size_t i=0; i<rm->animations.size(); i++) {
                                // "Run" or "Walk" or "Running_A"
                                if (rm->animations[i].name.find("Run") != std::string::npos || 
                                    rm->animations[i].name.find("Walk") != std::string::npos) {
                                    animComp->animationIndex = (int)i;
                                    animComp->currentTime = 0.0f;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        if (m_Cursor.hoveredEntity != -1 && unit->hasAction) {
            auto* target = m_Registry->GetComponent<BattleUnitComponent>(m_Cursor.hoveredEntity);
            if (target && target->team == BattleUnitComponent::Enemy) {
                float dist = sqrt(pow(t->x - m_Registry->GetComponent<Transform3DComponent>(m_Cursor.hoveredEntity)->x, 2) + 
                                  pow(t->y - m_Registry->GetComponent<Transform3DComponent>(m_Cursor.hoveredEntity)->y, 2));
                
                if (m_SelectedAction == Melee || m_SelectedAction == None) {
                    if (dist < 2.0f) ExecuteAction(current, Melee, m_Cursor.hoveredEntity, 0, 0);
                } else if (m_SelectedAction == Ranged) {
                    if (dist < 15.0f) ExecuteAction(current, Ranged, m_Cursor.hoveredEntity, 0, 0);
                } else if (m_SelectedAction == Shove) {
                    if (dist < 2.0f) ExecuteAction(current, Shove, m_Cursor.hoveredEntity, 0, 0);
                }
            }
        }
    }
    
    if (Input::IsKeyPressed(SDL_SCANCODE_SPACE)) {
        NextTurn();
    }
}

void BattleMode::ExecuteAction(Entity actor, ActionType action, Entity target, float x, float y) {
    std::cout << "ExecuteAction: " << action << " on Target: " << target << std::endl;
    auto* uActor = m_Registry->GetComponent<BattleUnitComponent>(actor);
    auto* uTarget = target != -1 ? m_Registry->GetComponent<BattleUnitComponent>(target) : nullptr;
    
    // Animation Trigger
    auto* mesh = m_Registry->GetComponent<MeshComponent>(actor);
    if (mesh) {
        RenderMesh* rm = m_Renderer->GetRenderMesh(mesh->meshName);
        if (rm) {
            std::string animName = "Idle";
            if (action == Melee) animName = "Melee_1H_Attack_Chop";
            else if (action == Ranged) animName = "Ranged_Bow_Shoot"; // Or Ranged_Bow_Release
            else if (action == Shove) animName = "Melee_Unarmed_Attack_Kick"; // or similar
            
            // Find index
            for(size_t i=0; i<rm->animations.size(); i++) {
                if (rm->animations[i].name.find(animName) != std::string::npos) {
                    auto* animComp = m_Registry->GetComponent<SkeletalAnimationComponent>(actor);
                    if (animComp) {
                        animComp->animationIndex = (int)i;
                        animComp->currentTime = 0.0f;
                        // Note: We don't wait for animation to deal damage currently.
                    }
                    break;
                }
            }
        }
    }

    if (action == Melee || action == Ranged) {
        int roll = rand() % 20 + 1;
        int mod = 5;
        if (uActor->isSneaking) { roll = std::max(roll, rand()%20+1); uActor->isSneaking = false; }
        if (roll + mod >= uTarget->ac) {
            int dmg = rand() % 8 + 3;
            uTarget->hp -= dmg;
            std::cout << "Hit! Damage: " << dmg << std::endl;
            if (uTarget->hp <= 0) { m_Registry->DestroyEntity(target); }
        } else {
            std::cout << "Miss!" << std::endl;
        }
        uActor->hasAction = false;
    } else if (action == Shove) {
        auto* t1 = m_Registry->GetComponent<Transform3DComponent>(actor);
        auto* t2 = m_Registry->GetComponent<Transform3DComponent>(target);
        float angle = atan2(t2->y - t1->y, t2->x - t1->x);
        t2->x += cos(angle) * 2.0f;
        t2->y += sin(angle) * 2.0f;
        uActor->hasAction = false;
    }
}

void BattleMode::RenderUI(GLRenderer* ren, TextRenderer* tr, int w, int h) {
    DrawTurnOrder(ren, tr, w, h);
    if (m_State == PlayerTurn) {
        DrawActionBar(ren, tr, w, h);
    }
}

void BattleMode::DrawActionBar(GLRenderer* ren, TextRenderer* tr, int w, int h) {
    int btnW = 80;
    int btnH = 80;
    int gap = 10;
    int count = 7;
    int startX = (w - (count * (btnW + gap))) / 2;
    int y = h - btnH - 20;
    
    struct ActionBtn {
        ActionType type;
        std::string label;
        SDL_Color color;
    };
    
    std::vector<ActionBtn> btns = {
        {Move, "Move", {50, 150, 50, 255}},
        {Melee, "Melee", {150, 50, 50, 255}},
        {Ranged, "Ranged", {150, 100, 50, 255}},
        {Dash, "Dash", {50, 100, 150, 255}},
        {Jump, "Jump", {100, 50, 150, 255}},
        {Sneak, "Sneak", {50, 50, 50, 255}},
        {Shove, "Shove", {150, 150, 50, 255}}
    };
    
    int mx, my;
    Input::GetMousePosition(mx, my);
    bool click = Input::IsMouseButtonPressed(SDL_BUTTON_LEFT);
    
    Entity current = m_TurnOrder[m_CurrentTurnIndex];
    auto* unit = m_Registry->GetComponent<BattleUnitComponent>(current);
    
    for (int i=0; i<count; i++) {
        int x = startX + i * (btnW + gap);
        bool hover = (mx >= x && mx <= x + btnW && my >= y && my <= y + btnH);
        bool selected = (m_SelectedAction == btns[i].type);
        if (btns[i].type == Sneak && unit && unit->isSneaking) selected = true;
        
        SDL_Color c = btns[i].color;
        if (selected) { c.r = std::min(255, c.r+50); c.g = std::min(255, c.g+50); c.b = std::min(255, c.b+50); }
        else if (hover) { c.r = std::min(255, c.r+30); c.g = std::min(255, c.g+30); c.b = std::min(255, c.b+30); }
        
        ren->DrawRect2D(x, y, btnW, btnH, c);
        tr->RenderTextCentered(ren, btns[i].label, x + btnW/2, y + btnH/2, {255, 255, 255, 255});
        
        if (click && hover) {
            if (btns[i].type == Dash && unit && unit->hasAction) {
                unit->currentMovement += unit->movement;
                unit->hasAction = false;
            } else if (btns[i].type == Sneak && unit) {
                unit->isSneaking = !unit->isSneaking;
            } else {
                m_SelectedAction = btns[i].type;
            }
        }
    }
    
    if (m_SelectedAction != None) {
        tr->RenderTextCentered(ren, "Selected: " + btns[(int)m_SelectedAction-1].label, w/2, y - 30, {255, 255, 255, 255});
    }
}

void BattleMode::DrawTurnOrder(GLRenderer* ren, TextRenderer* tr, int w, int h) {
    int startX = w / 2;
    int y = 20;
    int size = 40;
    int gap = 5;
    
    int totalW = (int)m_TurnOrder.size() * (size + gap);
    startX -= totalW / 2;
    
    for (int i=0; i<(int)m_TurnOrder.size(); i++) {
        int x = startX + i * (size + gap);
        Entity e = m_TurnOrder[i];
        
        SDL_Color c = {100, 100, 100, 255};
        if (i == m_CurrentTurnIndex) c = {200, 200, 100, 255}; 
        
        auto* u = m_Registry->GetComponent<BattleUnitComponent>(e);
        if (u) {
            if (u->team == BattleUnitComponent::Player) c.b = std::min(255, c.b+50);
            else c.r = std::min(255, c.r+50);
            if (u->hp <= 0) c = {50, 50, 50, 255}; 
        }
        
        ren->DrawRect2D(x, y, size, size, c);
        
        auto* mesh = m_Registry->GetComponent<MeshComponent>(e);
        if (mesh) {
            ren->RenderThumbnail(mesh->meshName, mesh->textureName, x+2, y+2, size-4);
        }
        
        if (u) {
            float pct = (float)u->hp / (float)u->maxHp;
            pct = std::max(0.0f, std::min(1.0f, pct));
            int barH = (int)(size * pct);
            ren->DrawRect2D(x, y + size - barH, 5, barH, {255, 0, 0, 200});
        }
    }
}

void BattleMode::UpdateAI(float dt) {
    static float aiTimer = 0.0f;
    aiTimer += dt;
    if (aiTimer < 1.0f) return; 
    aiTimer = 0.0f;
    
    Entity current = m_TurnOrder[m_CurrentTurnIndex];
    auto* actor = m_Registry->GetComponent<BattleUnitComponent>(current);
    if (!actor) { NextTurn(); return; }
    
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
            float moveDist = std::min(actor->currentMovement, minDist - 1.5f);
            if (moveDist > 0) {
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
                
                // Set Run Animation for enemy
                auto* animComp = m_Registry->GetComponent<SkeletalAnimationComponent>(current);
                auto* mesh = m_Registry->GetComponent<MeshComponent>(current);
                if (animComp && mesh) {
                    RenderMesh* rm = m_Renderer->GetRenderMesh(mesh->meshName);
                    if (rm) {
                        for(size_t i=0; i<rm->animations.size(); i++) {
                            if (rm->animations[i].name.find("Run") != std::string::npos || 
                                rm->animations[i].name.find("Walk") != std::string::npos) {
                                animComp->animationIndex = (int)i;
                                animComp->currentTime = 0.0f;
                                break;
                            }
                        }
                    }
                }
                
                return;
            }
        }
    }
    
    NextTurn();
}

void BattleMode::RaycastCursor() {
    int mx, my;
    Input::GetMousePosition(mx, my);
    if (!m_Camera) return;
    
    float aspect = 1280.0f / 720.0f; 
    
    float ndcX = (2.0f * mx) / 1280.0f - 1.0f;
    float ndcY = 1.0f - (2.0f * my) / 720.0f;
    
    float tanFov = tan(1.04f / 2.0f);
    
    float vx = ndcX * aspect * tanFov;
    float vy = ndcY * tanFov;
    float vz = -1.0f;
    
    float cy = cos(m_Camera->yaw);
    float sy = sin(m_Camera->yaw);
    float cp = cos(m_Camera->pitch);
    float sp = sin(m_Camera->pitch);
    
    float dx = cy*cp; 
    float dy = sy*cp; 
    float dz = sp;
    
    Vec3 F_gl = {dx, dz, -dy};
    Vec3 R_gl = Vec3::Normalize(Vec3::Cross(F_gl, {0,1,0}));
    Vec3 U_gl = Vec3::Normalize(Vec3::Cross(R_gl, F_gl));
    
    // View direction is (vx, vy, -1). 
    // Camera Basis: Right, Up, Back (-F).
    // World Ray = Right * vx + Up * vy + Back * (-1) = R * vx + U * vy + F
    Vec3 RayDir_gl = Vec3::Normalize(R_gl * vx + U_gl * vy + F_gl);
    Vec3 RayOrigin_gl = {m_Camera->x, m_Camera->z, -m_Camera->y};
    
    // Debug Ray
    static int logTimer = 0;
    if (logTimer++ % 60 == 0) {
         // std::cout << "RayDir Y: " << RayDir_gl.y << " Origin Y: " << RayOrigin_gl.y << std::endl;
    }
    
    if (abs(RayDir_gl.y) > 0.001f) {
        float t = -RayOrigin_gl.y / RayDir_gl.y;
        
        if (t > 0) {
            Vec3 Hit_gl = RayOrigin_gl + RayDir_gl * t;
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
                    if (dist < 1.0f) { 
                        m_Cursor.hoveredEntity = pair.first;
                        break;
                    }
                }
            }
        }
    }
}