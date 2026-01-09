#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#elif defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#else
#define GL_GLEXT_PROTOTYPES
// Try to include SDL_opengl.h in various common locations
#if __has_include(<SDL2/SDL_opengl.h>)
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#else
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#endif
#endif
