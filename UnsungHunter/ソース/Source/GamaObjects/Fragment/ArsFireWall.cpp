#include "ArsFireWall.h"
#include "ArsFireWallFragment.h"
#include "../../Scene/BaseScene/ArsScene.h"

namespace Alrescha {
	FireWall::FireWall(void)
		: GameObject(eFireWall)
		, mDelayTime()
		, mCreateCount(0)
		, mPosition({ 0.0f,0.0f,0.0f })
		, mPreviousPosition({ 0.0f,0.0f,0.0f })
	{}
	FireWall::~FireWall(void)
	{}
	void FireWall::Initialize(const D3DXVECTOR3& aPosition)
	{
		GameObject::Initialize();

		mPosition = aPosition;

		auto& waitTime = mDelayTime.GetWaitTime("DelayTime");
		waitTime.SetWaitTime(0.01f);
	}
	void FireWall::Update(float aDeltaTime)
	{

		mDelayTime.Update(aDeltaTime);

		if (mDelayTime.IsRegistered("DelayTime")) {
			//遅延時間なら生成処理を行わない
			if (!mDelayTime.GetWaitTime("DelayTime").IsExceed(true)) {
				return;
			}
		}

		CreateFragment();
	}
	void FireWall::CreateFragment(void)
	{
		//生成できる最大数.
		constexpr int MaxCreateNumber = 40;

		//指定した個数を超えている場合生成しない、かつ、自分を消す.
		if (mCreateCount >= MaxCreateNumber) { return; Kill(); }
		auto pFragment = std::make_shared<FireWallFragment>();
		mpScene->AddGameObject(pFragment, 4U);

		//どのぐらいずらすか.
		const float ShiftX = CastFloat((mCreateCount / 10) + Get::Random::Value(0.0f, 0.5f));
		constexpr float ShiftY = 0.0f;
		const float ShiftZ = Get::Random::Value(0.0f, 1.0f);

		//矢の右側に生成.
		if (mCreateCount % 2 == 0) {
			//ずらす量を矢の回転行列で変換.
			D3DXVECTOR3 shift = { ShiftX, ShiftY, ShiftZ };
			D3DXVec3TransformNormal(&shift, &shift, &mRotateMatrix);

			//矢の位置とずらす量.
			D3DXVECTOR3 pos = mPosition + shift;

			pFragment->Initialize(pos);
		}
		//矢の左側に生成.
		else{
			//ずらす量を矢の回転行列で変換.
			D3DXVECTOR3 shift = { -ShiftX, ShiftY, ShiftZ };
			D3DXVec3TransformNormal(&shift, &shift, &mRotateMatrix);

			//矢の位置からずらす量.
			D3DXVECTOR3 pos = mPosition + shift;

			pFragment->Initialize(pos);
		}

		//生成した合計の数を増やす.
		mCreateCount++;
	}
}