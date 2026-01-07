#include "UIHelper.h"
#include "GLRenderer.h"
#include "TextRenderer.h"
#include <algorithm>

using namespace PixelsEngine;

void UIHelper::DrawCrosshair(GLRenderer* renderer, int centerX, int centerY,
                              int size, SDL_Color color) {
  if (!renderer) {
    return;
  }

  int thickness = 2;
  // Vertical line
  renderer->DrawRect2D(centerX - thickness / 2, centerY - size, thickness,
                       size * 2, color);
  // Horizontal line
  renderer->DrawRect2D(centerX - size, centerY - thickness / 2, size * 2,
                       thickness, color);
}

void UIHelper::DrawHealthBar(GLRenderer* renderer, int x, int y, int width,
                              int height, int hp, int maxHp,
                              SDL_Color fgColor, SDL_Color bgColor) {
  if (!renderer || maxHp <= 0) {
    return;
  }

  // Draw background
  DrawPanel(renderer, x, y, width, height, bgColor);

  // Draw health fill
  int padding = 2;
  int healthWidth = (int)(((float)hp / maxHp) * (width - padding * 2));
  healthWidth = std::max(0, std::min(healthWidth, width - padding * 2));

  if (healthWidth > 0) {
    renderer->DrawRect2D(x + padding, y + padding, healthWidth,
                         height - padding * 2, fgColor);
  }
}

void UIHelper::DrawText(TextRenderer* textRenderer, GLRenderer* glRenderer,
                         const std::string& text, int x, int y,
                         SDL_Color color, bool centered) {
  if (!textRenderer || !glRenderer) {
    return;
  }

  if (centered) {
    textRenderer->RenderTextCentered(glRenderer, text, x, y, color);
  } else {
    textRenderer->RenderText(glRenderer, text, x, y, color);
  }
}

void UIHelper::DrawHitmarker(GLRenderer* renderer, int centerX, int centerY,
                              int size, SDL_Color color) {
  if (!renderer) {
    return;
  }

  int cornerSize = 5;
  // Top-left
  renderer->DrawRect2D(centerX - size, centerY - size, cornerSize, cornerSize,
                       color);
  // Top-right
  renderer->DrawRect2D(centerX + size - cornerSize, centerY - size, cornerSize,
                       cornerSize, color);
  // Bottom-left
  renderer->DrawRect2D(centerX - size, centerY + size - cornerSize, cornerSize,
                       cornerSize, color);
  // Bottom-right
  renderer->DrawRect2D(centerX + size - cornerSize, centerY + size - cornerSize,
                       cornerSize, cornerSize, color);
}

void UIHelper::DrawPanel(GLRenderer* renderer, int x, int y, int width,
                          int height, SDL_Color color) {
  if (!renderer) {
    return;
  }

  renderer->DrawRect2D(x, y, width, height, color);
}
