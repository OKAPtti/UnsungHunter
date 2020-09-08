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
		//HPが0なら.
		if (mpHitPoint->IsEmpty()) { Kill(); }

		if (mIsInvincible) {
			if (mInvincibleTime.GetWaitTime("Destroy").IsExceed(true)) {
				mIsInvincible = false;
			}
		}
		//無敵時間の更新
		mInvincibleTime.Update(aDeltaTime);
	}
	void Character::Draw3D(void)
	{
	}
	void Character::Damage(UINT aDamageValue, const std::string& aDamageName)
	{

		if (mInvincibleTime.IsRegistered(aDamageName)) {
			//無敵時間ならダメージ処理を行わない
			if (!mInvincibleTime.GetWaitTime(aDamageName).IsExceed(true)) {
				mIsInvincible = true;
				return;
			}
		}
		else {
			//新たな名前のダメージなら、初期化を行った後ダメージ処理を行う
			auto& waitTime = mInvincibleTime.GetWaitTime(aDamageName);
			waitTime.SetWaitTime(0.5f);
			waitTime.Recount();
		}

		//HP操作.
		mpHitPoint->Remove(aDamageValue);
	}
	void Character::Cure(UINT aCureValue)
	{
		mpHitPoint->Add(aCureValue);
	}
}