#include "ArsSprite.h"
#include "ArsDirectGraphics.h"
#include "../Texture/ArsTexture.h"

namespace Alrescha {
	Sprite::Sprite(void)
		: mpSprite(nullptr)
		, mpFont(nullptr)
		, mFontDesc()
	{}
	void Sprite::Initialize(void)
	{
		//スプライト生成.
		D3DXCreateSprite(Get3D().GetDevice(), &mpSprite);
	}
	void Sprite::Release(void)
	{
		SafeRelease(mpSprite);
	}
	void Sprite::DrawTex(const Texture& aTexture, const D3DXMATRIX& aMatrix, D3DCOLOR aColor)
	{
		SetTransform(aMatrix);
		mpSprite->Draw(aTexture.GetDirectTexture(),	//描画するテクスチャ
			&aTexture.GetRect(),	//描画する範囲(Rect)
			&aTexture.GetCenter(), nullptr, aColor);
	}
	void Sprite::DrawTex(const Texture& aTexture, RECT aRect, const D3DXMATRIX& aMatrix, D3DCOLOR aColor)
	{
		SetTransform(aMatrix);
		mpSprite->Draw(aTexture.GetDirectTexture(),	//描画するテクスチャ
			&aRect,	//描画する範囲(Rect)
			&aTexture.GetCenter(), nullptr, aColor);
	}
}