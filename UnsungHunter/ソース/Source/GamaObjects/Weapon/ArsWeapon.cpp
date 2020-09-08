#include "ArsWeapon.h"

namespace Alrescha {
	Weapon::Weapon(int aType)
		: GameObject(aType)
		, mDamage(30)
		, mIsHit(false)
	{}
	Weapon::~Weapon(void)
	{}
	void Weapon::Initialize(void)
	{
		GameObject::Initialize();
	}
	void Weapon::Update(float aDeltaTime)
	{
	}
	void Weapon::Draw3D(void)
	{
		GameObject::Draw3D();
	}
}