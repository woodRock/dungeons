#pragma once
#include "Camera.h"
#include "ECS.h"
#include "Graphics.h"
#include "Math3D.h"
#include "Shader.h"
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace PixelsEngine {

struct AABB {
  float minX, minY, minZ;
  float maxX, maxY, maxZ;
  
  AABB() : minX(0), minY(0), minZ(0), maxX(0), maxY(0), maxZ(0) {}
  AABB(float minX_, float minY_, float minZ_, float maxX_, float maxY_, float maxZ_)
    : minX(minX_), minY(minY_), minZ(minZ_), maxX(maxX_), maxY(maxY_), maxZ(maxZ_) {}
  
  // Check if two AABBs overlap
  bool Overlaps(const AABB& other) const {
    return !(maxX < other.minX || minX > other.maxX ||
             maxY < other.minY || minY > other.maxY ||
             maxZ < other.minZ || minZ > other.maxZ);
  }
  
  // Check if point is inside AABB
  bool Contains(float x, float y, float z) const {
    return x >= minX && x <= maxX && y >= minY && y <= maxY && z >= minZ && z <= maxZ;
  }
  
  // Get transformed AABB (translated by position)
  AABB Translate(float x, float y, float z) const {
    return AABB(minX + x, minY + y, minZ + z, maxX + x, maxY + y, maxZ + z);
  }
};

struct RenderMesh {
  unsigned int VAO, VBO, EBO;
  unsigned int indexCount;
  bool isSkinned = false;
  Skeleton skeleton;
  std::vector<SkeletalAnimation> animations;
  std::vector<float>
      boneGlobalMatrices; // Stores global transforms for attachments
  AABB boundingBox;      // Collision bounding box
};

class GLRenderer {
public:
  GLRenderer();
  ~GLRenderer();

  void Init(int width, int height);
  void SetClearColor(float r, float g, float b, float a = 1.0f);
  void Render(SDL_Window *window, const Camera &cam, Registry &registry,
              bool swap = true);

  void UpdateSkinnedMesh(RenderMesh &rm, int animIndex, float time);
  RenderMesh *GetRenderMesh(const std::string &name);

  // UI Rendering
  void InitUI();
  void DrawRect2D(float x, float y, float w, float h, SDL_Color color);
  void DrawRotatedRect2D(float x, float y, float w, float h, float angle, SDL_Color color);
  void DrawTexture2D(unsigned int textureID, float x, float y, float w, float h,
                     SDL_Color color = {255, 255, 255, 255});
  void DrawWireCube(float x, float y, float z, float size, SDL_Color color,
                    float thickness = 1.0f);
  void DrawWireCircle(float x, float y, float z, float radius, SDL_Color color,
                      float thickness = 1.0f);
  void DrawLine(float x1, float y1, float z1, float x2, float y2, float z2,
                SDL_Color color, float thickness = 1.0f);
  void RenderThumbnail(const std::string &meshName,
                       const std::string &textureName, int x, int y, int size);
  void RenderMeshPreview(const std::string &meshName,
                         const std::string &textureName, float x, float y,
                         float z, float rot, float alpha, float offsetX = 0,
                         float offsetY = 0, float offsetZ = 0);

  // Load mesh into GPU memory, returns identifier string
  bool LoadMesh(const std::string &name, const std::string &path);

  // Load texture into GPU memory
  bool LoadTexture(const std::string &name, const std::string &path);
  unsigned int CreateTextureFromSurface(SDL_Surface *surf);

  void Resize(int width, int height);

private:
  int m_Width;
  int m_Height;
  std::unique_ptr<Shader> m_Shader;

  std::unordered_map<std::string, RenderMesh> m_Meshes;
  std::unordered_map<std::string, unsigned int> m_Textures;

  unsigned int m_DefaultTexture;
  
  // Clear color
  float m_ClearColorR = 0.53f;
  float m_ClearColorG = 0.81f;
  float m_ClearColorB = 0.92f;
  float m_ClearColorA = 1.0f;

  // UI Resources
  std::unique_ptr<Shader> m_UIShader;
  unsigned int m_QuadVAO = 0;
  unsigned int m_QuadVBO = 0;

  std::unique_ptr<Shader> m_LineShader;

  unsigned int m_LineVAO = 0;

  unsigned int m_LineVBO = 0;

  Mat4 m_LastView;

  Mat4 m_LastProjection;
};

} // namespace PixelsEngine