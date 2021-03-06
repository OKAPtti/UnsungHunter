/**
* @file ArsSound.h
* @brief サウンドを制御するクラス
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
		* @brief 音を再生します
		* @param リプレイするかどうか(指定がない場合はfalse)
		* @note 引数がtrueなら再生中でも最初から再生しなおす
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
		* @brief 音を停止します
		*/
		void Stop(void)
		{
			mpDirectSoundBuffer->Stop();
		}

		/**
		* @brief 音が再生されているかどうかを返します
		* @note 再生されていればtrue
		*/
		bool IsPlaying(void)const
		{
			DWORD stats = 0;
			mpDirectSoundBuffer->GetStatus(&stats);
			return stats == DSBSTATUS_PLAYING;
		}

		/**
		* @brief 音の再生速度を設定します
		* @param 再生倍率
		*/
		void SetSpeed(float aSpeedMagnification)
		{
			mpDirectSoundBuffer->SetFrequency(static_cast<DWORD>(mHz * aSpeedMagnification));
			DWORD i;
			mpDirectSoundBuffer->GetFrequency(&i);
		}

		/**
		* @brief 音量を設定します
		* @param 0(無音)〜100(最大)
		*/
		void SetVolume(UINT aVolume)
		{
			LONG volume = DSBVOLUME_MIN;
			//0 〜 -600db
			if (aVolume >= 50) {
				volume = -600 + 600 * (aVolume - 50) / 50;
			}
			//-600 〜 -1200db 
			else if (aVolume >= 25) {
				volume = -1200 + 600 * (aVolume - 25) / 25;
			}
			//-1200 〜 -1800
			else if (aVolume >= 12) {
				volume = -1800 + 600 * (aVolume - 12) / 12;
			}
			//-1800 〜 -2400
			else if (aVolume >= 6) {
				volume = -2400 + 600 * (aVolume - 6) / 6;
			}
			//-2400 〜 -3000
			else if (aVolume >= 3) {
				volume = -3000 + 600 * (aVolume - 3) / 3;
			}
			// -3000〜-3600
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