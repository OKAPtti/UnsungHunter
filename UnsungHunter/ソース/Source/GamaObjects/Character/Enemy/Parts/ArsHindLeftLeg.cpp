#include "ArsHindLeftLeg.h"

namespace Alrescha {
	HindLeftLeg::HindLeftLeg(void)
		: ChildGameObject(eHindLeftLeg)
		, mIsReverse(false)
	{}
	HindLeftLeg::~HindLeftLeg(void)
	{}
	void HindLeftLeg::Initialize(const std::string & aModelName)
	{
		ChildGameObject::Initialize(aModelName);
	}
	void HindLeftLeg::UpdateTransform(const D3DXMATRIX& aParentMatrix, float aDeltaTime)
	{
		D3DXMATRIX tmpMat;
		D3DXMatrixIdentity(&tmpMat);
		
		auto parent = mpParent.lock();
		if (!parent) { return; }
		if (parent->GetType() == eBoss) {
			D3DXMatrixTranslation(&tmpMat, 0.275f, -0.005f, 0.9f);
		}
		else{
			D3DXMatrixTranslation(&tmpMat, 0.1f, 0.0f, 0.3f);
		}


		mTransMatrix = tmpMat * aParentMatrix;

		mMatrix = mRotateMatrix * mTransMatrix;
	}
	void HindLeftLeg::Draw(float aAlphaValue)
	{
		ChildGameObject::Draw(aAlphaValue);
	}
	void HindLeftLeg::DrawRed(void)
	{
		ChildGameObject::DrawRed();
	}
	void HindLeftLeg::WalkAnimation(void)
	{
		D3DXQUATERNION StartQua, EndQua, NowQua;

		D3DXMATRIX frontMat, backMat;

		constexpr float FrontDeg = -3.0f;
		constexpr float BackDeg = 3.0f;

		D3DXMatrixRotationX(&frontMat, D3DXToRadian(FrontDeg));
		D3DXMatrixRotationX(&backMat, D3DXToRadian(BackDeg));

		D3DXQuaternionRotationMatrix(&StartQua, &frontMat);
		D3DXQuaternionRotationMatrix(&EndQua, &backMat);

		D3DXQuaternionSlerp(&NowQua, &StartQua, &EndQua, mAnimeFrame);


		constexpr float frameSize = 0.125f / 4;

		if (!mIsReverse) {
			mAnimeFrame += frameSize;
		}
		else {
			mAnimeFrame -= frameSize;
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