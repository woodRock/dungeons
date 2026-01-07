#include "GLRenderer.h"
#include "Components.h"
#include "GLTFLoader.h"
#include "OBJLoader.h"
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <iostream>

namespace PixelsEngine {

GLRenderer::GLRenderer() : m_Width(800), m_Height(600), m_DefaultTexture(0) {}
GLRenderer::~GLRenderer() {}

void GLRenderer::Init(int width, int height) {
  m_Width = width;
  m_Height = height;
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  const char *vShader = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
    layout (location = 2) in vec3 aNormal;
    layout (location = 3) in ivec4 aJoints;
    layout (location = 4) in vec4 aWeights;

    out vec2 TexCoord;
    out vec3 Normal;
    out vec3 FragPos;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 jointMatrices[64];
    uniform bool useSkinning;

    void main() {
        mat4 skinMatrix = mat4(1.0);
        if (useSkinning) {
            skinMatrix = 
                aWeights.x * jointMatrices[aJoints.x] +
                aWeights.y * jointMatrices[aJoints.y] +
                aWeights.z * jointMatrices[aJoints.z] +
                aWeights.w * jointMatrices[aJoints.w];
        }
        vec4 worldPos = model * skinMatrix * vec4(aPos, 1.0);
        gl_Position = projection * view * worldPos;
        TexCoord = aTexCoord;
        Normal = mat3(model) * mat3(skinMatrix) * aNormal;
        FragPos = vec3(worldPos);
    }
  )";

  const char *fShader = R"(
      #version 330 core
      out vec4 FragColor;
      in vec2 TexCoord;
      in vec3 Normal;
      in vec3 FragPos;
      uniform sampler2D ourTexture;
      uniform vec3 lightPos;
      uniform float alpha;
      uniform bool useFlatColor;
      uniform vec4 flatColor;
      uniform vec3 flashColor;
      uniform float flashAmount;
  
      void main() {
          if (useFlatColor) {
              FragColor = flatColor;
          } else {
              vec3 ambient = 0.3 * vec3(1.0);
              vec3 norm = normalize(Normal);
              vec3 lightDir = normalize(lightPos - FragPos);
              vec3 diffuse = max(dot(norm, lightDir), 0.0) * vec3(1.0);
              vec4 texColor = texture(ourTexture, TexCoord);
              
              vec3 finalColor = (ambient + diffuse) * texColor.rgb;
              if (length(texColor.rgb) < 0.01) finalColor = vec3(1.0, 0.0, 1.0);
              
              // Apply flash
              finalColor = mix(finalColor, flashColor, flashAmount);
              
              FragColor = vec4(finalColor, alpha);
          }
      }
    )";

  m_Shader = std::make_unique<Shader>(vShader, fShader);
  m_Shader->Use();
  m_Shader->SetFloat("alpha", 1.0f);
  m_Shader->SetInt("useFlatColor", 0);
  m_Shader->SetVec3("flashColor", 1.0f, 0.0f, 0.0f);
  m_Shader->SetFloat("flashAmount", 0.0f);
  glGenTextures(1, &m_DefaultTexture);
  glBindTexture(GL_TEXTURE_2D, m_DefaultTexture);
  unsigned char white[] = {255, 255, 255, 255};
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               white);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  InitUI();
}

void GLRenderer::InitUI() {
  const char *vUI = R"(#version 330 core
        layout (location = 0) in vec2 aPos; layout (location = 1) in vec2 aTexCoord;
        out vec2 TexCoord; uniform mat4 projection; uniform mat4 model;
        void main() { gl_Position = projection * model * vec4(aPos, 0.0, 1.0); TexCoord = aTexCoord; })";
  const char *fUI = R"(#version 330 core
        out vec4 FragColor; in vec2 TexCoord; uniform sampler2D uiTexture; uniform vec4 color; uniform bool useTexture;
        void main() { FragColor = useTexture ? texture(uiTexture, TexCoord) * color : color; })";
  m_UIShader = std::make_unique<Shader>(vUI, fUI);
  float qV[] = {0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0,
                0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0};
  glGenVertexArrays(1, &m_QuadVAO);
  glGenBuffers(1, &m_QuadVBO);
  glBindVertexArray(m_QuadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(qV), qV, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * 4, (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * 4, (void *)(2 * 4));
  glBindVertexArray(0);
}

