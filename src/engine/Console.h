#pragma once
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace PixelsEngine {

class GLRenderer;
class TextRenderer;

class Console {
public:
    Console();
    ~Console();

    void Update();
    void Render(GLRenderer* renderer, TextRenderer* textRenderer, int screenWidth, int screenHeight);
    
    bool IsOpen() const { return m_IsOpen; }
    void Toggle() { m_IsOpen = !m_IsOpen; }
    void Open() { m_IsOpen = true; }
    void Close() { m_IsOpen = false; }
    
    // Enable/disable input processing
    void SetInputEnabled(bool enabled) { m_InputEnabled = enabled; }
    
    // Register a command callback
    using CommandCallback = std::function<void(const std::vector<std::string>&)>;
    void RegisterCommand(const std::string& name, CommandCallback callback);
    void ExecuteCommand(const std::string& command);
    
    // Add text to console output
    void AddLog(const std::string& text);
    
    // Process input (can be called separately)
    void ProcessInput();

private:
    std::vector<std::string> m_OutputLog;
    std::string m_InputBuffer;
    std::unordered_map<std::string, CommandCallback> m_Commands;
    bool m_IsOpen = false;
    bool m_InputEnabled = true;
    size_t m_MaxLogLines = 200;
    size_t m_ScrollOffset = 0;
    int m_ConsoleHeight = 200;
};

} // namespace PixelsEngine
