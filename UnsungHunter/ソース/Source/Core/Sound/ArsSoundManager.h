/**
* @file ArsSoundManager.h
* @brief サウンドの読み込み、管理クラス
*/

#pragma once

#include "ArsSound.h"
#include "../ArsSingleton.h"

namespace Alrescha {
	class DirectSound;
	class SoundManager:public Singleton<SoundManager>
	{
	public:
		friend class Singleton<SoundManager>;

		void Initialize(HWND apHwnd);

		void Finalize(void);

		void Update(void);

		//再生＆停止等細かく鳴らしたい人向け
		SPtr<Sound> GetSound(const std::string& aSoundName, UINT aVolume = 30);
		//ただただ鳴らしたい人向け(途中で停止や速度調整等ができない)
		void Play(const std::string& aSoundName, UINT aVolume = 30);

		//ループ再生させる
		void AddLoopSound(const std::string& aSoundName);
		//ループ再生している曲を取り除く
		void RemoveLoopSound(const std::string& aSoundName);
		Sound& GetLoopSound(const std::string& aSoundName);

	private:
		bool CreateSound(const std::string& aSoundName);
		ComPtr<IDirectSoundBuffer8> CreateSecondaryBuffer(const std::string& aSoundName)const;
		ComPtr<IDirectSoundBuffer8> CloneSecondaryBuffer(const std::string& aSoundName, const LPDIRECTSOUNDBUFFER& aOriginalBuffer)const;

		void ReadWave(const std::string& aPath, WAVEFORMATEX* apWaveFormatEx, DWORD* apBufferByte, char** apWaveData)const;

		UPtr<DirectSound> mpDirectSound;
		std::unordered_map<std::string, std::list<SPtr<Sound>>> mSoundMap;
		//クローンのため必要
		std::unordered_map<std::string, std::list<ComPtr<IDirectSoundBuffer8>>> mBufferMap;

		std::unordered_map<std::string, SPtr<Sound>>mLoopSoundMap;
	protected:
		SoundManager(void);
		virtual ~SoundManager(void)override final;
	};
	inline SoundManager& GetSoundManager(void) { return SoundManager::GetInstance(); }
}