#pragma once
#include "Graphics.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace PixelsEngine {

class OBJLoader {
public:
  static bool Load(const std::string &path, Mesh &outMesh) {
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
};

} // namespace PixelsEngine
