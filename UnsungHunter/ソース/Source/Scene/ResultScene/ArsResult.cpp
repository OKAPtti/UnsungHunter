#include "ArsResult.h"
#include "../../Core/Texture/ArsTextureManager.h"
#include "../../Core/Input/ArsInput.h"
#include "../ArsSceneManager.h"
#include "../../Core/DirectX/AllDirectXIncluders.h"
#include "../../Core/ArsConfiguration.h"
#include "../BaseScene/ArsScene.h"
#include "../ArsSceneType.h"

namespace Alrescha {
	Result::Result(void)
		: GameObject(eResult)
		, mpSubTexture(nullptr)
		, mSwitchFlg(true)

	{}
	Result::~Result(void)
	{}
	void Result::Initialize(const GameObjectType& aKilledCharacter)
	{
		//倒されたキャラクターによって、読み込む画像を変更.
		if (aKilledCharacter == ePlayer) {
			mpTexture = GetTexMgr().GetTexture("GAME-OVER.png");
			mpSubTexture = GetTexMgr().GetTexture("CLICK-TO-TITLE.png");
		}
		else if (aKilledCharacter == eBoss) {
			mpTexture = GetTexMgr().GetTexture("GAME-CLEAR.png");
			mpSubTexture = GetTexMgr().GetTexture("CLICK-TO-TITLE.png");
		}
	}
	void Result::Update(float aDeltaTime)
	{
		//SubTextureの点滅処理.
		{
			constexpr int AddSpeed = 4;
			constexpr int MaxAlphaValue = 255;
			//透明にせず、少しだけ見えるようにする.
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
	void Result::Draw2D(void)
	{
		//ずらす量.
		float shiftY = 100.0f;

		//GAME-OVER.
		D3DXMatrixTranslation(&mMatrix, WindowWidth / 2.0f, WindowHeight / 2.0f - shiftY, 0.0f);
		Get3D().GetSprite().DrawTex(*mpTexture, mMatrix);

		shiftY = 190.0f;

		//CLICK-TO-TITLE.
		D3DXMatrixIdentity(&mMatrix);
		D3DXMatrixTranslation(&mMatrix, WindowWidth / 2.0f, WindowHeight - shiftY, 0.0f);
		Get3D().GetSprite().DrawTex(*mpSubTexture, mMatrix,D3DCOLOR_ARGB(mAlphaValue,255,255,255));
	}
}