#include "ArsHindRightLeg.h"

namespace Alrescha {
	HindRightLeg::HindRightLeg(void)
		: ChildGameObject(eHindRightLeg)
		, mIsReverse(false)
	{}
	HindRightLeg::~HindRightLeg(void)
	{}

	void HindRightLeg::Initialize(const std::string& aModelName)
	{
		ChildGameObject::Initialize(aModelName);
	}

	void HindRightLeg::UpdateTransform(const D3DXMATRIX& aParentMatrix, float aDeltaTime)
	{
		D3DXMATRIX tmpMat;
		D3DXMatrixIdentity(&tmpMat);

		auto parent = mpParent.lock();
		if (!parent) { return; }
		if (parent->GetType() == eBoss) {
			D3DXMatrixTranslation(&tmpMat, -0.275f, -0.005f, 0.9f);
		}
		else
		{
			D3DXMatrixTranslation(&tmpMat, -0.1f, 0.0f, 0.3f);
		}

		mTransMatrix = tmpMat * aParentMatrix;

		mMatrix = mRotateMatrix * mTransMatrix;
	}

	void HindRightLeg::Draw(float aAlphaValue)
	{
		ChildGameObject::Draw(aAlphaValue);
	}

	void HindRightLeg::DrawRed(void)
	{
		ChildGameObject::DrawRed();
	}

	void HindRightLeg::WalkAnimation(void)
	{
		D3DXQUATERNION StartQua, EndQua, NowQua;

		D3DXMATRIX frontMat, backMat;

		constexpr float FrontDeg = 3.0f;
		constexpr float BackDeg = -3.0f;

		D3DXMatrixRotationX(&frontMat, D3DXToRadian(FrontDeg));
		D3DXMatrixRotationX(&backMat, D3DXToRadian(BackDeg));

		D3DXQuaternionRotationMatrix(&StartQua, &frontMat);
		D3DXQuaternionRotationMatrix(&EndQua, &backMat);

		D3DXQuaternionSlerp(&NowQua, &StartQua, &EndQua, mAnimeFrame);


		constexpr float FrameSize = 0.125f / 4;

		if (!mIsReverse) {
			mAnimeFrame += FrameSize;
		}
		else {
			mAnimeFrame -= FrameSize;
		}

		if (mAnimeFrame > 1.0f) {
			mIsReverse = true;
		}
		if (mAnimeFrame < 0.0f) {
			mIsReverse = false;
		}

		D3DXMATRIX nowMat;
		D3DXMatrixRotationQuaternion(&nowMat, &NowQua);

		mRotateMatrix = nowMat * mRotateMatrix;
	}

}