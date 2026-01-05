#include "GLRenderer.h"
#include "Components.h"
#include "OBJLoader.h"
#include <SDL2/SDL_image.h>
#include <iostream>

namespace PixelsEngine {

GLRenderer::GLRenderer() : m_Width(800), m_Height(600), m_DefaultTexture(0) {}

GLRenderer::~GLRenderer() {
  // Cleanup GL resources...
}

void GLRenderer::Init(int width, int height) {
  std::cout << "GLRenderer::Init calling..." << std::endl;
  m_Width = width;
  m_Height = height;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Fix for textures with odd widths (like some fonts)
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Simple Shaders
  const char *vShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;
        layout (location = 2) in vec3 aNormal;

        out vec2 TexCoord;
        out vec3 Normal;
        out vec3 FragPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            TexCoord = aTexCoord;
            Normal = mat3(transpose(inverse(model))) * aNormal;
            FragPos = vec3(model * vec4(aPos, 1.0));
        }
    )";

  const char *fShader = R"(
        #version 330 core
        out vec4 FragColor;

        in vec2 TexCoord;
        in vec3 Normal;
        in vec3 FragPos;

        uniform sampler2D ourTexture;
        uniform vec3 lightPos;
        uniform vec3 viewPos;
        uniform float alpha;
        uniform bool useAlpha;

        void main()
        {
            // Ambient
            float ambientStrength = 0.3;
            vec3 lightColor = vec3(1.0, 1.0, 1.0);
            vec3 ambient = ambientStrength * lightColor;
            
            // Diffuse
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            
            vec4 texColor = texture(ourTexture, TexCoord);
            vec3 result = (ambient + diffuse) * texColor.rgb;
            
            float outAlpha = texColor.a;
            if (useAlpha) outAlpha *= alpha;
            
            FragColor = vec4(result, outAlpha);
            
            if(FragColor.a < 0.1) discard;
        }
    )";

  m_Shader = std::make_unique<Shader>(vShader, fShader);

  // Create 1x1 white default texture
  glGenTextures(1, &m_DefaultTexture);
  glBindTexture(GL_TEXTURE_2D, m_DefaultTexture);
  unsigned char white[] = {255, 255, 255, 255};
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               white);

  InitUI();
}

