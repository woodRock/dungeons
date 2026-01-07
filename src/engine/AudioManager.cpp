#include "AudioManager.h"
#include <cmath>
#include <iostream>

AudioManager::AudioManager()
    : m_SfxVolume(MIX_MAX_VOLUME), m_MusicVolume(MIX_MAX_VOLUME) {}

AudioManager::~AudioManager() { Cleanup(); }

Mix_Chunk* AudioManager::LoadWAV(const std::string& path) {
  // Check cache first
  auto it = m_ChunkCache.find(path);
  if (it != m_ChunkCache.end()) {
    return it->second;
  }

  Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
  if (!chunk) {
    std::cerr << "Failed to load WAV: " << path << " - "
              << Mix_GetError() << std::endl;
    return nullptr;
  }

  m_ChunkCache[path] = chunk;
  return chunk;
}

Mix_Music* AudioManager::LoadMusic(const std::string& path) {
  // Check cache first
  auto it = m_MusicCache.find(path);
  if (it != m_MusicCache.end()) {
    return it->second;
  }

  Mix_Music* music = Mix_LoadMUS(path.c_str());
  if (!music) {
    std::cerr << "Failed to load music: " << path << " - "
              << Mix_GetError() << std::endl;
    return nullptr;
  }

  m_MusicCache[path] = music;
  return music;
}

int AudioManager::PlaySfx(Mix_Chunk* chunk, int loops) {
  if (!chunk) {
    return -1;
  }
  int channel = Mix_PlayChannel(-1, chunk, loops);
  if (channel != -1) {
    Mix_Volume(channel, m_SfxVolume);
  }
  return channel;
}

int AudioManager::PlaySfxSpatial(Mix_Chunk* chunk, float x, float y, float z,
                                  float cameraX, float cameraY,
                                  float cameraYaw, float maxDistance) {
  if (!chunk) {
    return -1;
  }

  int channel = Mix_PlayChannel(-1, chunk, 0);
  if (channel == -1) {
    return -1;
  }

  // Calculate distance and volume
  float dx = x - cameraX;
  float dy = y - cameraY;
  float dist = std::sqrt(dx * dx + dy * dy);
  int volume = (int)(m_SfxVolume * (1.0f - std::min(1.0f, dist / maxDistance)));
  Mix_Volume(channel, volume);

  // Calculate panning based on angle relative to camera direction
  float angle = std::atan2(dy, dx) - cameraYaw;
  float pan = std::sin(angle);
  Uint8 left = (Uint8)(127 * (1.0f - pan));
  Uint8 right = (Uint8)(127 * (1.0f + pan));
  Mix_SetPanning(channel, left, right);

  return channel;
}

int AudioManager::PlayMusic(Mix_Music* music, int loops) {
  if (!music) {
    return -1;
  }
  Mix_VolumeMusic(m_MusicVolume);
  return Mix_PlayMusic(music, loops);
}

void AudioManager::StopAllSfx() { Mix_HaltChannel(-1); }

void AudioManager::StopMusic() { Mix_HaltMusic(); }

void AudioManager::FreeChunk(Mix_Chunk*& chunk) {
  if (!chunk) {
    return;
  }

  // Find and remove from cache if it exists
  for (auto it = m_ChunkCache.begin(); it != m_ChunkCache.end(); ++it) {
    if (it->second == chunk) {
      Mix_FreeChunk(chunk);
      m_ChunkCache.erase(it);
      chunk = nullptr;
      return;
    }
  }

  // If not in cache, just free it directly
  Mix_FreeChunk(chunk);
  chunk = nullptr;
}

void AudioManager::FreeMusic(Mix_Music*& music) {
  if (!music) {
    return;
  }

  // Find and remove from cache if it exists
  for (auto it = m_MusicCache.begin(); it != m_MusicCache.end(); ++it) {
    if (it->second == music) {
      Mix_FreeMusic(music);
      m_MusicCache.erase(it);
      music = nullptr;
      return;
    }
  }

  // If not in cache, just free it directly
  Mix_FreeMusic(music);
  music = nullptr;
}

void AudioManager::SetSfxVolume(int volume) {
  m_SfxVolume = volume;
  Mix_Volume(-1, volume);
}

void AudioManager::SetMusicVolume(int volume) {
  m_MusicVolume = volume;
  Mix_VolumeMusic(volume);
}

void AudioManager::Cleanup() {
  // Free all cached chunks
  for (auto& pair : m_ChunkCache) {
    Mix_FreeChunk(pair.second);
  }
  m_ChunkCache.clear();

  // Free all cached music
  for (auto& pair : m_MusicCache) {
    Mix_FreeMusic(pair.second);
  }
  m_MusicCache.clear();
}
