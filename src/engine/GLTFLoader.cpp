#define CGLTF_IMPLEMENTATION
#include "GLTFLoader.h"
#include "Math3D.h"
#include "cgltf.h"
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

namespace PixelsEngine {

// Helper to apply transformation to vertices
void ApplyTransform(Mesh& mesh, size_t start, size_t count, const Mat4& transform) {
    for (size_t i = start; i < start + count; ++i) {
        Vertex& v = mesh.vertices[i];
        
        // Transform position
        Vec3 pos = {v.x, v.y, v.z};
        Vec3 tPos = transform * pos;
        v.x = tPos.x;
        v.y = tPos.y;
        v.z = tPos.z;
        
        // Transform normal (using transpose of inverse for non-uniform scaling, 
        // but here we just use the upper 3x3 for simplicity if scaling is uniform)
        Vec3 norm = {v.nx, v.ny, v.nz};
        // Simple rotation part of transform for normals
        Mat4 rotOnly = transform;
        rotOnly.m[12] = rotOnly.m[13] = rotOnly.m[14] = 0.0f;
        Vec3 tNorm = Vec3::Normalize(rotOnly * norm);
        v.nx = tNorm.x;
        v.ny = tNorm.y;
        v.nz = tNorm.z;
    }
}

void ProcessNode(cgltf_node* node, const Mat4& parentTransform, Mesh& outMesh) {
    Mat4 nodeMat;
    cgltf_node_transform_local(node, nodeMat.m);
    Mat4 worldTransform = parentTransform * nodeMat;

    if (node->mesh) {
        for (cgltf_size pIdx = 0; pIdx < node->mesh->primitives_count; ++pIdx) {
            cgltf_primitive* prim = &node->mesh->primitives[pIdx];
            size_t vertexStart = outMesh.vertices.size();
            
            cgltf_accessor* posAcc = nullptr;
            for (cgltf_size aIdx = 0; aIdx < prim->attributes_count; ++aIdx) {
                if (prim->attributes[aIdx].type == cgltf_attribute_type_position) {
                    posAcc = prim->attributes[aIdx].data;
                    break;
                }
            }
            if (!posAcc) continue;

            size_t count = posAcc->count;
            outMesh.vertices.resize(vertexStart + count);

            for (cgltf_size aIdx = 0; aIdx < prim->attributes_count; ++aIdx) {
                cgltf_attribute* attr = &prim->attributes[aIdx];
                for (cgltf_size k = 0; k < attr->data->count; ++k) {
                    Vertex& v = outMesh.vertices[vertexStart + k];
                    if (attr->type == cgltf_attribute_type_position)
                        cgltf_accessor_read_float(attr->data, k, &v.x, 3);
                    else if (attr->type == cgltf_attribute_type_normal)
                        cgltf_accessor_read_float(attr->data, k, &v.nx, 3);
                    else if (attr->type == cgltf_attribute_type_texcoord)
                        cgltf_accessor_read_float(attr->data, k, &v.u, 2);
                    else if (attr->type == cgltf_attribute_type_joints) {
                        uint32_t j[4];
                        cgltf_accessor_read_uint(attr->data, k, j, 4);
                        v.joints[0] = (int)j[0]; v.joints[1] = (int)j[1];
                        v.joints[2] = (int)j[2]; v.joints[3] = (int)j[3];
                    } else if (attr->type == cgltf_attribute_type_weights)
                        cgltf_accessor_read_float(attr->data, k, v.weights, 4);
                }
            }

            if (prim->indices) {
                for (cgltf_size k = 0; k < prim->indices->count; ++k)
                    outMesh.indices.push_back((unsigned int)cgltf_accessor_read_index(prim->indices, k) + (unsigned int)vertexStart);
            } else {
                for (cgltf_size k = 0; k < count; ++k)
                    outMesh.indices.push_back((unsigned int)(vertexStart + k));
            }
            
            // Apply node transformation to these vertices if not skinned
            if (!outMesh.isSkinned) {
                ApplyTransform(outMesh, vertexStart, count, worldTransform);
            }
        }
    }

    for (cgltf_size i = 0; i < node->children_count; ++i) {
        ProcessNode(node->children[i], worldTransform, outMesh);
    }
}

bool GLTFLoader::Load(const std::string &path, Mesh &outMesh) {
  cgltf_options options = {};
  cgltf_data *data = nullptr;
  cgltf_result result = cgltf_parse_file(&options, path.c_str(), &data);
  if (result != cgltf_result_success)
    return false;

  result = cgltf_load_buffers(&options, data, path.c_str());
  if (result != cgltf_result_success) {
    cgltf_free(data);
    return false;
  }

  std::map<cgltf_node *, int> nodeToBoneIdx;

  if (data->skins_count > 0) {
    cgltf_skin *skin = &data->skins[0];
    outMesh.isSkinned = true;
    outMesh.skeleton.bones.resize(skin->joints_count);
    outMesh.skeleton.jointMatrices.assign(skin->joints_count * 16, 0.0f);
    for (size_t i = 0; i < skin->joints_count; i++) {
      outMesh.skeleton.jointMatrices[i * 16 + 0] = 1.0f;
      outMesh.skeleton.jointMatrices[i * 16 + 5] = 1.0f;
      outMesh.skeleton.jointMatrices[i * 16 + 10] = 1.0f;
      outMesh.skeleton.jointMatrices[i * 16 + 15] = 1.0f;
    }

    std::vector<float> ibms(skin->joints_count * 16);
    if (skin->inverse_bind_matrices) {
      for (size_t i = 0; i < skin->joints_count; i++) {
        cgltf_accessor_read_float(skin->inverse_bind_matrices, i, &ibms[i * 16],
                                  16);
      }
    } else {
      for (size_t i = 0; i < skin->joints_count; i++) {
        Mat4 id;
        memcpy(&ibms[i * 16], id.m, 64);
      }
    }

    for (cgltf_size i = 0; i < skin->joints_count; ++i) {
      cgltf_node *jointNode = skin->joints[i];
      nodeToBoneIdx[jointNode] = (int)i;
      outMesh.skeleton.bones[i].name = jointNode->name ? jointNode->name : "";
      memcpy(outMesh.skeleton.bones[i].inverseBindMatrix, &ibms[i * 16], 64);
      cgltf_node_transform_local(jointNode,
                                 outMesh.skeleton.bones[i].localMatrix);
    }

    for (cgltf_size i = 0; i < skin->joints_count; ++i) {
      cgltf_node *jointNode = skin->joints[i];
      if (jointNode->parent && nodeToBoneIdx.count(jointNode->parent)) {
        outMesh.skeleton.bones[i].parentIndex =
            nodeToBoneIdx[jointNode->parent];
      } else {
        outMesh.skeleton.bones[i].parentIndex = -1;
      }
    }
  }

  // Use hierarchical loading if not skinned (skinned meshes use bone transforms)
  if (!outMesh.isSkinned && data->scenes_count > 0) {
      for (cgltf_size i = 0; i < data->scenes[0].nodes_count; ++i) {
          ProcessNode(data->scenes[0].nodes[i], Mat4::Identity(), outMesh);
      }
  } else {
      // Skinned or legacy path
      for (cgltf_size mIdx = 0; mIdx < data->meshes_count; ++mIdx) {
        cgltf_mesh *mesh = &data->meshes[mIdx];
        for (cgltf_size pIdx = 0; pIdx < mesh->primitives_count; ++pIdx) {
          cgltf_primitive *prim = &mesh->primitives[pIdx];
          size_t vertexStart = outMesh.vertices.size();
          cgltf_accessor *posAcc = nullptr;
          for (cgltf_size aIdx = 0; aIdx < prim->attributes_count; ++aIdx) {
            if (prim->attributes[aIdx].type == cgltf_attribute_type_position) {
              posAcc = prim->attributes[aIdx].data;
              break;
            }
          }
          if (!posAcc)
            continue;
          outMesh.vertices.resize(vertexStart + posAcc->count);

          for (cgltf_size aIdx = 0; aIdx < prim->attributes_count; ++aIdx) {
            cgltf_attribute *attr = &prim->attributes[aIdx];
            for (cgltf_size k = 0; k < attr->data->count; ++k) {
              Vertex &v = outMesh.vertices[vertexStart + k];
              if (attr->type == cgltf_attribute_type_position)
                cgltf_accessor_read_float(attr->data, k, &v.x, 3);
              else if (attr->type == cgltf_attribute_type_normal)
                cgltf_accessor_read_float(attr->data, k, &v.nx, 3);
              else if (attr->type == cgltf_attribute_type_texcoord)
                cgltf_accessor_read_float(attr->data, k, &v.u, 2);
              else if (attr->type == cgltf_attribute_type_joints) {
                uint32_t j[4];
                cgltf_accessor_read_uint(attr->data, k, j, 4);
                v.joints[0] = (int)j[0];
                v.joints[1] = (int)j[1];
                v.joints[2] = (int)j[2];
                v.joints[3] = (int)j[3];
              } else if (attr->type == cgltf_attribute_type_weights)
                cgltf_accessor_read_float(attr->data, k, v.weights, 4);
            }
          }

          if (prim->indices) {
            for (cgltf_size k = 0; k < prim->indices->count; ++k)
              outMesh.indices.push_back(
                  (unsigned int)cgltf_accessor_read_index(prim->indices, k) +
                  (unsigned int)vertexStart);
          } else {
            for (cgltf_size k = 0; k < posAcc->count; ++k)
              outMesh.indices.push_back((unsigned int)(vertexStart + k));
          }
        }
      }
  }

  for (cgltf_size i = 0; i < data->animations_count; ++i) {
    cgltf_animation *anim = &data->animations[i];
    SkeletalAnimation sAnim;
    sAnim.name = anim->name ? anim->name : "anim_" + std::to_string(i);
    sAnim.duration = 0;
    for (cgltf_size j = 0; j < anim->channels_count; ++j) {
      cgltf_animation_channel *chan = &anim->channels[j];
      if (!chan->target_node || !chan->target_node->name)
        continue;
      AnimationChannel sChan;
      sChan.boneName = chan->target_node->name;
      if (chan->target_path == cgltf_animation_path_type_translation)
        sChan.path = AnimationChannel::Translation;
      else if (chan->target_path == cgltf_animation_path_type_rotation)
        sChan.path = AnimationChannel::Rotation;
      else if (chan->target_path == cgltf_animation_path_type_scale)
        sChan.path = AnimationChannel::Scale;
      else
        continue;
      for (cgltf_size k = 0; k < chan->sampler->input->count; ++k) {
        AnimationKeyframe kf;
        cgltf_accessor_read_float(chan->sampler->input, k, &kf.time, 1);
        cgltf_accessor_read_float(chan->sampler->output, k, kf.value, 4);
        sChan.keyframes.push_back(kf);
        if (kf.time > sAnim.duration)
          sAnim.duration = kf.time;
      }
      sAnim.channels.push_back(sChan);
    }
    outMesh.animations.push_back(sAnim);
  }

  cgltf_free(data);
  return true;
}

std::vector<SkeletalAnimation>
GLTFLoader::LoadAnimations(const std::string &path) {
  Mesh tempMesh;
  if (Load(path, tempMesh))
    return tempMesh.animations;
  return {};
}

} // namespace PixelsEngine