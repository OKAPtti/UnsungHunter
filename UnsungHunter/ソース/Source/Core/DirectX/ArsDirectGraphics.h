/**
* @file ArsDirectGraphics.h
* @brief グラフィックスを制御するクラス
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
		* @brief 初期化をします
		* @note ライトや光の設定
		*/
		void Initialize(HWND apHwnd,bool aIsFullScreen);

		/**
		* @brief DirectX9に必要なものを生成します.
		*/
		void CreateDirectX9(HWND apHwnd, bool aIsFullScreen);

		/**
		* @brief デバイスを返します.
		*/
		const LPDIRECT3DDEVICE9 GetDevice(void)const { return mpD3DDevice.Get(); }

		/**
		* @brief スプライトを返します.
		*/
		Sprite& GetSprite(void)					{ return *mpSprite; }
	
		/**
		* @brief 加算合成モードに切り替えます
		*/
		void Blend_Add(void) 
		{
			mpD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			mpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			mpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		/**
		* @brief 半透明合成モードに切り替えます
		*/
		void Blend_Alpha(void) 
		{
			mpD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			mpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			mpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		/**
		* @brief デバイス側へワールド行列をセットします
		* @param セットしたい行列
		*/
		void SetWorldMatrix(const D3DXMATRIX* apMatrix) {
			
			mpD3DDevice->SetTransform(D3DTS_WORLD, apMatrix);
		}
		/**
		* @brief カメラをセットします
		* @param セットしたいカメラクラス
		*/
		void SetCamera(const CameraSystem& aCamera);
		/**
		* @brief カメラクラスを返します
		*/
		const CameraSystem& GetCamera(void) { return mCamera; }
	private:
		ComPtr<IDirect3D9> mpD3D;
		ComPtr<IDirect3DDevice9> mpD3DDevice;
		D3DPRESENT_PARAMETERS	mD3Dpp;
		D3DLIGHT9				mLight;

		SPtr<Sprite>			mpSprite;
		//カメラ
		CameraSystem			mCamera;

	protected:
		DirectGraphics(void);
		~DirectGraphics(void)override final;
	};
	inline DirectGraphics& Get3D(void) { return DirectGraphics::GetInstance(); }

}