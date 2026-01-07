#pragma once
#include "Graphics.h"
#include <string>
#include <vector>

namespace PixelsEngine {

class GLTFLoader {
public:
  static bool Load(const std::string &path, Mesh &outMesh);
  static std::vector<SkeletalAnimation> LoadAnimations(const std::string &path);
};

} // namespace PixelsEngine