void GLRenderer::InitUI() {
  // 2D Shader
  const char *vShaderUI = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;
        
        out vec2 TexCoord;
        
        uniform mat4 projection;
        uniform mat4 model;
        
        void main() {
            gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";

  const char *fShaderUI = R"(
        #version 330 core
        out vec4 FragColor;
        in vec2 TexCoord;
        
        uniform sampler2D uiTexture;
        uniform vec4 color;
        uniform bool useTexture;
        
        void main() {
            if (useTexture) {
                vec4 texColor = texture(uiTexture, TexCoord);
                FragColor = texColor * color;
            } else {
                FragColor = color;
            }
        }
    )";

  m_UIShader = std::make_unique<Shader>(vShaderUI, fShaderUI);

  // 1x1 Quad
  float quadVertices[] = {
      // pos      // tex
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

  glGenVertexArrays(1, &m_QuadVAO);
  glGenBuffers(1, &m_QuadVBO);
  glBindVertexArray(m_QuadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    // Line Shader
    const char* vLine = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 view;
        uniform mat4 projection;
        uniform mat4 model;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    const char* fLine = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec4 color;
        void main() {
            FragColor = color;
        }
    )";
    m_LineShader = std::make_unique<Shader>(vLine, fLine);

    float cubeLines[] = {
        0,0,0, 1,0,0,  1,0,0, 1,1,0,  1,1,0, 0,1,0,  0,1,0, 0,0,0,
        0,0,1, 1,0,1,  1,0,1, 1,1,1,  1,1,1, 0,1,1,  0,1,1, 0,0,1,
        0,0,0, 0,0,1,  1,0,0, 1,0,1,  1,1,0, 1,1,1,  0,1,0, 0,1,1
    };
    glGenVertexArrays(1, &m_LineVAO);
    glGenBuffers(1, &m_LineVBO);
    glBindVertexArray(m_LineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_LineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeLines), cubeLines, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void GLRenderer::DrawWireCube(float x, float y, float z, float size, SDL_Color color) {
    m_LineShader->Use();
    m_LineShader->SetMat4("view", m_LastView.m);
    m_LineShader->SetMat4("projection", m_LastProjection.m);
    
    // Grid alignment: size is usually 4.0. We want to draw from -size/2 to +size/2 relative to center.
    // My line cube is 0 to 1.
    Mat4 model = Mat4::Translate({x - size*0.5f, z - size*0.5f, -y - size*0.5f});
    model = model * Mat4::Scale({size, size, size});
    m_LineShader->SetMat4("model", model.m);
    
    float c[4] = {color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f};
    glUniform4fv(glGetUniformLocation(m_LineShader->ID, "color"), 1, c);
    
    glBindVertexArray(m_LineVAO);
    glDrawArrays(GL_LINES, 0, 24);
    glBindVertexArray(0);
}



void GLRenderer::DrawRect2D(int x, int y, int w, int h, SDL_Color color) {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_UIShader->Use();

  // Ortho Projection (Top-Left Origin) using logic window size
  // But we need to account for HighDPI in the projection or the viewport.
  // If glViewport is (0,0,dw,dh) and we want to draw in (0,0,1280,720)
  // We should use the logical size for Ortho.
  float L = 0, R = (float)m_Width, B = (float)m_Height, T = 0;
  float ortho[16] = {2.0f / (R - L),
                     0,
                     0,
                     0,
                     0,
                     2.0f / (T - B),
                     0,
                     0,
                     0,
                     0,
                     -1,
                     0,
                     -(R + L) / (R - L),
                     -(T + B) / (T - B),
                     0,
                     1};
  m_UIShader->SetMat4("projection", ortho);

  // Model Matrix
  Mat4 model = Mat4::Translate({(float)x, (float)y, 0});
  model = model * Mat4::Scale({(float)w, (float)h, 1.0f});
  m_UIShader->SetMat4("model", model.m);

  float c[4] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
                color.a / 255.0f};
  glUniform4fv(glGetUniformLocation(m_UIShader->ID, "color"), 1, c);
  m_UIShader->SetInt("useTexture", 0);

  glBindVertexArray(m_QuadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

void GLRenderer::DrawTexture2D(unsigned int textureID, int x, int y, int w,
                               int h, SDL_Color color) {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_UIShader->Use();

  float L = 0, R = (float)m_Width, B = (float)m_Height, T = 0;
  float ortho[16] = {2.0f / (R - L),
                     0,
                     0,
                     0,
                     0,
                     2.0f / (T - B),
                     0,
                     0,
                     0,
                     0,
                     -1,
                     0,
                     -(R + L) / (R - L),
                     -(T + B) / (T - B),
                     0,
                     1};
  m_UIShader->SetMat4("projection", ortho);

  Mat4 model = Mat4::Translate({(float)x, (float)y, 0});
  model = model * Mat4::Scale({(float)w, (float)h, 1.0f});
  m_UIShader->SetMat4("model", model.m);

  float c[4] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
                color.a / 255.0f};
  glUniform4fv(glGetUniformLocation(m_UIShader->ID, "color"), 1, c);

  m_UIShader->SetInt("useTexture", 1);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
  m_UIShader->SetInt("uiTexture", 0);

  glBindVertexArray(m_QuadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

void GLRenderer::Resize(int width, int height) {
  m_Width = width;
  m_Height = height;
  glViewport(0, 0, width, height);
}

bool GLRenderer::LoadMesh(const std::string &name, const std::string &path) {
  if (m_Meshes.find(name) != m_Meshes.end())
    return true;

  Mesh mesh;
  if (!OBJLoader::Load(path, mesh))
    return false;

  RenderMesh rm;
  rm.indexCount = (unsigned int)mesh.indices.size();

  glGenVertexArrays(1, &rm.VAO);
  glGenBuffers(1, &rm.VBO);
  glGenBuffers(1, &rm.EBO);

  glBindVertexArray(rm.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, rm.VBO);
  glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex),
               mesh.vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rm.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(),
               GL_STATIC_DRAW);

  // Vertex Positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  // Texture Coords
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, u));
  // Normals
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, nx));

  glBindVertexArray(0);

  m_Meshes[name] = rm;
  return true;
}

bool GLRenderer::LoadTexture(const std::string &name, const std::string &path) {
  SDL_Surface *surf = IMG_Load(path.c_str());
  if (!surf) {
    std::cerr << "Failed to load texture: " << path
              << " Error: " << IMG_GetError() << std::endl;
    return false;
  }

  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);

  int mode = GL_RGB;
  if (surf->format->BytesPerPixel == 4)
    mode = GL_RGBA;

  glTexImage2D(GL_TEXTURE_2D, 0, mode, surf->w, surf->h, 0, mode,
               GL_UNSIGNED_BYTE, surf->pixels);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  SDL_FreeSurface(surf);
  m_Textures[name] = texID;
  return true;
}

