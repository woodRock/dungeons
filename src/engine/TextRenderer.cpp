#include "OpenGL.h"
#include "TextRenderer.h"

namespace PixelsEngine {

TextRenderer::TextRenderer(const std::string &fontPath, int fontSize) {
  m_Font = TTF_OpenFont(fontPath.c_str(), fontSize);
  m_SmallFont = TTF_OpenFont(fontPath.c_str(), fontSize - 6);
  if (!m_Font) {
    std::cerr << "Failed to load font: " << fontPath
              << " Error: " << TTF_GetError() << std::endl;
  }
}

TextRenderer::~TextRenderer() {
  if (m_Font)
    TTF_CloseFont(m_Font);
  if (m_SmallFont)
    TTF_CloseFont(m_SmallFont);
}

void TextRenderer::RenderText(GLRenderer *ren, const std::string &text, int x,
                              int y, SDL_Color color) {
  if (!m_Font || text.empty())
    return;

  SDL_Surface *surface =
      TTF_RenderText_Blended(m_Font, text.c_str(), {255, 255, 255, 255});
  if (!surface)
    return;

  unsigned int texID = ren->CreateTextureFromSurface(surface);
  if (texID) {
    ren->DrawTexture2D(texID, x, y, surface->w, surface->h, color);
    glDeleteTextures(1, &texID);
  }
  SDL_FreeSurface(surface);
}

void TextRenderer::RenderTextCentered(GLRenderer *ren, const std::string &text,
                                      int x, int y, SDL_Color color) {
  if (!m_Font || text.empty())
    return;

  SDL_Surface *surface =
      TTF_RenderText_Blended(m_Font, text.c_str(), {255, 255, 255, 255});
  if (!surface)
    return;

  unsigned int texID = ren->CreateTextureFromSurface(surface);
  if (texID) {
    ren->DrawTexture2D(texID, x - surface->w / 2, y - surface->h / 2,
                       surface->w, surface->h, color);
    glDeleteTextures(1, &texID);
  }
  SDL_FreeSurface(surface);
}

void TextRenderer::RenderTextWrappedCentered(GLRenderer *ren,
                                             const std::string &text, int x,
                                             int y, int wrapWidth,
                                             SDL_Color color) {
  if (!m_Font || text.empty())
    return;

  SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(
      m_Font, text.c_str(), {255, 255, 255, 255}, wrapWidth);
  if (!surface)
    return;

  unsigned int texID = ren->CreateTextureFromSurface(surface);
  if (texID) {
    ren->DrawTexture2D(texID, x - surface->w / 2, y, surface->w, surface->h,
                       color);
    glDeleteTextures(1, &texID);
  }
  SDL_FreeSurface(surface);
}

} // namespace PixelsEngine