#include "Console.h"
#include "Input.h"
#include "GLRenderer.h"
#include "TextRenderer.h"
#include <iostream>
#include <sstream>

namespace PixelsEngine {

Console::Console() {
    m_OutputLog.push_back("Console initialized. Type /help for commands.");
}

Console::~Console() = default;

void Console::RegisterCommand(const std::string& name, CommandCallback callback) {
    m_Commands[name] = callback;
}

void Console::ExecuteCommand(const std::string& command) {
    // Add to output log
    m_OutputLog.push_back("> " + command);
    
    // Keep log size manageable
    if (m_OutputLog.size() > m_MaxLogLines * 2) {
        m_OutputLog.erase(m_OutputLog.begin());
    }
    
    // Parse command
    std::istringstream iss(command);
    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }
    
    if (args.empty()) return;
    
    std::string cmdName = args[0];
    
    // Check if command exists
    if (m_Commands.find(cmdName) != m_Commands.end()) {
        m_Commands[cmdName](args);
    } else {
        m_OutputLog.push_back("Unknown command: " + cmdName);
        if (m_OutputLog.size() > m_MaxLogLines * 2) {
            m_OutputLog.erase(m_OutputLog.begin());
        }
    }
}

void Console::AddLog(const std::string& text) {
    m_OutputLog.push_back(text);
    if (m_OutputLog.size() > m_MaxLogLines * 2) {
        m_OutputLog.erase(m_OutputLog.begin());
    }
}

void Console::ProcessInput() {
    if (!m_IsOpen || !m_InputEnabled) return;
    
    std::string textInput = Input::GetTextInput();
    if (!textInput.empty()) {
        m_InputBuffer += textInput;
        Input::ClearTextInput();
    }
    
    // Handle backspace
    if (Input::IsKeyPressed(SDL_SCANCODE_BACKSPACE) && !m_InputBuffer.empty()) {
        m_InputBuffer.pop_back();
    }
    
    // Handle enter
    if (Input::IsKeyPressed(SDL_SCANCODE_RETURN)) {
        if (!m_InputBuffer.empty()) {
            ExecuteCommand(m_InputBuffer);
            m_InputBuffer.clear();
        }
    }
}

void Console::Update() {
    if (Input::IsKeyPressed(SDL_SCANCODE_GRAVE)) {
        Toggle();
        if (m_IsOpen) {
            Input::StartTextInput();
        } else {
            Input::StopTextInput();
        }
    }
    
    ProcessInput();
}

void Console::Render(GLRenderer* renderer, TextRenderer* textRenderer, int screenWidth, int screenHeight) {
    if (!m_IsOpen) return;
    
    // Draw console background at bottom of screen
    int consoleY = screenHeight - m_ConsoleHeight;
    renderer->DrawRect2D(0, consoleY, screenWidth, m_ConsoleHeight, {0, 0, 0, 200});
    renderer->DrawRect2D(0, consoleY, screenWidth, 2, {100, 255, 100, 255});
    
    // Draw output log
    int logStartY = consoleY + 10;
    int visibleLines = (m_ConsoleHeight - 40) / 18;  // Account for padding
    int startIdx = std::max(0, (int)m_OutputLog.size() - visibleLines);
    
    for (size_t i = startIdx; i < m_OutputLog.size(); ++i) {
        int lineY = logStartY + (i - startIdx) * 18;
        textRenderer->RenderText(renderer, m_OutputLog[i], 10, lineY, {200, 200, 200, 255});
    }
    
    // Draw input prompt at bottom of console
    int inputY = consoleY + m_ConsoleHeight - 30;
    textRenderer->RenderText(renderer, "> " + m_InputBuffer + "_", 10, inputY, {0, 255, 0, 255});
}

} // namespace PixelsEngine