void GLRenderer::DrawRect2D(int x, int y, int w, int h, SDL_Color color) {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  m_UIShader->Use();
  float L = 0, R = (float)m_Width, B = (float)m_Height, T = 0;
  float ortho[16] = {2.0f / (R - L),
                     0,
                     0,
                     0,
                     0,
                     2.0f / (T - B),
                     0,
                     0,
                     0,
                     0,
                     -1,
                     0,
                     -(R + L) / (R - L),
                     -(T + B) / (T - B),
                     0,
                     1};
  m_UIShader->SetMat4("projection", ortho);
  Mat4 model = Mat4::Translate({(float)x, (float)y, 0}) *
               Mat4::Scale({(float)w, (float)h, 1});
  m_UIShader->SetMat4("model", model.m);
  float c[4] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
                color.a / 255.0f};
  glUniform4fv(glGetUniformLocation(m_UIShader->ID, "color"), 1, c);
  m_UIShader->SetInt("useTexture", 0);
  glBindVertexArray(m_QuadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
}

void GLRenderer::DrawTexture2D(unsigned int texID, int x, int y, int w, int h,
                               SDL_Color color) {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  m_UIShader->Use();
  float L = 0, R = (float)m_Width, B = (float)m_Height, T = 0;
  float ortho[16] = {2.0f / (R - L),
                     0,
                     0,
                     0,
                     0,
                     2.0f / (T - B),
                     0,
                     0,
                     0,
                     0,
                     -1,
                     0,
                     -(R + L) / (R - L),
                     -(T + B) / (T - B),
                     0,
                     1};
  m_UIShader->SetMat4("projection", ortho);
  Mat4 model = Mat4::Translate({(float)x, (float)y, 0}) *
               Mat4::Scale({(float)w, (float)h, 1});
  m_UIShader->SetMat4("model", model.m);
  float c[4] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
                color.a / 255.0f};
  glUniform4fv(glGetUniformLocation(m_UIShader->ID, "color"), 1, c);
  m_UIShader->SetInt("useTexture", 1);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texID);
  glBindVertexArray(m_QuadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
}

