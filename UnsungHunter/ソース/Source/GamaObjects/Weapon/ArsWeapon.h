/**
* @file ArsWeapon.h
* @brief 武器に必要なものを集めたクラス
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {

	class Weapon :public GameObject
	{
	public:
		Weapon(int aType);
		virtual ~Weapon(void);
		/**
		* @brief 初期化をします
		*/
		virtual void Initialize(void);
		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		virtual void Update(float aDeltaTime);
		/**
		* @brief 3D描画をします
		*/
		virtual void Draw3D(void);

	private:
	protected:
		//与えるダメージ量.
		int mDamage;

		//当ったか.
		bool mIsHit;
	};
}