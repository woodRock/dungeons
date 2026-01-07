#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <memory>

/**
 * AudioManager handles all audio loading, playback, and resource management.
 * Centralizes SDL_mixer operations and provides both spatial and non-spatial audio.
 */
class AudioManager {
public:
  AudioManager();
  ~AudioManager();

  /**
   * Load a WAV file into memory
   * @param path File path to the WAV file
   * @return Pointer to loaded chunk, or nullptr on failure
   */
  Mix_Chunk* LoadWAV(const std::string& path);

  /**
   * Load a music file into memory
   * @param path File path to the music file
   * @return Pointer to loaded music, or nullptr on failure
   */
  Mix_Music* LoadMusic(const std::string& path);

  /**
   * Play a sound effect on any available channel
   * @param chunk Pointer to the loaded sound
   * @param loops Number of times to loop (-1 for infinite)
   * @return Channel index, or -1 on failure
   */
  int PlaySfx(Mix_Chunk* chunk, int loops = 0);

  /**
   * Play a sound effect with spatial audio (volume and panning based on position)
   * @param chunk Pointer to the loaded sound
   * @param x Sound source X position
   * @param y Sound source Y position
   * @param z Sound source Z position
   * @param cameraX Camera X position for distance/angle calculation
   * @param cameraY Camera Y position for distance/angle calculation
   * @param cameraYaw Camera yaw angle for panning calculation
   * @param maxDistance Maximum distance before sound is silent
   * @return Channel index, or -1 on failure
   */
  int PlaySfxSpatial(Mix_Chunk* chunk, float x, float y, float z,
                      float cameraX, float cameraY, float cameraYaw,
                      float maxDistance = 20.0f);

  /**
   * Play background music
   * @param music Pointer to the loaded music
   * @param loops Number of times to loop (-1 for infinite)
   * @return 0 on success, -1 on failure
   */
  int PlayMusic(Mix_Music* music, int loops = -1);

  /**
   * Stop all sound effects
   */
  void StopAllSfx();

  /**
   * Stop background music
   */
  void StopMusic();

  /**
   * Free a loaded sound effect (and manage cache)
   * @param chunk Reference to the pointer (will be set to nullptr)
   */
  void FreeChunk(Mix_Chunk*& chunk);

  /**
   * Free a loaded music file (and manage cache)
   * @param music Reference to the pointer (will be set to nullptr)
   */
  void FreeMusic(Mix_Music*& music);

  /**
   * Set master volume for all sound effects (0-128)
   */
  void SetSfxVolume(int volume);

  /**
   * Set master volume for music (0-128)
   */
  void SetMusicVolume(int volume);

private:
  std::unordered_map<std::string, Mix_Chunk*> m_ChunkCache;
  std::unordered_map<std::string, Mix_Music*> m_MusicCache;
  int m_SfxVolume;
  int m_MusicVolume;

  /**
   * Clean up SDL_mixer resources
   */
  void Cleanup();
};
