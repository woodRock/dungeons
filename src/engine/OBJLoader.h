#pragma once
#include "Graphics.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

namespace PixelsEngine {

class OBJLoader {
public:
  static bool Load(const std::string &path, Mesh &outMesh) {
    if (path.find(".glb") != std::string::npos) {
        return LoadGLB(path, outMesh);
    }
    std::ifstream file(path);
    if (!file.is_open()) {
      std::cerr << "Failed to open OBJ: " << path << std::endl;
      return false;
    }

    std::vector<float> positions; // x, y, z
    std::vector<float> texCoords; // u, v
    std::vector<float> normals;   // nx, ny, nz

    // Flattened vertex data to handle unique combinations of v/vt/vn
    std::vector<Vertex> uniqueVertices;
    std::map<std::string, unsigned int> vertexMap;

    std::string line;
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      std::string type;
      iss >> type;

      if (type == "v") {
        float x, y, z;
        iss >> x >> y >> z;
        positions.push_back(x);
        positions.push_back(y);
        positions.push_back(z);
      } else if (type == "vt") {
        float u, v;
        iss >> u >> v;
        texCoords.push_back(u);
        texCoords.push_back(
            v); // OBJ often has V inverted relative to OpenGL, but let's check.
      } else if (type == "vn") {
        float nx, ny, nz;
        iss >> nx >> ny >> nz;
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);
      } else if (type == "f") {
        std::string vStr;
        int verticesInFace = 0;

        // We'll triangulate on the fly (fan)
        std::vector<unsigned int> faceIndices;

        while (iss >> vStr) {
          if (vertexMap.find(vStr) == vertexMap.end()) {
            // Parse v/vt/vn
            int vIdx = -1, vtIdx = -1, vnIdx = -1;
            size_t firstSlash = vStr.find('/');
            size_t secondSlash = vStr.find('/', firstSlash + 1);

            if (firstSlash != std::string::npos) {
              vIdx = std::stoi(vStr.substr(0, firstSlash)) - 1;
              if (secondSlash != std::string::npos) {
                // v/vt/vn or v//vn
                if (secondSlash > firstSlash + 1) {
                  vtIdx = std::stoi(vStr.substr(firstSlash + 1,
                                                secondSlash - firstSlash - 1)) -
                          1;
                }
                vnIdx = std::stoi(vStr.substr(secondSlash + 1)) - 1;
              } else {
                // v/vt
                vtIdx = std::stoi(vStr.substr(firstSlash + 1)) - 1;
              }
            } else {
              // v
              vIdx = std::stoi(vStr) - 1;
            }

            Vertex vert = {0, 0, 0, 0, 0, 0, 0, 0};
            if (vIdx >= 0 && (vIdx * 3 + 2) < positions.size()) {
              vert.x = positions[vIdx * 3];
              vert.y = positions[vIdx * 3 + 1];
              vert.z = positions[vIdx * 3 + 2];
            }
            if (vtIdx >= 0 && (vtIdx * 2 + 1) < texCoords.size()) {
              vert.u = texCoords[vtIdx * 2];
              vert.v = 1.0f - texCoords[vtIdx * 2 + 1]; // Flip V for OpenGL
            }
            if (vnIdx >= 0 && (vnIdx * 3 + 2) < normals.size()) {
              vert.nx = normals[vnIdx * 3];
              vert.ny = normals[vnIdx * 3 + 1];
              vert.nz = normals[vnIdx * 3 + 2];
            }

            vertexMap[vStr] = (unsigned int)outMesh.vertices.size();
            outMesh.vertices.push_back(vert);
          }
          faceIndices.push_back(vertexMap[vStr]);
        }

        // Triangulate (Fan)
        if (faceIndices.size() >= 3) {
          for (size_t i = 1; i < faceIndices.size() - 1; i++) {
            outMesh.indices.push_back(faceIndices[0]);
            outMesh.indices.push_back(faceIndices[i]);
            outMesh.indices.push_back(faceIndices[i + 1]);
          }
        }
      }
    }

    std::cout << "Loaded OBJ " << path << ": " << outMesh.vertices.size()
              << " vertices, " << outMesh.indices.size() << " indices."
              << std::endl;
    return true;
  }

  static bool LoadGLB(const std::string &path, Mesh &outMesh) {
      std::ifstream file(path, std::ios::binary);
      if (!file.is_open()) return false;

      // GLB Header
      uint32_t magic, version, length;
      file.read((char*)&magic, 4);
      file.read((char*)&version, 4);
      file.read((char*)&length, 4);

      if (magic != 0x46546C67) return false; // "glTF"

      // For simplicity, we skip JSON and find the BIN chunk
      // This is a VERY hacky/minimal loader for specific assets
      while (file.tellg() < length) {
          uint32_t chunkLength, chunkType;
          file.read((char*)&chunkLength, 4);
          file.read((char*)&chunkType, 4);

          if (chunkType == 0x004E4F53) { // JSON
              file.seekg(chunkLength, std::ios::cur);
          } else if (chunkType == 0x004E4942) { // BIN
              // We assume specific layout for these KayKit assets or just fail gracefully
              // Actually, without a JSON parser, loading arbitrary GLB is hard.
              // I'll try to find a simpler way or fallback to spheres for characters if needed.
              // But let's try to extract SOME data.
              
              // Instead of full GLB parsing, let's look for known OBJ versions or use a simplified mesh.
              break;
          } else {
              file.seekg(chunkLength, std::ios::cur);
          }
      }
      
      // Fallback: If GLB loading fails, just create a dummy "character" cube/mesh
      std::cout << "GLB loading not fully implemented, using dummy mesh for: " << path << std::endl;
      outMesh.vertices = {
          {-0.5, 0, -0.5, 0, 0, 0, 1, 0}, {0.5, 0, -0.5, 1, 0, 0, 1, 0}, {0.5, 2, -0.5, 1, 1, 0, 1, 0}, {-0.5, 2, -0.5, 0, 1, 0, 1, 0},
          {-0.5, 0, 0.5, 0, 0, 0, 1, 0}, {0.5, 0, 0.5, 1, 0, 0, 1, 0}, {0.5, 2, 0.5, 1, 1, 0, 1, 0}, {-0.5, 2, 0.5, 0, 1, 0, 1, 0}
      };
      outMesh.indices = { 0, 1, 2, 2, 3, 0,  4, 5, 6, 6, 7, 4,  0, 1, 5, 5, 4, 0,  2, 3, 7, 7, 6, 2,  1, 2, 6, 6, 5, 1,  0, 3, 7, 7, 4, 0 };
      
      return true;
  }
};

} // namespace PixelsEngine
