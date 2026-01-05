#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

namespace PixelsEngine {

class Input {
public:
  static void BeginFrame();
  static void ProcessEvent(const SDL_Event &e);
  static void SetRenderer(SDL_Renderer *renderer);
  static void SetLogicalScale(float x, float y) {
    m_ScaleX = x;
    m_ScaleY = y;
  }

  static bool IsKeyDown(SDL_Scancode key);
  static bool IsKeyPressed(SDL_Scancode key);
  static bool IsKeyReleased(SDL_Scancode key);

  static bool IsMouseButtonDown(int button);
  static bool IsMouseButtonPressed(int button);
  static void GetMousePosition(int &x, int &y);
  static void GetMouseDelta(int &dx, int &dy);

  // Text Input
  static void StartTextInput() {
    SDL_StartTextInput();
    m_TextInput = "";
    m_TextInputActive = true;
  }
  static void StopTextInput() {
    SDL_StopTextInput();
    m_TextInputActive = false;
  }
  static bool IsTextInputActive() { return m_TextInputActive; }
  static std::string GetTextInput() { return m_TextInput; }
  static void ClearTextInput() { m_TextInput = ""; }
  static void ClearKeys();

private:
  static Uint8 m_KeyboardState[SDL_NUM_SCANCODES];
  static Uint8 m_PrevKeyboardState[SDL_NUM_SCANCODES];
  static Uint32 m_MouseState;
  static Uint32 m_PrevMouseState;
  static int m_MouseX, m_MouseY;
  static int m_MouseRelX, m_MouseRelY;
  static SDL_Renderer *m_Renderer;
  static float m_ScaleX, m_ScaleY;

  static std::string m_TextInput;
  static bool m_TextInputActive;
};

} // namespace PixelsEngine