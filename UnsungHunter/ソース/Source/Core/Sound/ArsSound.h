/**
* @file ArsSound.h
* @brief �T�E���h�𐧌䂷��N���X
*/

#pragma once

namespace Alrescha {

	class Sound
	{
	public:
		Sound(LPDIRECTSOUNDBUFFER8 apSoundBuf)
			:mpDirectSoundBuffer(apSoundBuf)
			, mHz(0)
		{
			mpDirectSoundBuffer->GetFrequency(&mHz);
		}
		~Sound(void)
		{

		}

		/**
		* @brief �����Đ����܂�
		* @param ���v���C���邩�ǂ���(�w�肪�Ȃ��ꍇ��false)
		* @note ������true�Ȃ�Đ����ł��ŏ�����Đ����Ȃ���
		*/
		void Play(bool aReplay = false)const
		{
			if (IsPlaying()) {
				if (aReplay) {
					mpDirectSoundBuffer->SetCurrentPosition(0);
				}
			}
			else {
				mpDirectSoundBuffer->SetCurrentPosition(0);
				mpDirectSoundBuffer->Play(0, 0, 0);
			}
		}

		/**
		* @brief �����~���܂�
		*/
		void Stop(void)
		{
			mpDirectSoundBuffer->Stop();
		}

		/**
		* @brief �����Đ�����Ă��邩�ǂ�����Ԃ��܂�
		* @note �Đ�����Ă����true
		*/
		bool IsPlaying(void)const
		{
			DWORD stats = 0;
			mpDirectSoundBuffer->GetStatus(&stats);
			return stats == DSBSTATUS_PLAYING;
		}

		/**
		* @brief ���̍Đ����x��ݒ肵�܂�
		* @param �Đ��{��
		*/
		void SetSpeed(float aSpeedMagnification)
		{
			mpDirectSoundBuffer->SetFrequency(static_cast<DWORD>(mHz * aSpeedMagnification));
			DWORD i;
			mpDirectSoundBuffer->GetFrequency(&i);
		}

		/**
		* @brief ���ʂ�ݒ肵�܂�
		* @param 0(����)�`100(�ő�)
		*/
		void SetVolume(UINT aVolume)
		{
			LONG volume = DSBVOLUME_MIN;
			//0 �` -600db
			if (aVolume >= 50) {
				volume = -600 + 600 * (aVolume - 50) / 50;
			}
			//-600 �` -1200db 
			else if (aVolume >= 25) {
				volume = -1200 + 600 * (aVolume - 25) / 25;
			}
			//-1200 �` -1800
			else if (aVolume >= 12) {
				volume = -1800 + 600 * (aVolume - 12) / 12;
			}
			//-1800 �` -2400
			else if (aVolume >= 6) {
				volume = -2400 + 600 * (aVolume - 6) / 6;
			}
			//-2400 �` -3000
			else if (aVolume >= 3) {
				volume = -3000 + 600 * (aVolume - 3) / 3;
			}
			// -3000�`-3600
			else if (aVolume >= 1) {
				volume = -3600 + 600 * (aVolume - 1);
			}
			mpDirectSoundBuffer->SetVolume(volume);
		}

	private:
		LPDIRECTSOUNDBUFFER8 mpDirectSoundBuffer;

		DWORD mHz;
	};

}