void GLRenderer::UpdateSkinnedMesh(RenderMesh &rm, int animIdx, float time) {
  if (!rm.isSkinned)
    return;

  // 1. Start with Local Matrices from Bind Pose
  std::vector<Mat4> locals(rm.skeleton.bones.size());
  for (size_t i = 0; i < locals.size(); i++) {
    memcpy(locals[i].m, rm.skeleton.bones[i].localMatrix, 64);
  }

  // 2. Override Locals with Animation Data
  if (animIdx >= 0 && animIdx < (int)rm.animations.size()) {
    auto &anim = rm.animations[animIdx];
    float animTime = (anim.duration > 0) ? fmod(time, anim.duration) : 0;

    // Group channels by bone to compose T/R/S correctly
    struct BoneTransform {
      Vec3 pos = {0, 0, 0};
      bool hasPos = false;
      Vec4 rot = {0, 0, 0, 1};
      bool hasRot = false;
      Vec3 scl = {1, 1, 1};
      bool hasScl = false;
    };
    std::vector<BoneTransform> transforms(rm.skeleton.bones.size());

    for (auto &chan : anim.channels) {
      int bIdx = -1;
      for (size_t i = 0; i < rm.skeleton.bones.size(); i++) {
        if (rm.skeleton.bones[i].name == chan.boneName) {
          bIdx = (int)i;
          break;
        }
      }
      if (bIdx == -1 || chan.keyframes.empty())
        continue;

      int k1 = 0, k2 = 0;
      if (chan.keyframes.size() > 1) {
        for (size_t i = 0; i < chan.keyframes.size() - 1; i++) {
          if (animTime >= chan.keyframes[i].time &&
              animTime <= chan.keyframes[i + 1].time) {
            k1 = (int)i;
            k2 = (int)i + 1;
            break;
          }
        }
      }
      float t = (k1 == k2)
                    ? 0
                    : (animTime - chan.keyframes[k1].time) /
                          (chan.keyframes[k2].time - chan.keyframes[k1].time);

      if (chan.path == AnimationChannel::Translation) {
        Vec3 v1 = {chan.keyframes[k1].value[0], chan.keyframes[k1].value[1],
                   chan.keyframes[k1].value[2]};
        Vec3 v2 = {chan.keyframes[k2].value[0], chan.keyframes[k2].value[1],
                   chan.keyframes[k2].value[2]};
        transforms[bIdx].pos = v1 * (1 - t) + v2 * t;
        transforms[bIdx].hasPos = true;
      } else if (chan.path == AnimationChannel::Rotation) {
        Vec4 q1 = {chan.keyframes[k1].value[0], chan.keyframes[k1].value[1],
                   chan.keyframes[k1].value[2], chan.keyframes[k1].value[3]};
        Vec4 q2 = {chan.keyframes[k2].value[0], chan.keyframes[k2].value[1],
                   chan.keyframes[k2].value[2], chan.keyframes[k2].value[3]};
        Vec4 q = {q1.x * (1 - t) + q2.x * t, q1.y * (1 - t) + q2.y * t,
                  q1.z * (1 - t) + q2.z * t, q1.w * (1 - t) + q2.w * t};
        float len = sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
        if (len > 0.0001f) {
          q.x /= len;
          q.y /= len;
          q.z /= len;
          q.w /= len;
        } else
          q = {0, 0, 0, 1};
        transforms[bIdx].rot = q;
        transforms[bIdx].hasRot = true;
      }
    }

    // Apply composed transforms to locals
    for (size_t i = 0; i < locals.size(); i++) {
      if (transforms[i].hasRot) {
        // Replace rotation but keep existing translation
        float tx = locals[i].m[12];
        float ty = locals[i].m[13];
        float tz = locals[i].m[14];
        locals[i] = Mat4::FromQuaternion(transforms[i].rot);
        locals[i].m[12] = tx;
        locals[i].m[13] = ty;
        locals[i].m[14] = tz;
      }
      if (transforms[i].hasPos) {
        // Replace translation
        locals[i].m[12] = transforms[i].pos.x;
        locals[i].m[13] = transforms[i].pos.y;
        locals[i].m[14] = transforms[i].pos.z;
      }
    }
  }

  // 3. Compute Global Hierarchy Recursively
  std::vector<Mat4> globals(rm.skeleton.bones.size());
  std::vector<bool> computed(rm.skeleton.bones.size(), false);

  auto computeGlobal = [&](auto self, int i) -> void {
    if (computed[i])
      return;
    int p = rm.skeleton.bones[i].parentIndex;
    if (p == -1) {
      globals[i] = locals[i];
    } else {
      self(self, p);
      globals[i] = globals[p] * locals[i];
    }
    computed[i] = true;
  };

  for (size_t i = 0; i < globals.size(); i++) {
    computeGlobal(computeGlobal, (int)i);
  }

  // 4. Final Skin Matrices = GlobalTransform * InverseBindMatrix
  rm.boneGlobalMatrices.resize(globals.size() * 16);
  for (size_t i = 0; i < globals.size(); i++) {
    Mat4 ibm;
    memcpy(ibm.m, rm.skeleton.bones[i].inverseBindMatrix, 64);
    Mat4 finalMatrix = globals[i] * ibm;
    memcpy(&rm.skeleton.jointMatrices[i * 16], finalMatrix.m, 64);
    memcpy(&rm.boneGlobalMatrices[i * 16], globals[i].m, 64);
  }
}

