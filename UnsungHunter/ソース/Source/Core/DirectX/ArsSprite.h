/**
* @file ArsSprite.h
* @brief スプライトを制御するクラス
*/

#pragma once

namespace Alrescha {
	class Texture;
	class Sprite
	{
	public:
		Sprite(void);
		~Sprite(void) { Release(); }

		/**
		* @brief 初期化をします
		*/
		void Initialize(void);
		/**
		* @brief スプライトを解放します
		*/
		void Release(void);
	
		/**
		* @brief 描画を開始します
		*/
		bool Begin(void) {
			HRESULT hr = mpSprite->Begin(D3DXSPRITE_ALPHABLEND);
			if (hr == D3D_OK) { return true; }
			return false;
		}
		
		/**
		* @brief 描画を終了します
		*/
		void End(void) { mpSprite->End(); }
		
		/**
		* @brief 変換行列をセットします
		* @param セットしたい変換行列
		*/
		void SetTransform(const D3DXMATRIX& aMatrix) {
			mpSprite->SetTransform(&aMatrix);
		}

		/**
		* @brief スプライト描画をします
		* @param 描画したいテクスチャ
		* @param セットしたい変換行列
		* @param 色(指定無しなら白)
		*/
		void DrawTex(const Texture& aTexture,const D3DXMATRIX& aMatrix, D3DCOLOR aColor = -1);
		
		/**
		* @brief スプライト描画をします
		* @param 描画したいテクスチャ
		* @param 描画したいテクスチャの範囲
		* @param セットしたい変換行列
		* @param 色(指定無しなら白)
		*/
		void DrawTex(const Texture& aTexture, RECT aRect, const D3DXMATRIX& aMatrix, D3DCOLOR aColor = -1);

	private:
		LPD3DXSPRITE	mpSprite;
		//文字表示用フォント
		LPD3DXFONT		mpFont;
		//↑のフォント情報
		D3DXFONT_DESC	mFontDesc;
	};

}