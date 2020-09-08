/**
* @file ArsDirectSound.h
* @brief Direct�T�E���h�𐶐�����N���X
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
				_ASSERT_EXPR(false, L"�T�E���h�f�o�C�X�̐����Ɏ��s���܂���");
			}
			mpSoundDevice->SetCooperativeLevel(apHwnd, DSSCL_PRIORITY);
		}
		~DirectSound(void)
		{
		}

		/**
		* @brief �T�E���h�f�o�C�X�����܂�
		*/
		const LPDIRECTSOUND8 GetSoundDevice(void)const { return mpSoundDevice.Get(); }

	private:
		ComPtr<IDirectSound8> mpSoundDevice;
	};
}