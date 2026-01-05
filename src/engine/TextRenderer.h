#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

#include "GLRenderer.h"

namespace PixelsEngine {

class TextRenderer {
public:
  TextRenderer(const std::string &fontPath, int fontSize);
  ~TextRenderer();

  void RenderText(GLRenderer *ren, const std::string &text, int x, int y,
                  SDL_Color color);
  void RenderTextCentered(GLRenderer *ren, const std::string &text, int x,
                          int y, SDL_Color color);
  void RenderTextWrappedCentered(GLRenderer *ren, const std::string &text,
                                 int x, int y, int wrapWidth, SDL_Color color);

private:
  TTF_Font *m_Font = nullptr;
  TTF_Font *m_SmallFont = nullptr;
};

} // namespace PixelsEngine
