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
		//�|���ꂽ�L�����N�^�[�ɂ���āA�ǂݍ��މ摜��ύX.
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
		//SubTexture�̓_�ŏ���.
		{
			constexpr int AddSpeed = 4;
			constexpr int MaxAlphaValue = 255;
			//�����ɂ����A��������������悤�ɂ���.
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
		//���炷��.
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