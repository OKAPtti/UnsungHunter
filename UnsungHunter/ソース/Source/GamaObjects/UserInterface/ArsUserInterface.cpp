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

		//照準の表示.
		ShowCrossHair();

		//プレイヤーに関する情報.
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

			//テクスチャの矩形情報.
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

		{//選択してるアイテム、または選択されているアイテムの前後にあるアイテムを表示.
			//現在選択している矢のタイプを取得.
			const auto& SelectArrowType = pPlayer->GetArrowType();

			//画面の左下に描画される1/4円.
			D3DXMATRIX lowerLeft;
			{
				D3DXMatrixTranslation(&lowerLeft, 0.0f, WindowHeight, 0.0f);
				auto& pSprite = Get3D().GetSprite();

				const D3DXVECTOR3 ScaleSize(1.75f, 1.75f, 1.75f);
				D3DXMatrixScaling(&mScaleMatrix, ScaleSize.x, ScaleSize.y, ScaleSize.z);
				pSprite.DrawTex(*mpIconFrame, mScaleMatrix * lowerLeft);
			}
			//残数を取得.
			const auto& NumOfArrow = pPlayer->GetNumberOfArrows();

			{//選択された矢の描画位置と残数の表示.
				if (SelectArrowType == ArrowType::eNormalArrow) {
					D3DXMatrixTranslation(&mTransMatrix, 40.0f, WindowHeight - 20.0f, 0.0f);
					const D3DXVECTOR3 ScaleSize(0.75f, 0.75f, 0.75f);
					D3DXMatrixScaling(&mScaleMatrix, ScaleSize.x, ScaleSize.y, ScaleSize.z);
					pSprite.DrawTex(*mpInfinityTexture, mScaleMatrix * mTransMatrix);
				}
				else {
					//矢の最大.
					const auto MaxNum = NumOfArrow->GetMaxValue();

					//最大数に合わせた数字の画像範囲を計算.
					const LONG Height = 66;
					const LONG Width = 38;
					RECT rect = { Width * MaxNum,0,Width * MaxNum + Width,Height };

					//描画する高さ.
					constexpr float DrawHeight = WindowHeight - 20.0f;

					D3DXMatrixTranslation(&mTransMatrix, 210.0f, DrawHeight, 0.0f);
					const D3DXVECTOR3 ScaleSize(0.75f, 0.75f, 0.75f);
					D3DXMatrixScaling(&mScaleMatrix, ScaleSize.x, ScaleSize.y, ScaleSize.z);
					pSprite.DrawTex(*mpNumberTexture, rect, mScaleMatrix * mTransMatrix);

					//最大数と残数の間にあるスラッシュ.
					rect = { 418 - 50,0,Width * 10 + Width,Height };
					D3DXMatrixTranslation(&mTransMatrix, 185.0f, DrawHeight, 0.0f);
					pSprite.DrawTex(*mpNumberTexture, rect, mScaleMatrix * mTransMatrix);

					//残数.
					const auto CurrentNum = NumOfArrow->GetCurrentValue();
					//残数に合わせた数字の画像範囲を計算.
					rect = { Width * CurrentNum,0,Width * CurrentNum + Width,Height };
					D3DXMatrixTranslation(&mTransMatrix, 165.0f, DrawHeight, 0.0f);
					pSprite.DrawTex(*mpNumberTexture, rect, mScaleMatrix * mTransMatrix);
				}
			}

			//画面下から見て1番目の円.
			const D3DXVECTOR3 ScaleSize(0.5f, 0.5f, 0.5f);
			D3DXMatrixScaling(&mScaleMatrix,ScaleSize.x, ScaleSize.y, ScaleSize.z);
			D3DXMatrixTranslation(&mTransMatrix, 160.0f, -35.0f, 0.0f);
			D3DXMatrixRotationZ(&mRotateMatrix, D3DXToRadian(0.0f));
			mMatrix = mScaleMatrix * mTransMatrix * mRotateMatrix * lowerLeft;
			pSprite.DrawTex(*mpIconFrame, mMatrix);

			//選択されているアイテムのひとつ前を表示.
			if (SelectArrowType == ArrowType::eNormalArrow) {
				pSprite.DrawTex(*mpFireArrow, mMatrix);
			}
			else if (SelectArrowType == ArrowType::eFireArrow) {
				pSprite.DrawTex(*mpNormalArrow, mMatrix);
			}

			//2番目.
			D3DXMatrixRotationZ(&mRotateMatrix, D3DXToRadian(-30.0f));
			D3DXMatrixScaling(&mScaleMatrix, 0.85f, 0.85f, 0.85f);
			mMatrix = mScaleMatrix * mTransMatrix * mRotateMatrix * lowerLeft;
			pSprite.DrawTex(*mpIconFrame, mMatrix);

			//回転要素を取り除く.
			mMatrix = *D3DXMatrixInverse(&D3DXMATRIX(), nullptr, &mRotateMatrix) * mMatrix;

			//選択されているアイテムを表示.
			//選択されたアイテムの残数が0の場合半透明にする.
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

			//3番目.
			D3DXMatrixRotationZ(&mRotateMatrix, D3DXToRadian(-60.0f));
			D3DXMatrixScaling(&mScaleMatrix, 0.5f, 0.5f, 0.5f);
			mMatrix = mScaleMatrix * mTransMatrix * mRotateMatrix * lowerLeft;
			pSprite.DrawTex(*mpIconFrame, mMatrix);

			//回転要素を取り除く.
			mMatrix = *D3DXMatrixInverse(&D3DXMATRIX(), nullptr, &mRotateMatrix) * mMatrix;

			//選択されているアイテムのひとつ後を表示.
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