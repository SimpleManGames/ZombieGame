#include "AudioEngine.h"
#include "Errors.h"

namespace Engine2D {


	void SoundEffect::Play(int loops) {
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1) {
			fatalError("Playing effect failed." + std::string(Mix_GetError()));
		}
	}
	void Music::Play(int loops) {
		if (Mix_PlayMusic(m_music, loops) == -1) {
			fatalError("Playing music failed." + std::string(Mix_GetError()));
		}
	}

	void Music::Pause() {
		Mix_PauseMusic();
	}

	void Music::Stop() {
		Mix_HaltMusic();
	}

	void Music::Resume() {
		Mix_ResumeMusic();
	}

	AudioEngine::AudioEngine()
	{
	}
	AudioEngine::~AudioEngine() {
		Destory();
	}
	void AudioEngine::Init() {
		if (!m_isInit) {
			if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
				fatalError("Mix_Init error: " + std::string(Mix_GetError()));
			}
			if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
				fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
			}
			m_isInit = true;
		}
		else
			fatalError("Cannot call Init twice for AudioEngine.");
	}
	void AudioEngine::Destory() {
		if (m_isInit) {
			m_isInit = false;

			for (auto& it : m_EffectMap)
				Mix_FreeChunk(it.second);

			for (auto& it : m_MusicMap)
				Mix_FreeMusic(it.second);

			m_EffectMap.clear();
			m_MusicMap.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}
	void AudioEngine::SetEffectVolume(const std::string& filePath, int volume) {
		auto it = m_EffectMap.find(filePath);

		if (it == m_EffectMap.end()) {
			fatalError("Changing effect volume failed.");
		}
		else
			Mix_VolumeChunk(it->second, volume);
	}
	void AudioEngine::SetMusicVolume(int volume) {
			Mix_VolumeMusic(volume);
	}
	SoundEffect AudioEngine::loadSoundEffect(const std::string & filePath) {
		auto it = m_EffectMap.find(filePath);

		SoundEffect effect;

		if (it == m_EffectMap.end()) {
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr) {
				fatalError("Loading sound effect failed.");
			}

			effect.m_chunk = chunk;
			m_EffectMap[filePath] = chunk;
		}
		else
			effect.m_chunk = it->second;

		return effect;
	}
	Music AudioEngine::loadMusic(const std::string & filePath) {
		auto it = m_MusicMap.find(filePath);

		Music music;

		if (it == m_MusicMap.end()) {
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr) {
				fatalError("Loading music failed.");
			}

			music.m_music = mixMusic;
			m_MusicMap[filePath] = mixMusic;
		}
		else
			music.m_music = it->second;

		return music;
	}
}