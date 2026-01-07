#include "Skybox.h"

namespace PixelsEngine {

Skybox::Skybox() : m_ColorR(0.53f), m_ColorG(0.81f), m_ColorB(0.92f), m_ColorA(1.0f) {}

Skybox::~Skybox() {}

void Skybox::SetColor(float r, float g, float b, float a) {
  m_ColorR = r;
  m_ColorG = g;
  m_ColorB = b;
  m_ColorA = a;
}

void Skybox::GetColor(float& r, float& g, float& b, float& a) const {
  r = m_ColorR;
  g = m_ColorG;
  b = m_ColorB;
  a = m_ColorA;
}

void Skybox::Apply() const {
  // Skybox color will be set by the rendering system
  // glClearColor is handled in Game_Render.cpp
}

} // namespace PixelsEngine