RenderMesh *GLRenderer::GetRenderMesh(const std::string &name) {
  return m_Meshes.count(name) ? &m_Meshes[name] : nullptr;
}

void GLRenderer::Render(SDL_Window *win, const Camera &cam, Registry &reg,
                        bool swap) {
  int dw, dh;
  SDL_GL_GetDrawableSize(win, &dw, &dh);
  glViewport(0, 0, dw, dh);
  glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_Shader->Use();
  Vec3 glEye = {cam.x, cam.z, -cam.y};
  float dx = cos(cam.yaw) * cos(cam.pitch), dy = sin(cam.yaw) * cos(cam.pitch),
        dz = sin(cam.pitch);
  Mat4 view = Mat4::LookAt(glEye, {glEye.x + dx, glEye.y + dz, glEye.z - dy},
                           {0, 1, 0});
  Mat4 proj = Mat4::Perspective(1.04f, (float)m_Width / m_Height, 0.1f, 100.0f);
  m_Shader->SetMat4("view", view.m);
  m_Shader->SetMat4("projection", proj.m);
  m_Shader->SetVec3("lightPos", glEye.x, glEye.y, glEye.z);
  m_Shader->SetInt("ourTexture", 0);
  m_Shader->SetInt("useAlpha", 0);
  auto &viewGroup = reg.View<MeshComponent>();
  for (auto &pair : viewGroup) {
    auto &mc = pair.second;
    auto *t = reg.GetComponent<Transform3DComponent>(pair.first);
    auto *unit = reg.GetComponent<BattleUnitComponent>(pair.first);

    if (m_Meshes.count(mc.meshName)) {
      RenderMesh &rm = m_Meshes[mc.meshName];
      Mat4 model = Mat4::Translate({t ? t->x : 0, t ? t->z : 0, t ? -t->y : 0});
      if (t)
        model = model * Mat4::RotateY(-t->rot);
      model = model * Mat4::Scale({mc.scaleX, mc.scaleZ, mc.scaleY});
      m_Shader->SetMat4("model", model.m);

      if (unit && unit->flashAmount > 0.0f) {
        m_Shader->SetFloat("flashAmount", unit->flashAmount);
      } else {
        m_Shader->SetFloat("flashAmount", 0.0f);
      }

      if (rm.isSkinned) {
        m_Shader->SetInt("useSkinning", 1);
        std::vector<float> jm(64 * 16, 0.0f);
        for (int i = 0; i < 64; i++)
          jm[i * 16] = jm[i * 16 + 5] = jm[i * 16 + 10] = jm[i * 16 + 15] =
              1.0f;
        if (!rm.skeleton.jointMatrices.empty())
          memcpy(jm.data(), rm.skeleton.jointMatrices.data(),
                 std::min(jm.size() * sizeof(float),
                          rm.skeleton.jointMatrices.size() * sizeof(float)));
        GLint loc = glGetUniformLocation(m_Shader->ID, "jointMatrices");
        if (loc != -1)
          glUniformMatrix4fv(loc, 64, GL_FALSE, jm.data());
      } else
        m_Shader->SetInt("useSkinning", 0);
      glActiveTexture(GL_TEXTURE0);
      GLuint texID = (m_Textures.count(mc.textureName))
                         ? m_Textures[mc.textureName]
                         : m_DefaultTexture;
      glBindTexture(GL_TEXTURE_2D, texID);
      static bool loggedOnce = false;
      if (!loggedOnce && rm.isSkinned) {
        std::cout << "Rendering skinned mesh: " << mc.meshName 
                  << " with texture: " << mc.textureName 
                  << " (ID: " << texID << ", found: " << m_Textures.count(mc.textureName) 
                  << ")" << std::endl;
        loggedOnce = true;
      }
      glBindVertexArray(rm.VAO);
      glDrawElements(GL_TRIANGLES, rm.indexCount, GL_UNSIGNED_INT, 0);

      // Handle Attachments (Weapons)
      auto *attach = reg.GetComponent<AttachmentComponent>(pair.first);
      if (attach && rm.isSkinned && !rm.boneGlobalMatrices.empty()) {
        int bIdx = -1;
        for (size_t i = 0; i < rm.skeleton.bones.size(); i++) {
          if (rm.skeleton.bones[i].name == attach->boneName) {
            bIdx = (int)i;
            break;
          }
        }

        if (bIdx == -1) {
          static bool logged = false;
          if (!logged) {
            std::cout << "Attachment Error: Bone '" << attach->boneName
                      << "' not found in mesh '" << mc.meshName << "'"
                      << std::endl;
            std::cout << "Available bones: ";
            for (auto &b : rm.skeleton.bones)
              std::cout << b.name << ", ";
            std::cout << std::endl;
            logged = true;
          }
        }

        if (bIdx != -1) {
          Mat4 boneGlobal;
          memcpy(boneGlobal.m, &rm.boneGlobalMatrices[bIdx * 16], 64);
          Mat4 weaponModel =
              model * boneGlobal * Mat4::RotateX(attach->rotX) *
              Mat4::RotateY(attach->rotY) * Mat4::RotateZ(attach->rotZ) *
              Mat4::Scale({attach->scale, attach->scale, attach->scale});
          m_Shader->SetMat4("model", weaponModel.m);
          m_Shader->SetInt("useSkinning", 0);
          if (m_Meshes.count(attach->meshName)) {
            RenderMesh &wrm = m_Meshes[attach->meshName];
            glBindTexture(GL_TEXTURE_2D, (m_Textures.count(attach->textureName))
                                             ? m_Textures[attach->textureName]
                                             : m_DefaultTexture);
            glBindVertexArray(wrm.VAO);
            glDrawElements(GL_TRIANGLES, wrm.indexCount, GL_UNSIGNED_INT, 0);
          }
        }
      }
    }
  }
  if (swap)
    SDL_GL_SwapWindow(win);
}

