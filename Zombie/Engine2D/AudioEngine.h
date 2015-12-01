#pragma once

#include <Mixer/SDL_mixer.h>

#include <string>
#include <map>

namespace Engine2D {

	class SoundEffect {
	public:
		friend class AudioEngine;

		void Play(int loops = 0);
	private:
		Mix_Chunk* m_chunk = nullptr;
	};

	class Music {
	public:
		friend class AudioEngine;

		void Play(int loops = 1);

		static void Pause();
		static void Stop();
		static void Resume();
	private:
		Mix_Music* m_music = nullptr;
	};

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void Init();
		void Destory();
		void SetEffectVolume(const std::string& filePath, int volume); /// 0 - 128
		void SetMusicVolume(int volume); /// 0 - 128

		SoundEffect loadSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);
	private:
		bool m_isInit = false;
		std::map<std::string, Mix_Chunk*> m_EffectMap;
		std::map<std::string, Mix_Music*> m_MusicMap;
	};
}
