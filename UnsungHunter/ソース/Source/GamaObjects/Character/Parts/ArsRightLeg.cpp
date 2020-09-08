#include "ArsRightLeg.h"
#include "../ArsPlayer.h"

namespace Alrescha {
	RightLeg::RightLeg(void)
		: ChildGameObject(eRightLeg)
	{}
	RightLeg::~RightLeg(void)
	{}
	void RightLeg::Initialize(const std::string& aModelName)
	{
		ChildGameObject::Initialize(aModelName);
	}
	void RightLeg::UpdateTransform(const D3DXMATRIX& aParentMatrix, float aDeltaTime)
	{
		D3DXMATRIX parentMat;
		auto parent = mpParent.lock();
		if (parent != nullptr) {
			mTransMatrix = parent->GetTransMatrix();
			parentMat = parent->GetRotateMatrix();
		}

		//親からどれくらい離すか.
		D3DXVECTOR3 vec(0.1f, -0.19f, 0.0f);
		D3DXVec3TransformCoord(&vec, &vec, &parentMat);

		mRotateMatrix = parentMat;

		D3DXMATRIX tmpMat;
		D3DXMatrixTranslation(&tmpMat, vec.x, vec.y, vec.z);
		mTransMatrix = tmpMat * mTransMatrix;

		auto player = parent->Cast<Player>(ePlayer);
		if (!player) { return; }
		//動いていればアニメーション.
		if (player->GetIsMove()) {
			WalkAnimation();
		}

		mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;
	}
	void RightLeg::Draw(float aAlphaValue)
	{
		ChildGameObject::Draw(aAlphaValue);
	}
	void RightLeg::WalkAnimation(void)
	{
		D3DXQUATERNION StartQua, EndQua, NowQua;

		D3DXMATRIX frontMat, backMat;

		D3DXMatrixRotationX(&frontMat, D3DXToRadian(-10.0f));
		D3DXMatrixRotationX(&backMat, D3DXToRadian(10.0f));

		D3DXQuaternionRotationMatrix(&StartQua, &frontMat);
		D3DXQuaternionRotationMatrix(&EndQua, &backMat);

		D3DXQuaternionSlerp(&NowQua, &StartQua, &EndQua, mAnimeFrame);


		constexpr float frameSize = 0.0475f;
		static bool IsReverse = false;

		if (!IsReverse) {
			mAnimeFrame += frameSize;
		}
		else {
			mAnimeFrame -= frameSize;
		}

		if (mAnimeFrame > 1.0f) {
			IsReverse = true;
		}
		if (mAnimeFrame < 0.0f) {
			IsReverse = false;
		}

		D3DXMATRIX nowMat;
		D3DXMatrixRotationQuaternion(&nowMat, &NowQua);

		mRotateMatrix = nowMat * mRotateMatrix;
	}
}