#include "ArsCharacter.h"

namespace Alrescha {
	Character::Character(int aType)
		: GameObject(aType)
		, mpChild(nullptr)
		, mpHitPoint(nullptr)
		, mpStamina(nullptr)
		, mInvincibleTime()
		, mIsInvincible(false)
		, mIsHit(false)
	{}
	Character::~Character(void)
	{}
	void Character::Initialize(void)
	{
		GameObject::Initialize();

		auto& waitTime = mInvincibleTime.GetWaitTime("FireWall");
		waitTime.SetWaitTime(0.25f);

		mInvincibleTime.Register("FireArrow", 0.5f);
		mInvincibleTime.Register("Destroy", 0.5f);
		mInvincibleTime.GetWaitTime("Destroy").ForceCountToExceed();
	}
	void Character::Update(float aDeltaTime)
	{
		//HP��0�Ȃ�.
		if (mpHitPoint->IsEmpty()) { Kill(); }

		if (mIsInvincible) {
			if (mInvincibleTime.GetWaitTime("Destroy").IsExceed(true)) {
				mIsInvincible = false;
			}
		}
		//���G���Ԃ̍X�V
		mInvincibleTime.Update(aDeltaTime);
	}
	void Character::Draw3D(void)
	{
	}
	void Character::Damage(UINT aDamageValue, const std::string& aDamageName)
	{

		if (mInvincibleTime.IsRegistered(aDamageName)) {
			//���G���ԂȂ�_���[�W�������s��Ȃ�
			if (!mInvincibleTime.GetWaitTime(aDamageName).IsExceed(true)) {
				mIsInvincible = true;
				return;
			}
		}
		else {
			//�V���Ȗ��O�̃_���[�W�Ȃ�A���������s������_���[�W�������s��
			auto& waitTime = mInvincibleTime.GetWaitTime(aDamageName);
			waitTime.SetWaitTime(0.5f);
			waitTime.Recount();
		}

		//HP����.
		mpHitPoint->Remove(aDamageValue);
	}
	void Character::Cure(UINT aCureValue)
	{
		mpHitPoint->Add(aCureValue);
	}
}