#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

namespace PixelsEngine {

class Shader {
public:
  GLuint ID;

  Shader(const char *vertexSource, const char *fragmentSource) {
    // 1. Compile shaders
    GLuint sVertex, sFragment;

    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    CheckCompileErrors(sVertex, "VERTEX");

    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    CheckCompileErrors(sFragment, "FRAGMENT");

    // 2. Link Program
    ID = glCreateProgram();
    glAttachShader(ID, sVertex);
    glAttachShader(ID, sFragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");

    // 3. Delete shaders
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
  }

  void Use() { glUseProgram(ID); }

  void SetMat4(const std::string &name, const float *mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       mat);
  }

  void SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }

private:
  void CheckCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout
            << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
            << infoLog
            << "\n -- --------------------------------------------------- -- "
            << std::endl;
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout
            << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
            << infoLog
            << "\n -- --------------------------------------------------- -- "
            << std::endl;
      }
    }
  }
};

} // namespace PixelsEngine
