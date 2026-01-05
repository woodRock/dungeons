#include "Input.h"
#include <cstring>

namespace PixelsEngine {

Uint8 Input::m_KeyboardState[SDL_NUM_SCANCODES] = {0};
Uint8 Input::m_PrevKeyboardState[SDL_NUM_SCANCODES] = {0};
Uint32 Input::m_MouseState = 0;
Uint32 Input::m_PrevMouseState = 0;
int Input::m_MouseX = 0;
int Input::m_MouseY = 0;
int Input::m_MouseRelX = 0;
int Input::m_MouseRelY = 0;
SDL_Renderer *Input::m_Renderer = nullptr;
float Input::m_ScaleX = 1.0f;
float Input::m_ScaleY = 1.0f;
std::string Input::m_TextInput = "";
bool Input::m_TextInputActive = false;

void Input::SetRenderer(SDL_Renderer *renderer) { m_Renderer = renderer; }

void Input::BeginFrame() {
  memcpy(m_PrevKeyboardState, m_KeyboardState, SDL_NUM_SCANCODES);
  m_PrevMouseState = m_MouseState;

  m_MouseRelX = 0;
  m_MouseRelY = 0;

  // Sync mouse position (handles startup/no-event cases)
  int rawX, rawY;
  SDL_GetMouseState(&rawX, &rawY);
  if (m_Renderer) {
    float logX, logY;
    SDL_RenderWindowToLogical(m_Renderer, rawX, rawY, &logX, &logY);
    m_MouseX = (int)logX;
    m_MouseY = (int)logY;
  } else {
    m_MouseX = (int)(rawX * m_ScaleX);
    m_MouseY = (int)(rawY * m_ScaleY);
  }
}

void Input::ProcessEvent(const SDL_Event &e) {
  if (e.type == SDL_KEYDOWN) {
    m_KeyboardState[e.key.keysym.scancode] = 1;
  } else if (e.type == SDL_KEYUP) {
    m_KeyboardState[e.key.keysym.scancode] = 0;
  } else if (e.type == SDL_MOUSEBUTTONDOWN) {
    m_MouseState |= SDL_BUTTON(e.button.button);
    m_MouseX = e.button.x;
    m_MouseY = e.button.y;
  } else if (e.type == SDL_MOUSEBUTTONUP) {
    m_MouseState &= ~SDL_BUTTON(e.button.button);
    m_MouseX = e.button.x;
    m_MouseY = e.button.y;
  } else if (e.type == SDL_MOUSEMOTION) {
    m_MouseX = e.motion.x;
    m_MouseY = e.motion.y;
    m_MouseRelX += e.motion.xrel;
    m_MouseRelY += e.motion.yrel;
  } else if (e.type == SDL_TEXTINPUT) {
    m_TextInput += e.text.text;
  }
}

bool Input::IsKeyDown(SDL_Scancode key) { return m_KeyboardState[key]; }

bool Input::IsKeyPressed(SDL_Scancode key) {
  return m_KeyboardState[key] && !m_PrevKeyboardState[key];
}

bool Input::IsKeyReleased(SDL_Scancode key) {
  return !m_KeyboardState[key] && m_PrevKeyboardState[key];
}

bool Input::IsMouseButtonDown(int button) {
  return (m_MouseState & SDL_BUTTON(button));
}

bool Input::IsMouseButtonPressed(int button) {
  return (m_MouseState & SDL_BUTTON(button)) &&
         !(m_PrevMouseState & SDL_BUTTON(button));
}

void Input::GetMousePosition(int &x, int &y) {
  x = m_MouseX;
  y = m_MouseY;
}

void Input::GetMouseDelta(int &dx, int &dy) {

  dx = m_MouseRelX;

  dy = m_MouseRelY;

}



void Input::ClearKeys() {

    memset(m_KeyboardState, 0, SDL_NUM_SCANCODES);

    memset(m_PrevKeyboardState, 0, SDL_NUM_SCANCODES);

}



} // namespace PixelsEngine
