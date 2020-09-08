#include "ArsFrontRightLeg.h"

namespace Alrescha {
	FrontRightLeg::FrontRightLeg(void)
		: ChildGameObject(eFrontRightLeg)
		, mIsReverse(false)
	{}
	FrontRightLeg::~FrontRightLeg(void)
	{}
	void FrontRightLeg::Initialize(const std::string & aModelName)
	{
		ChildGameObject::Initialize(aModelName);
	}
	void FrontRightLeg::UpdateTransform(const D3DXMATRIX& aParentMatrix, float aDeltaTime)
	{
		D3DXMATRIX tmpMat;
		D3DXMatrixIdentity(&tmpMat);

		auto parent = mpParent.lock();
		if (!parent) { return; }
		if (parent->GetType() == eBoss) {
			D3DXMatrixTranslation(&tmpMat, -0.2575f, -0.25f, -0.65f);
		}
		else{
			D3DXMatrixTranslation(&tmpMat, -0.075f, -0.1f, -0.275f);
		}

		mTransMatrix = tmpMat * aParentMatrix;


		mMatrix = mRotateMatrix * mTransMatrix;
	}
	void FrontRightLeg::Draw(float aAlphaValue)
	{
		ChildGameObject::Draw(aAlphaValue);
	}

	void FrontRightLeg::DrawRed()
	{
		ChildGameObject::DrawRed();
	}

	void FrontRightLeg::WalkAnimation(void)
	{

		D3DXQUATERNION StartQua, EndQua, NowQua;

		D3DXMATRIX frontMat, backMat;

		D3DXMatrixRotationX(&frontMat, D3DXToRadian(3.0f));
		D3DXMatrixRotationX(&backMat, D3DXToRadian(-3.0f));

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