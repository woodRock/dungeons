#pragma once
#include "ECS.h"
#include "Camera.h"
#include <SDL2/SDL.h>

namespace PixelsEngine {

class Skybox {
public:
  Skybox();
  ~Skybox();

  // Set skybox color
  void SetColor(float r, float g, float b, float a = 1.0f);
  
  // Get current color
  void GetColor(float& r, float& g, float& b, float& a) const;

  // Apply skybox (sets OpenGL clear color)
  void Apply() const;

private:
  float m_ColorR;
  float m_ColorG;
  float m_ColorB;
  float m_ColorA;
};

} // namespace PixelsEngine
