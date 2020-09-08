/**
* @file ArsDirectGraphics.h
* @brief �O���t�B�b�N�X�𐧌䂷��N���X
*/

#pragma once

#include "../ArsSingleton.h"
#include "../Camera/ArsCameraSystem.h"

#pragma comment(lib, "DirectX/d3d9.lib")
#pragma comment(lib, "DirectX/d3dx9.lib")

namespace Alrescha {
	class Sprite;
	class CameraSystem;
	class DirectGraphics:public Singleton<DirectGraphics> 
	{
	public:
		friend class Singleton<DirectGraphics>;

		/**
		* @brief �����������܂�
		* @note ���C�g����̐ݒ�
		*/
		void Initialize(HWND apHwnd,bool aIsFullScreen);

		/**
		* @brief DirectX9�ɕK�v�Ȃ��̂𐶐����܂�.
		*/
		void CreateDirectX9(HWND apHwnd, bool aIsFullScreen);

		/**
		* @brief �f�o�C�X��Ԃ��܂�.
		*/
		const LPDIRECT3DDEVICE9 GetDevice(void)const { return mpD3DDevice.Get(); }

		/**
		* @brief �X�v���C�g��Ԃ��܂�.
		*/
		Sprite& GetSprite(void)					{ return *mpSprite; }
	
		/**
		* @brief ���Z�������[�h�ɐ؂�ւ��܂�
		*/
		void Blend_Add(void) 
		{
			mpD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			mpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			mpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		/**
		* @brief �������������[�h�ɐ؂�ւ��܂�
		*/
		void Blend_Alpha(void) 
		{
			mpD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			mpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			mpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		/**
		* @brief �f�o�C�X���փ��[���h�s����Z�b�g���܂�
		* @param �Z�b�g�������s��
		*/
		void SetWorldMatrix(const D3DXMATRIX* apMatrix) {
			
			mpD3DDevice->SetTransform(D3DTS_WORLD, apMatrix);
		}
		/**
		* @brief �J�������Z�b�g���܂�
		* @param �Z�b�g�������J�����N���X
		*/
		void SetCamera(const CameraSystem& aCamera);
		/**
		* @brief �J�����N���X��Ԃ��܂�
		*/
		const CameraSystem& GetCamera(void) { return mCamera; }
	private:
		ComPtr<IDirect3D9> mpD3D;
		ComPtr<IDirect3DDevice9> mpD3DDevice;
		D3DPRESENT_PARAMETERS	mD3Dpp;
		D3DLIGHT9				mLight;

		SPtr<Sprite>			mpSprite;
		//�J����
		CameraSystem			mCamera;

	protected:
		DirectGraphics(void);
		~DirectGraphics(void)override final;
	};
	inline DirectGraphics& Get3D(void) { return DirectGraphics::GetInstance(); }

}