#pragma once
#include <string>

class GameSettings {
public:
  static GameSettings& Instance() {
    static GameSettings instance;
    return instance;
  }

  // Audio
  float GetMusicVolume() const { return m_MusicVolume; }
  void SetMusicVolume(float volume) { m_MusicVolume = (volume < 0) ? 0 : (volume > 1) ? 1 : volume; }

  float GetSFXVolume() const { return m_SFXVolume; }
  void SetSFXVolume(float volume) { m_SFXVolume = (volume < 0) ? 0 : (volume > 1) ? 1 : volume; }

  // Graphics
  bool GetFullscreen() const { return m_Fullscreen; }
  void SetFullscreen(bool fullscreen) { m_Fullscreen = fullscreen; }

  float GetMouseSensitivity() const { return m_MouseSensitivity; }
  void SetMouseSensitivity(float sensitivity) { m_MouseSensitivity = (sensitivity < 0.1f) ? 0.1f : (sensitivity > 2.0f) ? 2.0f : sensitivity; }

  // Difficulty
  int GetDifficulty() const { return m_Difficulty; } // 0=Easy, 1=Normal, 2=Hard
  void SetDifficulty(int difficulty) { m_Difficulty = (difficulty < 0) ? 0 : (difficulty > 2) ? 2 : difficulty; }

  std::string GetDifficultyName() const {
    switch (m_Difficulty) {
      case 0: return "Easy";
      case 1: return "Normal";
      case 2: return "Hard";
      default: return "Normal";
    }
  }

private:
  GameSettings() 
    : m_MusicVolume(0.7f), m_SFXVolume(0.8f), m_Fullscreen(false), 
      m_MouseSensitivity(1.0f), m_Difficulty(1) {}

  float m_MusicVolume = 0.7f;
  float m_SFXVolume = 0.8f;
  bool m_Fullscreen = false;
  float m_MouseSensitivity = 1.0f;
  int m_Difficulty = 1; // 0=Easy, 1=Normal, 2=Hard
};