unsigned int GLRenderer::CreateTextureFromSurface(SDL_Surface *surf) {
  if (!surf)
    return 0;

  // Ensure surface is in a format GL likes (RGBA 32-bit in memory)
  // On Little Endian (Mac/PC), SDL_PIXELFORMAT_ABGR8888 maps to [R, G, B, A]
  // bytes.
  SDL_Surface *optimized =
      SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_ABGR8888, 0);
  if (!optimized)
    return 0;

  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, optimized->w, optimized->h, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, optimized->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  SDL_FreeSurface(optimized);
  return texID;
}

void GLRenderer::RenderThumbnail(const std::string &meshName,
                                 const std::string &textureName, int x, int y,
                                 int size) {
  if (m_Meshes.find(meshName) == m_Meshes.end())
    return;

  // Save current viewport
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  // Account for HighDPI scale
  int dw, dh;
  SDL_GL_GetDrawableSize(SDL_GL_GetCurrentWindow(), &dw, &dh);
  float scaleX = (float)dw / m_Width;
  float scaleY = (float)dh / m_Height;

  // GL viewport is bottom-up
  glViewport((int)(x * scaleX), (int)((m_Height - y - size) * scaleY),
             (int)(size * scaleX), (int)(size * scaleY));

  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT);

  m_Shader->Use();

  // Fixed Thumbnail Camera
  Vec3 eye = {4, 4, 4};
  Vec3 center = {0, 0, 0};
  Mat4 view = Mat4::LookAt(eye, center, {0, 1, 0});
  Mat4 proj = Mat4::Perspective(0.8f, 1.0f, 0.1f, 20.0f);

  m_Shader->SetMat4("view", view.m);
  m_Shader->SetMat4("projection", proj.m);

  Mat4 model = Mat4::Identity();
  model = model * Mat4::RotateY(0.7f);

  m_Shader->SetMat4("model", model.m);
  m_Shader->SetInt("ourTexture", 0);

  float lightPos[] = {eye.x, eye.y, eye.z};
  glUniform3fv(glGetUniformLocation(m_Shader->ID, "lightPos"), 1, lightPos);

  glActiveTexture(GL_TEXTURE0);
  if (!textureName.empty() &&
      m_Textures.find(textureName) != m_Textures.end()) {
    glBindTexture(GL_TEXTURE_2D, m_Textures[textureName]);
  } else {
    glBindTexture(GL_TEXTURE_2D, m_DefaultTexture);
  }

  RenderMesh &rm = m_Meshes[meshName];
  glBindVertexArray(rm.VAO);
  glDrawElements(GL_TRIANGLES, rm.indexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // Restore viewport
  glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void GLRenderer::RenderMeshPreview(const std::string& meshName, const std::string& textureName, float x, float y, float z, float rot, float alpha, float offsetX, float offsetY, float offsetZ) {
    if (m_Meshes.find(meshName) == m_Meshes.end()) return;

    m_Shader->Use();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUniform1i(glGetUniformLocation(m_Shader->ID, "useAlpha"), 1);
    glUniform1f(glGetUniformLocation(m_Shader->ID, "alpha"), alpha);

    Mat4 model = Mat4::Identity();
    model = model * Mat4::Translate({x, z, -y}); // Map Engine to GL
    model = model * Mat4::RotateY(-rot);
    model = model * Mat4::Translate({offsetX, offsetZ, -offsetY});
    
    m_Shader->SetMat4("model", model.m);

  glActiveTexture(GL_TEXTURE0);
  if (!textureName.empty() &&
      m_Textures.find(textureName) != m_Textures.end()) {
    glBindTexture(GL_TEXTURE_2D, m_Textures[textureName]);
  } else {
    glBindTexture(GL_TEXTURE_2D, m_DefaultTexture);
  }

  RenderMesh &rm = m_Meshes[meshName];
  glBindVertexArray(rm.VAO);
  glDrawElements(GL_TRIANGLES, rm.indexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glUniform1i(glGetUniformLocation(m_Shader->ID, "useAlpha"), 0);
  glDisable(GL_BLEND);
}

void GLRenderer::Render(SDL_Window *window, const Camera &cam,
                        Registry &registry, bool swap) {
  int dw, dh;
  SDL_GL_GetDrawableSize(window, &dw, &dh);
  glViewport(0, 0, dw, dh);

  glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky Blue
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_Shader->Use();

  // View Matrix (LookAt)
  // Cam: x,y,z is position. yaw/pitch are angles.
  // Convert yaw/pitch to direction vector
  Vec3 eye = {
      cam.x, cam.z,
      cam.y}; // Note: Original engine had z as UP (Wolf3D style: x,y is floor,
              // z is height). But wait, the Raycaster code said: floorX =
              // cam.x, floorY = cam.y. "z" was height (e.g. 0.5). OpenGL
              // usually Y is up. Let's adopt Y-up for OpenGL to match standard.
              // Current engine: X,Y is ground plane. Z is height.
              // So Eye = (cam.x, cam.z, cam.y) if we map: Engine Z -> GL Y.
              // Engine Y -> GL Z.

  // Engine Coordinates:
  // X = Right/Left
  // Y = Forward/Backward (North/South)
  // Z = Up/Down

  // Standard GL:
  // X = Right
  // Y = Up
  // Z = Backward (Out of screen)

  // Mapping:
  // Engine X -> GL X
  // Engine Z -> GL Y
  // Engine Y -> GL -Z (Forward is -Z)

  Vec3 glEye = {cam.x, cam.z, -cam.y};

  float yawRad = cam.yaw; // Engine yaw: 0 is East (X+). Pi/2 is North (Y+).
  // In GL: 0 should be -Z?
  // Let's just calculate direction vector in Engine space and map it.

  float dirX = cos(cam.yaw) * cos(cam.pitch);
  float dirY = sin(cam.yaw) * cos(cam.pitch); // Engine Y
  float dirZ = sin(cam.pitch);                // Engine Z

      Vec3 glTarget = {glEye.x + dirX, glEye.y + dirZ, glEye.z - dirY}; // Note -dirY because Engine Y is GL -Z

      

      Mat4 view = Mat4::LookAt(glEye, glTarget, {0, 1, 0});

      Mat4 proj = Mat4::Perspective(1.04f, (float)m_Width / (float)m_Height, 0.1f, 100.0f); // ~60 deg

  

      m_LastView = view;

      m_LastProjection = proj;

  

      m_Shader->SetMat4("view", view.m);

  
  m_Shader->SetMat4("projection", proj.m);
  m_Shader->SetInt("ourTexture", 0);

  glUniform1i(glGetUniformLocation(m_Shader->ID, "useAlpha"), 0);
  glUniform1f(glGetUniformLocation(m_Shader->ID, "alpha"), 1.0f);

  // Light at camera position
  float lightPos[] = {glEye.x, glEye.y, glEye.z};
  glUniform3fv(glGetUniformLocation(m_Shader->ID, "lightPos"), 1, lightPos);

  auto &viewGroup = registry.View<MeshComponent>();
  for (auto &pair : viewGroup) {
    Entity e = pair.first;
    auto &meshComp = pair.second;
    auto *trans = registry.GetComponent<Transform3DComponent>(e);

    if (m_Meshes.find(meshComp.meshName) != m_Meshes.end()) {
      RenderMesh &rm = m_Meshes[meshComp.meshName];

      Mat4 model = Mat4::Identity();

      // Translate
      float tx = 0, ty = 0, tz = 0;
      if (trans) {
        tx = trans->x;
        ty = trans->z;
        tz = -trans->y;
      } // Map Engine Coords to GL
      model = model * Mat4::Translate({tx, ty, tz});

                  // Rotate (Yaw)

                  if (trans) {

                      model = model * Mat4::RotateY(-trans->rot);

                  }

                  

                  // Apply Local Offsets (Pivot)

                  model = model * Mat4::Translate({meshComp.offsetX, meshComp.offsetZ, -meshComp.offsetY});

      

                  // Scale

                  model = model * Mat4::Scale({meshComp.scaleX, meshComp.scaleZ, meshComp.scaleY}); 

      

      m_Shader->SetMat4("model", model.m);

      // Bind Texture
      glActiveTexture(GL_TEXTURE0);
      if (!meshComp.textureName.empty() &&
          m_Textures.find(meshComp.textureName) != m_Textures.end()) {
        glBindTexture(GL_TEXTURE_2D, m_Textures[meshComp.textureName]);
      } else {
        glBindTexture(GL_TEXTURE_2D, m_DefaultTexture);
      }

      glBindVertexArray(rm.VAO);
      glDrawElements(GL_TRIANGLES, rm.indexCount, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    }
  }

  if (swap)
    SDL_GL_SwapWindow(window);
}

} // namespace PixelsEngine
