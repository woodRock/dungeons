#pragma once
#include <string>
#include <vector>

namespace PixelsEngine {

struct Vertex {
  float x, y, z;
  float u, v;
  float nx, ny, nz;
  int joints[4];
  float weights[4];
};

struct Bone {
  std::string name;
  int parentIndex = -1;
  float localMatrix[16];
  float inverseBindMatrix[16];
};

struct Skeleton {
  std::vector<Bone> bones;
  std::vector<float> jointMatrices;
};

struct AnimationKeyframe {
  float time;
  float value[4];
};

struct AnimationChannel {
  std::string boneName;
  enum Path { Translation, Rotation, Scale };
  Path path;
  std::vector<AnimationKeyframe> keyframes;
};

struct SkeletalAnimation {
  std::string name;
  float duration;
  std::vector<AnimationChannel> channels;
};

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  bool isSkinned = false;
  Skeleton skeleton;
  std::vector<SkeletalAnimation> animations;
};

} // namespace PixelsEngine
