#include "ArsTitle.h"
#include "../../Core/Texture/ArsTextureManager.h"
#include "../../Core/DirectX/ArsSprite.h"
#include "../../Core/DirectX/ArsDirectGraphics.h"
#include "../../Core/ArsConfiguration.h"

namespace Alrescha {

	Title::Title(void)
		: GameObject(eTitle)
		, mpSubTexture(nullptr)
		, mAlphaValue(0)
		, mSwitchFlg(false)
	{
	}

	Title::~Title(void)
	{
	}
	void Title::Initialize(void)
	{
		mpTexture = GetTexMgr().GetTexture("Unsung-Hunter.png");
		mpSubTexture = GetTexMgr().GetTexture("CLICK-TO-START.png");
		mpBackGroundTexture = GetTexMgr().GetTexture("TitleBackGround.png");
	}
	void Title::Update(float aDeltaTime)
	{
		{//“_–Åˆ—.
			constexpr int AddSpeed = 4;
			constexpr int MaxAlphaValue = 255;
			constexpr int MinAlphaValue = 30;

			if (!mSwitchFlg) { mAlphaValue += AddSpeed; }
			else mAlphaValue -= AddSpeed;

			if (mAlphaValue > MaxAlphaValue) {
				mAlphaValue = MaxAlphaValue;
				mSwitchFlg = true;
			}

			if (mAlphaValue < MinAlphaValue) {
				mAlphaValue = MinAlphaValue;
				mSwitchFlg = false;
			}
		}
	}
	void Title::Draw2D(void)
	{

		auto& pSprite = Get3D().GetSprite();
		//Unsung-Hunter.
		D3DXMatrixIdentity(&mMatrix);
		D3DXMatrixTranslation(&mMatrix, WindowWidth / 2.0f, WindowHeight / 2.0f, 0.0f);
		pSprite.DrawTex(*mpBackGroundTexture, mMatrix);

		//‚¸‚ç‚·—Ê.
		float shiftY = 100.0f;

		//”wŒi.
		D3DXMatrixIdentity(&mMatrix);
		D3DXMatrixTranslation(&mMatrix, WindowWidth / 2.0f, WindowHeight / 2.0f - shiftY, 0.0f);
		pSprite.DrawTex(*mpTexture, mMatrix);

		shiftY = 190.0f;

		//CLICKTOSTART.
		D3DXMatrixIdentity(&mMatrix);
		D3DXMatrixTranslation(&mMatrix, WindowWidth / 2.0f, WindowHeight - shiftY, 0.0f);
		pSprite.DrawTex(*mpSubTexture, mMatrix,D3DCOLOR_ARGB(mAlphaValue,255,255,255));
	}
}