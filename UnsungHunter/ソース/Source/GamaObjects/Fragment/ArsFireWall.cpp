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
			//�x�����ԂȂ琶���������s��Ȃ�
			if (!mDelayTime.GetWaitTime("DelayTime").IsExceed(true)) {
				return;
			}
		}

		CreateFragment();
	}
	void FireWall::CreateFragment(void)
	{
		//�����ł���ő吔.
		constexpr int MaxCreateNumber = 40;

		//�w�肵�����𒴂��Ă���ꍇ�������Ȃ��A���A����������.
		if (mCreateCount >= MaxCreateNumber) { return; Kill(); }
		auto pFragment = std::make_shared<FireWallFragment>();
		mpScene->AddGameObject(pFragment, 4U);

		//�ǂ̂��炢���炷��.
		const float ShiftX = CastFloat((mCreateCount / 10) + Get::Random::Value(0.0f, 0.5f));
		constexpr float ShiftY = 0.0f;
		const float ShiftZ = Get::Random::Value(0.0f, 1.0f);

		//��̉E���ɐ���.
		if (mCreateCount % 2 == 0) {
			//���炷�ʂ��̉�]�s��ŕϊ�.
			D3DXVECTOR3 shift = { ShiftX, ShiftY, ShiftZ };
			D3DXVec3TransformNormal(&shift, &shift, &mRotateMatrix);

			//��̈ʒu�Ƃ��炷��.
			D3DXVECTOR3 pos = mPosition + shift;

			pFragment->Initialize(pos);
		}
		//��̍����ɐ���.
		else{
			//���炷�ʂ��̉�]�s��ŕϊ�.
			D3DXVECTOR3 shift = { -ShiftX, ShiftY, ShiftZ };
			D3DXVec3TransformNormal(&shift, &shift, &mRotateMatrix);

			//��̈ʒu���炸�炷��.
			D3DXVECTOR3 pos = mPosition + shift;

			pFragment->Initialize(pos);
		}

		//�����������v�̐��𑝂₷.
		mCreateCount++;
	}
}