#pragma once
#include <cmath>
#include <cstring>

namespace PixelsEngine {

struct Vec3 {
  float x, y, z;

  Vec3 operator+(const Vec3 &o) const { return {x + o.x, y + o.y, z + o.z}; }
  Vec3 operator-(const Vec3 &o) const { return {x - o.x, y - o.y, z - o.z}; }
  Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }

  static float Dot(const Vec3 &a, const Vec3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }
  static Vec3 Cross(const Vec3 &a, const Vec3 &b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
  }
  static Vec3 Normalize(const Vec3 &v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len == 0)
      return {0, 0, 0};
    return v * (1.0f / len);
  }
};

struct Mat4 {
  float m[16]; // Column-major

  static Mat4 Identity() {
    Mat4 res;
    std::memset(res.m, 0, 16 * sizeof(float));
    res.m[0] = 1.0f;
    res.m[5] = 1.0f;
    res.m[10] = 1.0f;
    res.m[15] = 1.0f;
    return res;
  }

  static Mat4 Translate(const Vec3 &v) {
    Mat4 res = Identity();
    res.m[12] = v.x;
    res.m[13] = v.y;
    res.m[14] = v.z;
    return res;
  }

  static Mat4 Scale(const Vec3 &v) {
    Mat4 res = Identity();
    res.m[0] = v.x;
    res.m[5] = v.y;
    res.m[10] = v.z;
    return res;
  }

  static Mat4 RotateZ(float angle) {
    Mat4 res = Identity();
    float c = std::cos(angle);
    float s = std::sin(angle);
    res.m[0] = c;
    res.m[1] = s;
    res.m[4] = -s;
    res.m[5] = c;
    return res;
  }

  static Mat4 RotateX(float angle) {
    Mat4 res = Identity();
    float c = std::cos(angle);
    float s = std::sin(angle);
    res.m[5] = c;
    res.m[6] = s;
    res.m[9] = -s;
    res.m[10] = c;
    return res;
  }

  static Mat4 RotateY(float angle) {
    Mat4 res = Identity();
    float c = std::cos(angle);
    float s = std::sin(angle);
    res.m[0] = c;
    res.m[2] = -s;
    res.m[8] = s;
    res.m[10] = c;
    return res;
  }

  static Mat4 Perspective(float fov, float aspect, float znear, float zfar) {
    Mat4 res;
    std::memset(res.m, 0, 16 * sizeof(float));
    float tanHalfFovy = std::tan(fov / 2.0f);
    res.m[0] = 1.0f / (aspect * tanHalfFovy);
    res.m[5] = 1.0f / tanHalfFovy;
    res.m[10] = -(zfar + znear) / (zfar - znear);
    res.m[11] = -1.0f;
    res.m[14] = -(2.0f * zfar * znear) / (zfar - znear);
    return res;
  }

  static Mat4 LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up) {
    Vec3 f = Vec3::Normalize(center - eye);
    Vec3 s = Vec3::Normalize(Vec3::Cross(f, up));
    Vec3 u = Vec3::Cross(s, f);

    Mat4 res = Identity();
    res.m[0] = s.x;
    res.m[4] = s.y;
    res.m[8] = s.z;
    res.m[1] = u.x;
    res.m[5] = u.y;
    res.m[9] = u.z;
    res.m[2] = -f.x;
    res.m[6] = -f.y;
    res.m[10] = -f.z;
    res.m[12] = -Vec3::Dot(s, eye);
    res.m[13] = -Vec3::Dot(u, eye);
    res.m[14] = Vec3::Dot(f, eye);
    return res;
  }

  Mat4 operator*(const Mat4 &rhs) const {
    Mat4 res;
    for (int c = 0; c < 4; c++) {
      for (int r = 0; r < 4; r++) {
        res.m[c * 4 + r] =
            m[0 * 4 + r] * rhs.m[c * 4 + 0] + m[1 * 4 + r] * rhs.m[c * 4 + 1] +
            m[2 * 4 + r] * rhs.m[c * 4 + 2] + m[3 * 4 + r] * rhs.m[c * 4 + 3];
      }
    }
    return res;
  }
};

} // namespace PixelsEngine
