#include "AnimationTestMode.h"
#include "../engine/Components.h"
#include "../engine/GLTFLoader.h"
#include "../engine/Input.h"
#include <iostream>

using namespace PixelsEngine;

AnimationTestMode::AnimationTestMode(Registry* registry, GLRenderer* renderer)
    : m_Registry(registry), m_Renderer(renderer) {}

void AnimationTestMode::Init(Camera* camera) {
    if (camera) {
        camera->x = 0; camera->y = -10; camera->z = 2;
        camera->yaw = M_PI * 0.5f;
        camera->pitch = 0;
    }

    m_Renderer->LoadMesh("Knight", "assets/adventurers/Characters/gltf/Knight.glb");
    m_Renderer->LoadTexture("knight_tex", "assets/adventurers/Textures/knight_texture.png");
    m_Renderer->LoadMesh("Sword", "assets/adventurers/Assets/gltf/sword_1handed.gltf");
    m_Renderer->LoadMesh("Bow", "assets/adventurers/Assets/gltf/crossbow_1handed.gltf");

    auto anims = GLTFLoader::LoadAnimations("assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_General.glb");
    RenderMesh* rm = m_Renderer->GetRenderMesh("Knight");
    if (rm) { 
        rm->animations = anims; 
        rm->isSkinned = true; 
        
        std::cout << "--- Knight Bones ---" << std::endl;
        for (auto& b : rm->skeleton.bones) {
            std::cout << "Bone: " << b.name << std::endl;
        }
        
        std::cout << "--- Animations ---" << std::endl;
        for (size_t i=0; i<anims.size(); i++) {
            std::cout << "[" << i << "] " << anims[i].name << std::endl;
            if (anims[i].name.find("Idle") != std::string::npos && m_CurrentAnim == 0) {
                m_CurrentAnim = (int)i;
            }
        }
    }

    m_TestEntity = m_Registry->CreateEntity();
    m_Registry->AddComponent<Transform3DComponent>(m_TestEntity, {0, 0, 0, 1.57f, 0});
    m_Registry->AddComponent<MeshComponent>(m_TestEntity, {"Knight", "knight_tex", 1, 1, 1});
    m_Registry->AddComponent<SkeletalAnimationComponent>(m_TestEntity, {m_CurrentAnim, 0, 1});
    
    AttachmentComponent attach;
    attach.meshName = "Sword";
    attach.textureName = "knight_tex";
    attach.boneName = "hand.r"; // Corrected from logs
    attach.scale = 1.0f;
    attach.rotX = -7.63f;
    attach.rotY = 18.148f;
    attach.rotZ = 1.702f;
    m_Registry->AddComponent<AttachmentComponent>(m_TestEntity, attach);
}

void AnimationTestMode::Update(float dt) {
    auto* anim = m_Registry->GetComponent<SkeletalAnimationComponent>(m_TestEntity);
    auto* attach = m_Registry->GetComponent<AttachmentComponent>(m_TestEntity);

    if (anim) {
        anim->currentTime += dt;
        RenderMesh* rm = m_Renderer->GetRenderMesh("Knight");
        if (rm) m_Renderer->UpdateSkinnedMesh(*rm, anim->animationIndex, anim->currentTime);
    }

    if (Input::IsKeyPressed(SDL_SCANCODE_SPACE)) {
        m_ShowBow = !m_ShowBow;
        if (attach) {
            if (m_ShowBow) {
                attach->meshName = "Bow";
                attach->rotX = -16.896f; attach->rotY = 30.052f; attach->rotZ = 11.404f;
            } else {
                attach->meshName = "Sword";
                attach->rotX = -7.63f; attach->rotY = 18.148f; attach->rotZ = 1.702f;
            }
        }
    }
    
    // Real-time rotation tuning for attachments
    if (attach) {
        float speed = 2.0f * dt;
        if (Input::IsKeyDown(SDL_SCANCODE_U)) attach->rotX += speed;
        if (Input::IsKeyDown(SDL_SCANCODE_I)) attach->rotX -= speed;
        if (Input::IsKeyDown(SDL_SCANCODE_O)) attach->rotY += speed;
        if (Input::IsKeyDown(SDL_SCANCODE_P)) attach->rotY -= speed;
        if (Input::IsKeyDown(SDL_SCANCODE_J)) attach->rotZ += speed;
        if (Input::IsKeyDown(SDL_SCANCODE_K)) attach->rotZ -= speed;

        static float logTimer = 0;
        logTimer += dt;
        if (logTimer > 1.0f) {
            std::cout << "Attach Rot - X: " << attach->rotX << " Y: " << attach->rotY << " Z: " << attach->rotZ << std::endl;
            logTimer = 0;
        }
    }
    
    if (Input::IsKeyPressed(SDL_SCANCODE_RIGHT)) {
        m_CurrentAnim++;
        if (anim) anim->animationIndex = m_CurrentAnim;
        std::cout << "Current Anim: " << m_CurrentAnim << std::endl;
    }
    if (Input::IsKeyPressed(SDL_SCANCODE_LEFT)) {
        m_CurrentAnim--;
        if (anim) anim->animationIndex = m_CurrentAnim;
        std::cout << "Current Anim: " << m_CurrentAnim << std::endl;
    }
}

void AnimationTestMode::RenderUI(GLRenderer* renderer, int w, int h) {
    renderer->DrawRect2D(10, 10, 300, 100, {0, 0, 0, 150});
    // Text rendering would go here if we had a text renderer pointer
}
