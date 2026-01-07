#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#elif defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

namespace PixelsEngine {

class Shader {
public:
  GLuint ID;

  Shader(const char *vertexSource, const char *fragmentSource) {
    GLuint sVertex, sFragment;
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    CheckCompileErrors(sVertex, "VERTEX");

    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    CheckCompileErrors(sFragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, sVertex);
    glAttachShader(ID, sFragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");

    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
  }

  void Use() { glUseProgram(ID); }

  void SetMat4(const std::string &name, const float *mat) const {
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1)
      glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
  }

  void SetInt(const std::string &name, int value) const {
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1)
      glUniform1i(loc, value);
  }

  void SetVec3(const std::string &name, float x, float y, float z) const {
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1)
      glUniform3f(loc, x, y, z);
  }

  void SetFloat(const std::string &name, float value) const {
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1)
      glUniform1f(loc, value);
  }

  void SetVec4(const std::string &name, float x, float y, float z,
               float w) const {
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1)
      glUniform4f(loc, x, y, z, w);
  }

private:
  void CheckCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR: " << type << "\n"
                  << infoLog << std::endl;
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR: " << type << "\n"
                  << infoLog << std::endl;
      }
    }
  }
};

} // namespace PixelsEngine