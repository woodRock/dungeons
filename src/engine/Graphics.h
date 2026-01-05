#pragma once
#include <string>
#include <vector>

namespace PixelsEngine {

struct Vertex {
  float x, y, z;
  float u, v;
  float nx, ny, nz;
};

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::string texturePath;
};

} // namespace PixelsEngine