bool GLRenderer::LoadMesh(const std::string &name, const std::string &path) {
  Mesh m;
  bool isGltf = (path.find(".glb") != std::string::npos ||
                 path.find(".gltf") != std::string::npos);
  if (!(isGltf ? GLTFLoader::Load(path, m) : OBJLoader::Load(path, m)))
    return false;
  RenderMesh rm;
  rm.indexCount = (unsigned int)m.indices.size();
  rm.isSkinned = m.isSkinned;
  rm.skeleton = m.skeleton;
  rm.animations = m.animations;
  glGenVertexArrays(1, &rm.VAO);
  glGenBuffers(1, &rm.VBO);
  glGenBuffers(1, &rm.EBO);
  glBindVertexArray(rm.VAO);
  glBindBuffer(GL_ARRAY_BUFFER, rm.VBO);
  glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(Vertex),
               m.vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rm.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size() * 4, m.indices.data(),
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, x));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, u));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, nx));
  glEnableVertexAttribArray(3);
  glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex),
                         (void *)offsetof(Vertex, joints));
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, weights));
  glBindVertexArray(0);
  m_Meshes[name] = rm;
  return true;
}

bool GLRenderer::LoadTexture(const std::string &name, const std::string &path) {
  SDL_Surface *s = IMG_Load(path.c_str());
  if (!s) {
    std::cerr << "Failed to load texture image: " << path << std::endl;
    return false;
  }
  SDL_Surface *opt = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
  if (!opt) {
    std::cerr << "Failed to convert surface format for: " << path << std::endl;
    SDL_FreeSurface(s);
    return false;
  }
  GLuint t;
  glGenTextures(1, &t);
  glBindTexture(GL_TEXTURE_2D, t);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, opt->w, opt->h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, opt->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  SDL_FreeSurface(s);
  SDL_FreeSurface(opt);
  m_Textures[name] = t;
  std::cout << "Loaded texture: " << name << " from " << path << " (ID: " << t << ")" << std::endl;
  return true;
}

