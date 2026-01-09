#include "Application.h"
#include "Input.h"
#include "OpenGL.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace PixelsEngine {

Application::Application(const char *title, int width, int height)
    : m_Width(width), m_Height(height) {
  // Initialize Camera
  m_Camera = std::make_unique<Camera>(width, height);

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    return;
  }

  // Initialize PNG and JPG loading
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    std::cerr << "SDL_image could not initialize! SDL_image Error: "
              << IMG_GetError() << std::endl;
    return;
  }

  // Initialize SDL_ttf
  if (TTF_Init() == -1) {
    std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: "
              << TTF_GetError() << std::endl;
    return;
  }

  // Initialize SDL_mixer
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: "
              << Mix_GetError() << std::endl;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  m_Window = SDL_CreateWindow(
      title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
      SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
  if (!m_Window) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    return;
  }

  m_GLContext = SDL_GL_CreateContext(m_Window);
  if (!m_GLContext) {
    std::cerr << "OpenGL context could not be created! SDL_Error: "
              << SDL_GetError() << std::endl;
  }
  SDL_GL_MakeCurrent(m_Window, m_GLContext);
  SDL_GL_SetSwapInterval(1); // VSync

  // Set initial viewport based on drawable size (HighDPI support)
  int dw, dh;
  SDL_GL_GetDrawableSize(m_Window, &dw, &dh);
  glViewport(0, 0, dw, dh);

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // 0 = nearest pixel sampling

  // m_Renderer = SDL_CreateRenderer(
  //    m_Window, -1, SDL_RENDERER_SOFTWARE); // Use Software to avoid GL
  //    context conflict?

  // SDL_RenderSetLogicalSize(m_Renderer, width, height);
  m_Renderer = nullptr;

  m_IsRunning = true;
}

Application::~Application() {
  if (m_Renderer)
    SDL_DestroyRenderer(m_Renderer);
  if (m_Window)
    SDL_DestroyWindow(m_Window);
  Mix_CloseAudio();
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void Application::ToggleFullScreen() {
  Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
  bool isFullscreen = SDL_GetWindowFlags(m_Window) & fullscreenFlag;
  SDL_SetWindowFullscreen(m_Window, isFullscreen ? 0 : fullscreenFlag);
  SDL_RenderSetLogicalSize(m_Renderer, m_Width, m_Height);
}

void Application::Run() {
  OnStart();
  m_LastTime = SDL_GetTicks();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(
      [](void *arg) { static_cast<Application *>(arg)->Step(); }, this, 0, 1);
#else
  while (m_IsRunning) {
    Step();
  }
#endif
}

void Application::Step() {
  Input::SetRenderer(m_Renderer);

  int ww, wh;
  SDL_GetWindowSize(m_Window, &ww, &wh);
  Input::SetLogicalScale((float)m_Width / ww, (float)m_Height / wh);

  Input::BeginFrame();

  Uint32 currentTime = SDL_GetTicks();
  float deltaTime = (currentTime - m_LastTime) / 1000.0f;
  m_LastTime = currentTime;

  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    Input::ProcessEvent(e);

    if (e.type == SDL_QUIT) {
      m_IsRunning = false;
#ifdef __EMSCRIPTEN__
      emscripten_cancel_main_loop();
#endif
    } else if (e.type == SDL_WINDOWEVENT) {
      if (e.window.event == SDL_WINDOWEVENT_RESIZED ||
          e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        SDL_RenderSetLogicalSize(m_Renderer, m_Width, m_Height);
      }
    }
  }

  OnUpdate(deltaTime);

  // SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
  // SDL_RenderClear(m_Renderer);

  OnRender();

  // SDL_RenderPresent(m_Renderer);
}

} // namespace PixelsEngine
