#include "ArsRightArm.h"
#include "../ArsPlayer.h"
#include "../../../Core/Input/ArsInput.h"

namespace Alrescha {
	RightArm::RightArm(void)
		: ChildGameObject(eRightArm)
	{}
	RightArm::~RightArm(void)
	{}
	void RightArm::Initialize(const std::string& aModelName)
	{
		ChildGameObject::Initialize(aModelName);
	}
	void RightArm::UpdateTransform(const D3DXMATRIX& aParentMatrix, float aDeltaTime)
	{
		D3DXMATRIX parentMat;
		auto parent = mpParent.lock();
		if (parent != nullptr) {
			mTransMatrix = parent->GetTransMatrix();
			parentMat = parent->GetRotateMatrix();
		}

		//親からどれくらい離すか.
		D3DXVECTOR3 vec(0.16f, 0.2f, 0.0f);
		D3DXVec3TransformCoord(&vec, &vec, &parentMat);

		mRotateMatrix = parentMat;

		D3DXMATRIX tmpMat;
		D3DXMatrixTranslation(&tmpMat, vec.x, vec.y, vec.z);
		mTransMatrix = tmpMat * mTransMatrix;

		auto player = parent->Cast<Player>(ePlayer);
		if (!player) { return; }
		//動いていて、かつ、弓を構えていなければ.
		if (player->GetIsMove() && !player->GetIsBendBow()) {
			WalkAnimation();
		}
		//弓を構えていてかつ、矢を放とうとしていれば.
		else if (player->GetIsBendBow() && player->GetIsShootArrow()) {
			HoldingBow();
			D3DXMATRIX rotate;
			D3DXMatrixRotationZ(&rotate, D3DXToRadian(-60.0f));
			mRotateMatrix = rotate * mRotateMatrix;
		}
		else {
			mAnimeFrame = 0.0f;
		}

		mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;
	}
	void RightArm::Draw(float aAlphaValue)
	{
		ChildGameObject::Draw(aAlphaValue);
	}
	void RightArm::WalkAnimation(void)
	{
		D3DXQUATERNION startQua, endQua, nowQua;

		D3DXMATRIX frontMat, backMat;
		D3DXMatrixIdentity(&frontMat);
		D3DXMatrixIdentity(&backMat);

		constexpr float FrontDeg = 40.0f;
		constexpr float BackDeg = -40.0f;

		D3DXMatrixRotationX(&frontMat, D3DXToRadian(FrontDeg));
		D3DXMatrixRotationX(&backMat, D3DXToRadian(BackDeg));

		D3DXQuaternionRotationMatrix(&startQua, &frontMat);
		D3DXQuaternionRotationMatrix(&endQua, &backMat);

		D3DXQuaternionSlerp(&nowQua, &startQua, &endQua, mAnimeFrame);

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
		D3DXMatrixRotationQuaternion(&nowMat, &nowQua);

		mRotateMatrix = nowMat * mRotateMatrix;
	}
	void RightArm::HoldingBow(void)
	{
		D3DXQUATERNION StartQua, EndQua, NowQua;

		D3DXMATRIX frontMat, backMat;
		D3DXMatrixIdentity(&frontMat);
		D3DXMatrixIdentity(&backMat);

		constexpr float FrontDeg = -90.0f;
		constexpr float BackDeg = -90.0f;

		D3DXMatrixRotationX(&frontMat, D3DXToRadian(FrontDeg));
		D3DXMatrixRotationX(&backMat, D3DXToRadian(BackDeg));

		D3DXQuaternionRotationMatrix(&StartQua, &frontMat);
		D3DXQuaternionRotationMatrix(&EndQua, &backMat);

		D3DXQuaternionSlerp(&NowQua, &StartQua, &EndQua, mAnimeFrame);

		D3DXMATRIX nowMat;
		D3DXMatrixIdentity(&nowMat);
		D3DXMatrixRotationQuaternion(&nowMat, &NowQua);

		D3DXVECTOR3 StartPos, EndPos, NowPos;
		//スタート位置.
		StartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//ゴール位置.
		EndPos = D3DXVECTOR3(0.25f, 0.0f, -0.075f);
		D3DXVec3Lerp(&NowPos, &StartPos, &EndPos, mAnimeFrame);

		constexpr float frameSize = 0.035f;
		static bool IsReverse = false;

		if (!IsReverse) {
			mAnimeFrame += frameSize;
		}
		else {
			mAnimeFrame -= frameSize;
		}

		if (mAnimeFrame > 1.0f) {
			mAnimeFrame = 1.0f;
		}

		D3DXMATRIX parentRotMat, parentTransMat;
		auto parent = mpParent.lock();
		if (parent == nullptr) {
			return;
		}
		parentRotMat = parent->GetRotateMatrix();
		parentTransMat = parent->GetTransMatrix();


		mRotateMatrix = nowMat * parentRotMat;

		D3DXVec3TransformCoord(&NowPos, &NowPos, &parentRotMat);

		D3DXMATRIX tmpMat;
		D3DXMatrixIdentity(&tmpMat);
		tmpMat._41 = NowPos.x;
		tmpMat._42 = NowPos.y;
		tmpMat._43 = NowPos.z;

		mTransMatrix = tmpMat * mTransMatrix;
	}
}