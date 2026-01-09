#pragma once

#include "TextRenderer.h"
#include "GLRenderer.h"
#include "UIHelper.h"
#include <string>
#include <vector>

namespace PixelsEngine {

struct GameOverStats {
    std::string title;              // "MISSION SUCCESS", "VICTORY", "GAME OVER", etc.
    SDL_Color titleColor;           // {0, 255, 100, 255} for success, {255, 50, 50, 255} for failure
    bool isVictory;                 // Determines button text
    std::vector<std::pair<std::string, std::string>> stats;  // ("Label", "Value") pairs
};

class GameOverScreen {
public:
    static void Render(GLRenderer* renderer, TextRenderer* textRenderer, 
                      int width, int height, const GameOverStats& stats) {
        // Overlay background
        renderer->DrawRect2D(0, 0, width, height, {0, 0, 0, 180});
        
        int centerX = width / 2;
        int centerY = height / 2;
        
        // Title
        textRenderer->RenderTextCentered(renderer, stats.title, centerX, centerY - 100, 
                                        stats.titleColor);
        
        // Stats Panel
        int panelW = 400;
        int panelH = 50 + (stats.stats.size() * 30);
        UIHelper::DrawPanel(renderer, centerX - panelW/2, centerY - 60, panelW, panelH, 
                           {30, 30, 40, 220});
        
        int statsY = centerY - 40;
        for (const auto& stat : stats.stats) {
            std::string statLine = stat.first + ": " + stat.second;
            textRenderer->RenderText(renderer, statLine, centerX - 180, statsY, 
                                    {200, 200, 200, 255});
            statsY += 30;
        }
        
        // Buttons
        int btnY = centerY + panelH/2 + 20;
        std::string retryText = stats.isVictory ? "NEXT LEVEL" : "RETRY";
        textRenderer->RenderTextCentered(renderer, "Press [R] to " + retryText, centerX, btnY, 
                                        {255, 255, 255, 255});
        textRenderer->RenderTextCentered(renderer, "Press [ESC] to Exit", centerX, btnY + 30, 
                                        {200, 200, 200, 255});
    }
};

}  // namespace PixelsEngine