unsigned int GLRenderer::CreateTextureFromSurface(SDL_Surface *s) {
  if (!s)
    return 0;
  SDL_Surface *opt = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
  if (!opt)
    return 0;
  GLuint t;
  glGenTextures(1, &t);
  glBindTexture(GL_TEXTURE_2D, t);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, opt->w, opt->h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, opt->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  SDL_FreeSurface(opt);
  return t;
}

void GLRenderer::Resize(int w, int h) {
  m_Width = w;
  m_Height = h;
}

void GLRenderer::DrawWireCube(float x, float y, float z, float s, SDL_Color c,
                              float thickness) {
  static unsigned int cubeVAO = 0;
  static unsigned int cubeVBO = 0;
  if (cubeVAO == 0) {
    float v[] = {-0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f,
                 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f,
                 -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
                 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,
                 -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
                 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  -0.5f,
                 0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f};
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
  }

  m_Shader->Use();
  Mat4 model = Mat4::Translate({x, z, -y}) * Mat4::Scale({s, s, s});
  m_Shader->SetMat4("model", model.m);
  m_Shader->SetInt("useSkinning", 0);
  m_Shader->SetInt("useFlatColor", 1);
  m_Shader->SetVec4("flatColor", c.r / 255.0f, c.g / 255.0f, c.b / 255.0f,
                    c.a / 255.0f);
  m_Shader->SetFloat("flashAmount", 0.0f);

  glBindVertexArray(cubeVAO);
  glLineWidth(thickness);
  glDrawArrays(GL_LINES, 0, 24);
  glLineWidth(1.0f);

  m_Shader->SetInt("useFlatColor", 0);
}

void GLRenderer::DrawWireCircle(float x, float y, float z, float radius,
                                SDL_Color c, float thickness) {
  static unsigned int circleVAO = 0;
  static unsigned int circleVBO = 0;
  static int segments = 32;
  if (circleVAO == 0) {
    std::vector<float> v;
    for (int i = 0; i <= segments; i++) {
      float theta = 2.0f * M_PI * float(i) / float(segments);
      v.push_back(cos(theta));
      v.push_back(sin(theta));
      v.push_back(0.0f);
    }
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);
    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), v.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
  }

  m_Shader->Use();
  Mat4 model = Mat4::Translate({x, z, -y}) * Mat4::RotateX(M_PI / 2) *
               Mat4::Scale({radius, radius, 1.0f});
  m_Shader->SetMat4("model", model.m);
  m_Shader->SetInt("useSkinning", 0);
  m_Shader->SetInt("useFlatColor", 1);
  m_Shader->SetVec4("flatColor", c.r / 255.0f, c.g / 255.0f, c.b / 255.0f,
                    c.a / 255.0f);
  m_Shader->SetFloat("flashAmount", 0.0f);

  glBindVertexArray(circleVAO);
  glLineWidth(thickness);
  glDrawArrays(GL_LINE_STRIP, 0, segments + 1);
  glLineWidth(1.0f);

  m_Shader->SetInt("useFlatColor", 0);
}

void GLRenderer::DrawLine(float x1, float y1, float z1, float x2, float y2,
                          float z2, SDL_Color c, float thickness) {
  static unsigned int lineVAO = 0;
  static unsigned int lineVBO = 0;
  if (lineVAO == 0) {
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
  }

  float v[] = {x1, z1, -y1, x2, z2, -y2};

  m_Shader->Use();
  m_Shader->SetMat4("model", Mat4::Identity().m);
  m_Shader->SetInt("useSkinning", 0);
  m_Shader->SetInt("useFlatColor", 1);
  m_Shader->SetVec4("flatColor", c.r / 255.0f, c.g / 255.0f, c.b / 255.0f,
                    c.a / 255.0f);
  m_Shader->SetFloat("flashAmount", 0.0f);

  glBindVertexArray(lineVAO);
  glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glLineWidth(thickness);
  glDrawArrays(GL_LINES, 0, 2);
  glLineWidth(1.0f);

  m_Shader->SetInt("useFlatColor", 0);
}

