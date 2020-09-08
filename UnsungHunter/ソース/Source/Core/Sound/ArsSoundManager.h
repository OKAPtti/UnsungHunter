/**
* @file ArsSoundManager.h
* @brief �T�E���h�̓ǂݍ��݁A�Ǘ��N���X
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

		//�Đ�����~���ׂ����炵�����l����
		SPtr<Sound> GetSound(const std::string& aSoundName, UINT aVolume = 30);
		//���������炵�����l����(�r���Œ�~�⑬�x���������ł��Ȃ�)
		void Play(const std::string& aSoundName, UINT aVolume = 30);

		//���[�v�Đ�������
		void AddLoopSound(const std::string& aSoundName);
		//���[�v�Đ����Ă���Ȃ���菜��
		void RemoveLoopSound(const std::string& aSoundName);
		Sound& GetLoopSound(const std::string& aSoundName);

	private:
		bool CreateSound(const std::string& aSoundName);
		ComPtr<IDirectSoundBuffer8> CreateSecondaryBuffer(const std::string& aSoundName)const;
		ComPtr<IDirectSoundBuffer8> CloneSecondaryBuffer(const std::string& aSoundName, const LPDIRECTSOUNDBUFFER& aOriginalBuffer)const;

		void ReadWave(const std::string& aPath, WAVEFORMATEX* apWaveFormatEx, DWORD* apBufferByte, char** apWaveData)const;

		UPtr<DirectSound> mpDirectSound;
		std::unordered_map<std::string, std::list<SPtr<Sound>>> mSoundMap;
		//�N���[���̂��ߕK�v
		std::unordered_map<std::string, std::list<ComPtr<IDirectSoundBuffer8>>> mBufferMap;

		std::unordered_map<std::string, SPtr<Sound>>mLoopSoundMap;
	protected:
		SoundManager(void);
		virtual ~SoundManager(void)override final;
	};
	inline SoundManager& GetSoundManager(void) { return SoundManager::GetInstance(); }
}