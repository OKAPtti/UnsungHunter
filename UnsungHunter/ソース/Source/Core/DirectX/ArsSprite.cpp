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
		//�X�v���C�g����.
		D3DXCreateSprite(Get3D().GetDevice(), &mpSprite);
	}
	void Sprite::Release(void)
	{
		SafeRelease(mpSprite);
	}
	void Sprite::DrawTex(const Texture& aTexture, const D3DXMATRIX& aMatrix, D3DCOLOR aColor)
	{
		SetTransform(aMatrix);
		mpSprite->Draw(aTexture.GetDirectTexture(),	//�`�悷��e�N�X�`��
			&aTexture.GetRect(),	//�`�悷��͈�(Rect)
			&aTexture.GetCenter(), nullptr, aColor);
	}
	void Sprite::DrawTex(const Texture& aTexture, RECT aRect, const D3DXMATRIX& aMatrix, D3DCOLOR aColor)
	{
		SetTransform(aMatrix);
		mpSprite->Draw(aTexture.GetDirectTexture(),	//�`�悷��e�N�X�`��
			&aRect,	//�`�悷��͈�(Rect)
			&aTexture.GetCenter(), nullptr, aColor);
	}
}