void GLRenderer::RenderThumbnail(const std::string &meshName,
                                 const std::string &textureName, int x, int y,
                                 int size) {
  if (m_Meshes.find(meshName) == m_Meshes.end()) {
    static int errCount = 0;
    if (errCount < 10) {
      std::cerr << "Thumbnail Error: Mesh not found: " << meshName << std::endl;
      errCount++;
    }
    return;
  }

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  float scale = (float)viewport[2] / (float)m_Width;
  int scaledX = viewport[0] + (int)(x * scale);
  int scaledY = (int)(y * scale);
  int scaledSize = (int)(size * scale);
  int scaledHeight = viewport[3];

  glViewport(scaledX, scaledHeight - (scaledY + scaledSize), scaledSize,
             scaledSize);

  glEnable(GL_SCISSOR_TEST);
  glScissor(scaledX, scaledHeight - (scaledY + scaledSize), scaledSize,
            scaledSize);
  glClear(GL_DEPTH_BUFFER_BIT);
  glDisable(GL_SCISSOR_TEST);

  glEnable(GL_DEPTH_TEST);

  m_Shader->Use();
  m_Shader->SetFloat("alpha", 1.0f);
  m_Shader->SetInt("useFlatColor", 0);

  Vec3 eye = {0.0f, 1.5f, 2.5f};
  Mat4 view = Mat4::LookAt(eye, {0, 0.5f, 0}, {0, 1, 0});
  Mat4 proj = Mat4::Perspective(0.8f, 1.0f, 0.1f, 100.0f);

  m_Shader->SetMat4("view", view.m);
  m_Shader->SetMat4("projection", proj.m);
  m_Shader->SetVec3("lightPos", eye.x, eye.y, eye.z);

  Mat4 model = Mat4::RotateY(0.78f);
  m_Shader->SetMat4("model", model.m);
  m_Shader->SetInt("useSkinning", 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_Textures.count(textureName)
                                   ? m_Textures[textureName]
                                   : m_DefaultTexture);

  RenderMesh &rm = m_Meshes[meshName];
  glBindVertexArray(rm.VAO);
  glDrawElements(GL_TRIANGLES, rm.indexCount, GL_UNSIGNED_INT, 0);

  glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void GLRenderer::RenderMeshPreview(const std::string &meshName,
                                   const std::string &textureName, float x,
                                   float y, float z, float rot, float alpha,
                                   float ox, float oy, float oz) {
  if (m_Meshes.find(meshName) == m_Meshes.end())
    return;

  m_Shader->Use();

  Mat4 model = Mat4::Translate({x + ox, y + oy, z + oz}) * Mat4::RotateY(-rot);
  m_Shader->SetMat4("model", model.m);
  m_Shader->SetInt("useSkinning", 0);
  m_Shader->SetInt("useFlatColor", 0);
  m_Shader->SetFloat("alpha", alpha);

  if (alpha < 0.99f) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE); // Don't write depth for transparent ghost
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_Textures.count(textureName)
                                   ? m_Textures[textureName]
                                   : m_DefaultTexture);

  RenderMesh &rm = m_Meshes[meshName];
  glBindVertexArray(rm.VAO);
  glDrawElements(GL_TRIANGLES, rm.indexCount, GL_UNSIGNED_INT, 0);

  if (alpha < 0.99f) {
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
  }
  m_Shader->SetFloat("alpha", 1.0f);
}

} // namespace PixelsEngine
