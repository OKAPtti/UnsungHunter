#include "ArsUserInterface.h"
#include "../../Scene/BaseScene/ArsScene.h"
#include "../../Core/Texture/ArsTextureManager.h"
#include "../Character/ArsPlayer.h"
#include "../../Core/ArsConfiguration.h"
#include "../../Core/DirectX/AllDirectXIncluders.h"
#include "../../GamaObjects/Character/ArsArrowType.h"

namespace Alrescha {
	UserInterface::UserInterface(void)
		: GameObject(eUserInterface)
		, mScaleSize({ 1.0f,1.0f,1.0f })
		, mScalingLerper()
		, mpStaminaTexture(nullptr)
		, mpBarFrame(nullptr)
		, mpCrossHairInside(nullptr)
		, mpCrossHairOutside(nullptr)
		, mpNormalArrow(nullptr)
		, mpFireArrow(nullptr)
		, mpIconFrame(nullptr)
		, mpInfinityTexture(nullptr)
		, mpNumberTexture(nullptr)
	{}
	UserInterface::~UserInterface(void)
	{}
	void UserInterface::Initialize(void)
	{
		mpTexture = GetTexMgr().GetTexture("Bar.png");
		mpIconFrame = GetTexMgr().GetTexture("frame.png");
		mpBarFrame = GetTexMgr().GetTexture("Border_Black.png");
		mpStaminaTexture = GetTexMgr().GetTexture("Stamina.png");
		mpCrossHairInside = GetTexMgr().GetTexture("CrossHairInside.png");
		mpCrossHairOutside = GetTexMgr().GetTexture("CrossHairOutside.png");
		mpNormalArrow = GetTexMgr().GetTexture("Arrow.png");
		mpFireArrow = GetTexMgr().GetTexture("FireArrow.png");
		mpInfinityTexture = GetTexMgr().GetTexture("Infinity.png");
		mpNumberTexture = GetTexMgr().GetTexture("Number.png");
	}
	void UserInterface::Draw2D(void)
	{
		if (!mScalingLerper.IsMaxCount()) {
			mScalingLerper.AddCount();
			mScaleSize = mScalingLerper.Lerp();
		}

		//�Ə��̕\��.
		ShowCrossHair();

		//�v���C���[�Ɋւ�����.
		PlayerInfo();

	}
	void UserInterface::CrossHairScaleDown(void)
	{
		const D3DXVECTOR3 MaxScaleSize{ 1.0f,1.0f,1.0f };
		const D3DXVECTOR3 StartPosition(0.45f, 0.45f, 0.45f);
		mScalingLerper.Reset();
		mScalingLerper.SetStart(StartPosition);
		mScalingLerper.SetEnd(MaxScaleSize);
		mScalingLerper.SetEndCount(10);
	}
	void UserInterface::PlayerInfo(void)
	{
		//Player.
		auto pGameObject = mpScene->FindGameObject(ePlayer);
		if (!pGameObject) { return; }

		auto pPlayer = pGameObject->Cast<Player>(ePlayer);
		if (!pPlayer) { return; }

		auto& pSprite = Get3D().GetSprite();
		{//HP.
			const auto& Hp = pPlayer->GetHp();

			//�e�N�X�`���̋�`���.
			const auto Rect = mpTexture->GetRect();
			const auto RightGuage = CastLong(Rect.right * (1 - Hp->Rate()));

			auto tex = mpTexture->GetDirectTexture();
			auto center = mpTexture->GetCenter();
			RECT rect = { Rect.left,Rect.top,Rect.right - RightGuage,Rect.bottom };

			D3DXMatrixTranslation(&mMatrix, 170.0f, 40.0f, 0.0f);
			pSprite.DrawTex(*mpTexture, rect, mMatrix);

			pSprite.DrawTex(*mpTexture,mMatrix,D3DCOLOR_ARGB(125, 255, 255, 255));

			pSprite.DrawTex(*mpBarFrame, mMatrix);
		}
		{//Stamina.
			const auto& Stamina = pPlayer->GetStamina();
			const auto Rect = mpTexture->GetRect();

			const auto LeftGuage = CastLong(Stamina->GetMinValue());
			const auto RightGuage = CastLong(Rect.right * (1 - Stamina->Rate()));

			auto tex = mpStaminaTexture->GetDirectTexture();
			auto center = mpStaminaTexture->GetCenter();
			RECT rect = { Rect.left,Rect.top,Rect.right - RightGuage,Rect.bottom };

			D3DXMatrixTranslation(&mMatrix, 170.0f, 75.0f, 0.0f);
			pSprite.DrawTex(*mpStaminaTexture, rect, mMatrix);

			pSprite.DrawTex(*mpStaminaTexture, mMatrix, D3DCOLOR_ARGB(125, 255, 255, 255));

			pSprite.DrawTex(*mpBarFrame, mMatrix);
		}

		{//�I�����Ă�A�C�e���A�܂��͑I������Ă���A�C�e���̑O��ɂ���A�C�e����\��.
			//���ݑI�����Ă����̃^�C�v���擾.
			const auto& SelectArrowType = pPlayer->GetArrowType();

			//��ʂ̍����ɕ`�悳���1/4�~.
			D3DXMATRIX lowerLeft;
			{
				D3DXMatrixTranslation(&lowerLeft, 0.0f, WindowHeight, 0.0f);
				auto& pSprite = Get3D().GetSprite();

				const D3DXVECTOR3 ScaleSize(1.75f, 1.75f, 1.75f);
				D3DXMatrixScaling(&mScaleMatrix, ScaleSize.x, ScaleSize.y, ScaleSize.z);
				pSprite.DrawTex(*mpIconFrame, mScaleMatrix * lowerLeft);
			}
			//�c�����擾.
			const auto& NumOfArrow = pPlayer->GetNumberOfArrows();

			{//�I�����ꂽ��̕`��ʒu�Ǝc���̕\��.
				if (SelectArrowType == ArrowType::eNormalArrow) {
					D3DXMatrixTranslation(&mTransMatrix, 40.0f, WindowHeight - 20.0f, 0.0f);
					const D3DXVECTOR3 ScaleSize(0.75f, 0.75f, 0.75f);
					D3DXMatrixScaling(&mScaleMatrix, ScaleSize.x, ScaleSize.y, ScaleSize.z);
					pSprite.DrawTex(*mpInfinityTexture, mScaleMatrix * mTransMatrix);
				}
				else {
					//��̍ő�.
					const auto MaxNum = NumOfArrow->GetMaxValue();

					//�ő吔�ɍ��킹�������̉摜�͈͂��v�Z.
					const LONG Height = 66;
					const LONG Width = 38;
					RECT rect = { Width * MaxNum,0,Width * MaxNum + Width,Height };

					//�`�悷�鍂��.
					constexpr float DrawHeight = WindowHeight - 20.0f;

					D3DXMatrixTranslation(&mTransMatrix, 210.0f, DrawHeight, 0.0f);
					const D3DXVECTOR3 ScaleSize(0.75f, 0.75f, 0.75f);
					D3DXMatrixScaling(&mScaleMatrix, ScaleSize.x, ScaleSize.y, ScaleSize.z);
					pSprite.DrawTex(*mpNumberTexture, rect, mScaleMatrix * mTransMatrix);

					//�ő吔�Ǝc���̊Ԃɂ���X���b�V��.
					rect = { 418 - 50,0,Width * 10 + Width,Height };
					D3DXMatrixTranslation(&mTransMatrix, 185.0f, DrawHeight, 0.0f);
					pSprite.DrawTex(*mpNumberTexture, rect, mScaleMatrix * mTransMatrix);

					//�c��.
					const auto CurrentNum = NumOfArrow->GetCurrentValue();
					//�c���ɍ��킹�������̉摜�͈͂��v�Z.
					rect = { Width * CurrentNum,0,Width * CurrentNum + Width,Height };
					D3DXMatrixTranslation(&mTransMatrix, 165.0f, DrawHeight, 0.0f);
					pSprite.DrawTex(*mpNumberTexture, rect, mScaleMatrix * mTransMatrix);
				}
			}

			//��ʉ����猩��1�Ԗڂ̉~.
			const D3DXVECTOR3 ScaleSize(0.5f, 0.5f, 0.5f);
			D3DXMatrixScaling(&mScaleMatrix,ScaleSize.x, ScaleSize.y, ScaleSize.z);
			D3DXMatrixTranslation(&mTransMatrix, 160.0f, -35.0f, 0.0f);
			D3DXMatrixRotationZ(&mRotateMatrix, D3DXToRadian(0.0f));
			mMatrix = mScaleMatrix * mTransMatrix * mRotateMatrix * lowerLeft;
			pSprite.DrawTex(*mpIconFrame, mMatrix);

			//�I������Ă���A�C�e���̂ЂƂO��\��.
			if (SelectArrowType == ArrowType::eNormalArrow) {
				pSprite.DrawTex(*mpFireArrow, mMatrix);
			}
			else if (SelectArrowType == ArrowType::eFireArrow) {
				pSprite.DrawTex(*mpNormalArrow, mMatrix);
			}

			//2�Ԗ�.
			D3DXMatrixRotationZ(&mRotateMatrix, D3DXToRadian(-30.0f));
			D3DXMatrixScaling(&mScaleMatrix, 0.85f, 0.85f, 0.85f);
			mMatrix = mScaleMatrix * mTransMatrix * mRotateMatrix * lowerLeft;
			pSprite.DrawTex(*mpIconFrame, mMatrix);

			//��]�v�f����菜��.
			mMatrix = *D3DXMatrixInverse(&D3DXMATRIX(), nullptr, &mRotateMatrix) * mMatrix;

			//�I������Ă���A�C�e����\��.
			//�I�����ꂽ�A�C�e���̎c����0�̏ꍇ�������ɂ���.
			int alphaValue = 255;
			if (NumOfArrow->IsEmpty()) {
				alphaValue = 127;
			}
			
			if (SelectArrowType == ArrowType::eNormalArrow) {
				pSprite.DrawTex(*mpNormalArrow, mMatrix);
			}
			else if (SelectArrowType == ArrowType::eFireArrow) {
				pSprite.DrawTex(*mpFireArrow, mMatrix, D3DCOLOR_ARGB(alphaValue, 255, 255, 255));
			}

			//3�Ԗ�.
			D3DXMatrixRotationZ(&mRotateMatrix, D3DXToRadian(-60.0f));
			D3DXMatrixScaling(&mScaleMatrix, 0.5f, 0.5f, 0.5f);
			mMatrix = mScaleMatrix * mTransMatrix * mRotateMatrix * lowerLeft;
			pSprite.DrawTex(*mpIconFrame, mMatrix);

			//��]�v�f����菜��.
			mMatrix = *D3DXMatrixInverse(&D3DXMATRIX(), nullptr, &mRotateMatrix) * mMatrix;

			//�I������Ă���A�C�e���̂ЂƂ��\��.
			if (SelectArrowType == ArrowType::eNormalArrow) {
				pSprite.DrawTex(*mpFireArrow, mMatrix);
			}
			else if (SelectArrowType == ArrowType::eFireArrow) {
				pSprite.DrawTex(*mpNormalArrow, mMatrix);
			}
		}
	}
	void UserInterface::ShowCrossHair(void)
	{
		auto& pSprite = Get3D().GetSprite();

		{//CrossHairInside.
			auto tex = mpCrossHairInside->GetDirectTexture();
			auto rect = mpCrossHairInside->GetRect();
			auto center = mpCrossHairInside->GetCenter();

			D3DXMatrixTranslation(&mMatrix, WindowHalfWidth, WindowHalfHeight, 0.0f);
			pSprite.DrawTex(*mpCrossHairInside, mMatrix);
		}
		{//CrossHairOutside.
			auto tex = mpCrossHairOutside->GetDirectTexture();
			auto rect = mpCrossHairOutside->GetRect();
			auto center = mpCrossHairOutside->GetCenter();

			D3DXMatrixTranslation(&mMatrix, WindowHalfWidth, WindowHalfHeight, 0.0f);
			D3DXMatrixScaling(&mScaleMatrix, mScaleSize.x, mScaleSize.y, mScaleSize.z);
			mMatrix = mScaleMatrix * mMatrix;
			pSprite.DrawTex(*mpCrossHairOutside, mMatrix);
		}
	}
}