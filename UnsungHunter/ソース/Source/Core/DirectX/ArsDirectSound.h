/**
* @file ArsDirectSound.h
* @brief Directサウンドを生成するクラス
*/

#pragma once

#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )

namespace Alrescha {
	class DirectSound 
	{
	public:
		DirectSound(HWND apHwnd)
		{
			if (FAILED(DirectSoundCreate8(NULL, &mpSoundDevice, NULL))) {
				_ASSERT_EXPR(false, L"サウンドデバイスの生成に失敗しました");
			}
			mpSoundDevice->SetCooperativeLevel(apHwnd, DSSCL_PRIORITY);
		}
		~DirectSound(void)
		{
		}

		/**
		* @brief サウンドデバイスをします
		*/
		const LPDIRECTSOUND8 GetSoundDevice(void)const { return mpSoundDevice.Get(); }

	private:
		ComPtr<IDirectSound8> mpSoundDevice;
	};
}