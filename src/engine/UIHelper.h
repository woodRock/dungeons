#pragma once

#include <string>
#include <SDL2/SDL.h>

namespace PixelsEngine {
class GLRenderer;
class TextRenderer;
} // namespace PixelsEngine

using PixelsEngine::GLRenderer;
using PixelsEngine::TextRenderer;

/**
 * UIHelper provides reusable functions for rendering common UI elements.
 * Ensures consistent appearance and reduces duplication across game modes.
 */
class UIHelper {
public:
  /**
   * Draw a crosshair in the center of the screen
   * @param renderer GLRenderer instance
   * @param centerX Screen center X coordinate
   * @param centerY Screen center Y coordinate
   * @param size Size of crosshair arms
   * @param color RGBA color
   */
  static void DrawCrosshair(GLRenderer* renderer, int centerX, int centerY,
                             int size = 10,
                             SDL_Color color = {255, 255, 255, 150});

  /**
   * Draw a health bar
   * @param renderer GLRenderer instance
   * @param x Screen X position
   * @param y Screen Y position
   * @param width Bar width
   * @param height Bar height
   * @param hp Current health
   * @param maxHp Maximum health
   * @param fgColor Foreground (health) color
   * @param bgColor Background color
   */
  static void DrawHealthBar(GLRenderer* renderer, int x, int y, int width,
                             int height, int hp, int maxHp,
                             SDL_Color fgColor = {255, 50, 50, 255},
                             SDL_Color bgColor = {50, 50, 50, 200});

  /**
   * Draw text with consistent styling
   * @param textRenderer TextRenderer instance
   * @param glRenderer GLRenderer instance
   * @param text Text to render
   * @param x Screen X position
   * @param y Screen Y position
   * @param color Text color
   * @param centered Whether to center the text at the given position
   */
  static void DrawText(TextRenderer* textRenderer, GLRenderer* glRenderer,
                        const std::string& text, int x, int y,
                        SDL_Color color = {255, 255, 255, 255},
                        bool centered = false);

  /**
   * Draw hitmarker (visual feedback for hits)
   * @param renderer GLRenderer instance
   * @param centerX Screen center X
   * @param centerY Screen center Y
   * @param size Size of hitmarker corners
   * @param color Hitmarker color
   */
  static void DrawHitmarker(GLRenderer* renderer, int centerX, int centerY,
                             int size = 15,
                             SDL_Color color = {255, 0, 0, 255});

  /**
   * Draw a rectangular background panel
   * @param renderer GLRenderer instance
   * @param x Screen X position
   * @param y Screen Y position
   * @param width Panel width
   * @param height Panel height
   * @param color Panel color
   */
  static void DrawPanel(GLRenderer* renderer, int x, int y, int width,
                        int height,
                        SDL_Color color = {50, 50, 50, 200});
};
